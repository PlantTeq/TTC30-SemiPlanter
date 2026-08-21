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
#include "stddefs.h"


#include "ApdbCfg.h"
#include "IO_CAN.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>



static IO_ErrorType can_init_rc;


float speedDegCAN;


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

		}

	}
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





void SendConfig1(void)
{
	ubyte4 tempvalue;
	CanFrame_t can_tx_frame;

	can_tx_frame.extended = IO_CAN_EXT_FRAME;
	can_tx_frame.length = 8;
	can_tx_frame.id =  0x00B00101;


	CanTxFrame(0, &can_tx_frame);
}




