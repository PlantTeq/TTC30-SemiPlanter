/*
 * canbus.c
 *
 *  Created on: Dec 15, 2022
 *      Author: Matthijs
 */
#include <string.h>
#include "Canbus.h"
#include "Sysconf.h"
#include "Control.h"
#include "IO_UART.h"
#include "UART.h"
#include "CanHandler.h"

#include "eeprom.h"
#include "params.h"
#include "stddefs.h"


#include "can_messages.h"
#include "ApdbCfg.h"
#include "IO_CAN.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>



static IO_ErrorType can_init_rc;


float speedDegCAN;

static volatile bool SpacingConfigPending = FALSE;


#define PGN_REQUEST_CONFIG1	0x00B00102L

#define PGN_SAVE_CONFIG1	0x00C00102L

#define PGN_COMMAND			0x00D00102L
#define PGN_STATUS_COMMAND	0x00A00202L
#define PGN_SPEED_SENSOR	0x18FF0B15L


static void handleCanRx(UI08_t bCanNum, CanFrame_t* pFrame);

void CanbusInit()
{
	CanInit(0, 250000, TRUE);
	CanSetRxHandler(0, handleCanRx);

	planter.spdCanReceived = FALSE;

}


void handleCanRx(UI08_t bCanNum, CanFrame_t* pFrame)
{
	if (bCanNum == 0) {
		// Voorbeeld frame data

		CanFrame_t can_frame;
		can_frame.id = pFrame->id;
		can_frame.length = pFrame->length;
		can_frame.data[0] = pFrame->data[0];
		can_frame.data[1] = pFrame->data[1];
		can_frame.data[2] = pFrame->data[2];
		can_frame.data[3] = pFrame->data[3];
		can_frame.data[4] = pFrame->data[4];
		can_frame.data[5] = pFrame->data[5];
		can_frame.data[6] = pFrame->data[6];
		can_frame.data[7] = pFrame->data[7];

		unsigned short data = 0;

		switch (can_frame.id)
		{
		case PGN_REQUEST_CONFIG1:
			UART_Printf (IO_UART, "received CAN getconfig ID\r\n");
			SendConfig1();
		break;

		case PGN_SAVE_CONFIG1:
			UART_Printf (IO_UART, "received CAN save config1\r\n");


			Save();
		break;
		
	
	
		case PGN_SPEED_SENSOR:

			memcpy(&data, can_frame.data+2, 2);
			ubyte2 tempdata = EndianSwap16(data);
			planter.PlantWheelDeg = (float) tempdata /10;
	


			planter.spdCanReceived = TRUE;
	

			break;

		default:
			// Alle protocolberichten worden gedecodeerd naar CanMsgs; verwerking gebeurt in de hoofdlus
			CanMessages_Receive(can_frame.id, pFrame->extended != 0, can_frame.data, can_frame.length);
			break;

		}

	}
}

// Verwerkt ontvangen protocolberichten en beantwoordt configverzoeken. Wordt vanuit de
// hoofdlus aangeroepen, zodat er niet vanuit de ontvangsthandler gezonden wordt.
void CanProcessRx(void)
{
	MsgPlantSpacingConfig_t spacingMsg;
	const Param_t *Param;

	if (CanMsgs.PlantSpacingCommand.Updated)
	{
		CanMsgs.PlantSpacingCommand.Updated = false;
		// De controller toetst zelf aan min/max, ook als het display dat al deed
		if (!ParamsSet(CONFIGGROUP_PLANTSPACING, CanMsgs.PlantSpacingCommand.Data.PlantSpacingSetpoint))
		{
			UART_Printf (IO_UART, "plant spacing rejected: %u\r\n", CanMsgs.PlantSpacingCommand.Data.PlantSpacingSetpoint);
		}
		SpacingConfigPending = TRUE;
	}

	if (CanMsgs.ConfigRequest.Updated)
	{
		CanMsgs.ConfigRequest.Updated = false;
		if (CanMsgs.ConfigRequest.Data.ConfigGroup == CONFIGGROUP_ALL ||
			CanMsgs.ConfigRequest.Data.ConfigGroup == CONFIGGROUP_PLANTSPACING)
		{
			SpacingConfigPending = TRUE;
		}
	}

	if (!SpacingConfigPending)
	{
		return;
	}
	SpacingConfigPending = FALSE;

	Param = ParamsFind(CONFIGGROUP_PLANTSPACING);
	spacingMsg.PlantSpacingCurrent = *Param->Value;
	spacingMsg.PlantSpacingDefault = Param->Default;
	spacingMsg.PlantSpacingMin = Param->Min;
	spacingMsg.PlantSpacingMax = Param->Max;
	MsgPlantSpacingConfig_send(&spacingMsg);
}

// Enige platformspecifieke koppeling van can_messages: een frame op bus 0 versturen.
bool CanMessages_Transmit(uint32_t id, bool extended, const uint8_t *data, uint8_t dlc)
{
	CanFrame_t can_tx_frame;

	memset(&can_tx_frame, 0, sizeof(can_tx_frame));
	memcpy(can_tx_frame.data, data, dlc);
	can_tx_frame.extended = extended ? IO_CAN_EXT_FRAME : IO_CAN_STD_FRAME;
	can_tx_frame.length = dlc;
	can_tx_frame.id = id;

	CanTxFrame(0, &can_tx_frame);
	return true;
}

void SendCanInfo(void)
{
	CanFrame_t can_tx_frame;
	ubyte4 tempvalue;


	can_tx_frame.extended = IO_CAN_EXT_FRAME;
	can_tx_frame.length = 5;
	can_tx_frame.id =  0x00A00101;

	

    CanTxFrame(0, &can_tx_frame);
}





// Stuurt de actuele snelheid (planter.speed in m/s) als mm/s naar het display.
void SendCanSpeed(void)
{
	MsgSpeedStatus_t speedMsg;

	speedMsg.SpeedActual = (int32_t)(planter.speed * 1000.0f);
	MsgSpeedStatus_send(&speedMsg);
}

// Stuurt het toerental van het plantwiel (RPM, afgeleid uit de hoeksensor op
// PlantWheelSensorPin). Moet elke 100 ms worden aangeroepen.
void SendCanPlantWheelSpeed(void)
{
	static float previousDeg = 0.0f;
	static bool initialized = FALSE;

	MsgPlantWheelSpeed_t wheelMsg;
	float currentDeg;
	float deltaDeg;

	// Direct uit de ADC: planter.PlantWheelDeg wordt door ADCDataCheck gefilterd
	// en blijft daardoor stil bij stilstand of draaien in omgekeerde richting.
	if (cfg.maxPlantWheelADC <= cfg.minPlantWheelADC)
	{
		return;
	}
	currentDeg = MapF((float)planter.PlantWheelADC, (float)cfg.minPlantWheelADC, (float)cfg.maxPlantWheelADC, 0.0f, 360.0f);

	deltaDeg = currentDeg - previousDeg;
	previousDeg = currentDeg;
	if (initialized == FALSE)
	{
		initialized = TRUE;
		return;
	}

	// De hoek loopt rond bij 360 graden; neem de kortste weg.
	if (deltaDeg > 180.0f) deltaDeg -= 360.0f;
	if (deltaDeg < -180.0f) deltaDeg += 360.0f;
	if (deltaDeg < 0.0f) deltaDeg = -deltaDeg;

	// graden per 100 ms -> omwentelingen per minuut
	wheelMsg.PlantwheelSpeed = deltaDeg / 360.0f * 600.0f;
	MsgPlantWheelSpeed_send(&wheelMsg);
}

void SendConfig1(void)
{
	ubyte4 tempvalue;
	CanFrame_t can_tx_frame;

	can_tx_frame.extended = IO_CAN_EXT_FRAME;
	can_tx_frame.length = 8;
	can_tx_frame.id =  0x00B00101;


	CanTxFrame(0, &can_tx_frame);
}




