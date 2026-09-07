/*
 * control.c
 *
 *  Created on: Dec 15, 2022
 *      Author: Matthijs
 */
#include <string.h>
#include "control.h"
#include "sysconf.h"
#include "IO_Driver.h"
#include "IO_ADC.h"
#include "IO_DIO.h"
#include "IO_ADC.h"
#include "UART.h"
#include "IO_PWM.h"
#include "IO_Vout.h"
#include "IO_PWD.h"
#include "eeprom.h"


#include "Canbus.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>




#define PLANT_WHEEL_HOLDER_COUNT 4
#define MIN_PLANT_SPEED 0.025f /* m/s; below this the plant wheel is disabled */

long Map(long x, long in_min, long in_max, long out_min, long out_max);
float MapF(float x, float in_min, float in_max, float out_min, float out_max);
bool ADCDataCheck(ubyte2 ADC, ubyte2 lastADC, ubyte2 minADC, ubyte2 maxADC);
float CalculateWheelSpeed(ubyte2 wheelEncoderCount, ubyte2 pulsesPerMeter);
static void UpdatePlantWheelControl(void);



/****************************************************************************/
/**                                                                        **/
/**                     GLOBAL VARIABLES                                   **/
/**                                                                        **/
/****************************************************************************/

static IO_ErrorType io_error_control;



ubyte2 minTargetAmp;
ubyte2 maxTargetAmp;
ubyte2 speedAdcCalibrationSamples;


ubyte2 do_voltage_fb;
bool speedAdcCalibrated;
bool dataSPDCheck;
static ubyte2 previousWheelEncoderCount;
static ubyte1 wheelSpeedSampleCount;
static bool wheelSpeedInitialized;
static float previousPlantWheelDeg;
static bool plantWheelControlInitialized;







/****************************************************************************/
/**                                                                        **/
/**                          Functions                                     **/
/**                                                                        **/
/****************************************************************************/

void ControlInit(void)
{

    planter.estopValue = FALSE;
    planter.targetAmp = 150;


	previousWheelEncoderCount = 0;
	wheelSpeedSampleCount = 0;
	wheelSpeedInitialized = FALSE;
	previousPlantWheelDeg = 0;
	plantWheelControlInitialized = FALSE;


    // PID controller configuration
    IO_PID_CONFIG pid_cfg =
    {
    		.Kd = 0U,             // Differnetial gain scaled by 10000 => gain of 0.05
    		.Kff = 100U,          // Feed-forwared gain scaled by 1000 => gain of 10
    		.Ki = 220000U,           // Integral gain scaled by 10000 => gain of 15
    		.Kp = 750U,     //35000        // Proportional gain sclaed by 1000 => agin of 0.05
    		.max_limit = 55704,    // Maximal allowed duty cycle is 85%
    		.min_limit = 0U,        // Minimal allowed duty cycle is 0% (0)
    };



    //initialize spd sensor
    io_error_control = IO_PWD_CountInit(WheelEncoderPin, IO_PWD_RISING_COUNT, IO_PWD_UP_COUNT, 0, IO_PWD_PD, NULL);
    if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO-init-error enc spd sensor %u \r\n", io_error_control);

    //Init plantwheel sensor
    io_error_control = IO_ADC_ChannelInit(PlantWheelSensorPin, IO_ADC_RATIOMETRIC, IO_ADC_RANGE_5V, NULL);
    if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO-init-error adc pos sensor %u \r\n", io_error_control);

	

    //Init sensor Estop
    io_error_control = IO_DI_Init(EstopPin, IO_DI_PD, NULL);
    if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO-init-error estop sensor %u \r\n", io_error_control);

    //Init enable
    io_error_control = IO_DI_Init(EnablePin, IO_DI_PD, NULL);
    if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO-init-error enable sensor %u \r\n", io_error_control);

#
	// Setup a current-controlled PWM
	io_error_control = IO_PWM_CurrentInit( PlantWheelValvePin
			, 100            // frequency is 100 Hz
			, &pid_cfg       // use custom PID configuration
			, 10             // use 10ms as cycle time for the PID controller
			, NULL           // Handle for PID controller not needed => pass NULL
			, NULL );        // safety configuration; currently not supported
	if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO-init-error enable sensor %u \r\n", io_error_control);


    minTargetAmp = 350;
    maxTargetAmp = 2000;


}

void ControlUpdate(void)
{

    int i;
	ubyte2 loopTimeMs = 10;
    dataSPDCheck = FALSE;

	// read ADC status speed sensor
	io_error_control = IO_ADC_Get(PlantWheelSensorPin, &planter.PlantWheelADC, &planter.PlantWheelADCFresh);
	if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO-ADC-get-error adc sensor spd %u \r\n", io_error_control);

	// read current of PWM output
	io_error_control = IO_PWM_GetCur(PlantWheelValvePin, &planter.PWMcurrent, &planter.PWMfresh);
	if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO-PWM-current-error current %u \r\n", io_error_control);

	// read Estop
	io_error_control = IO_DI_Get(EstopPin, &planter.estopValue);
	if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO-get-error estop %u \r\n", io_error_control);

	// read enable value
	io_error_control = IO_DI_Get(EnablePin, &planter.enableValue);
	if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO-get-error enable %u \r\n", io_error_control);

	io_error_control = IO_PWD_CountGet (WheelEncoderPin, &planter.wheelEncoderCount,NULL, &planter.wheelEncoderFresh);
	if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO-get-error encoder %u \r\n", io_error_control);

	if (wheelSpeedInitialized == FALSE)
	{
		previousWheelEncoderCount = planter.wheelEncoderCount;
		wheelSpeedSampleCount = 0;
		wheelSpeedInitialized = TRUE;
	}
	else
	{
		wheelSpeedSampleCount++;
		if (wheelSpeedSampleCount >= 10)
		{
			planter.speed = CalculateWheelSpeed(planter.wheelEncoderCount, cfg.pulsesPerMeter);
			wheelSpeedSampleCount = 0;
		}
	}
	
	if (io_error_control != IO_E_OK)
	{
		UART_Printf (IO_UART, "Error %u \r\n", io_error_control);
		return;
	}

	if(planter.spdCanReceived == FALSE)
	{
		if(cfg.speedAdcCalibrationEnabled)
		{
			if(speedAdcCalibrated == FALSE)
			{
				if(planter.PlantWheelADC > cfg.maxPlantWheelADC && planter.PlantWheelADC <=5000) cfg.maxPlantWheelADC = planter.PlantWheelADC;
				if(planter.PlantWheelADC < cfg.minPlantWheelADC && planter.PlantWheelADC > 0) cfg.minPlantWheelADC = planter.PlantWheelADC;

				if(planter.PlantWheelADC > 0 && speedAdcCalibrationSamples < 50)
				{
					speedAdcCalibrationSamples++;
				}

				if(speedAdcCalibrationSamples >= 50 && cfg.maxPlantWheelADC > cfg.minPlantWheelADC)
				{
					//speedAdcCalibrated = TRUE;
				}
			}
		}

		dataSPDCheck = ADCDataCheck(planter.PlantWheelADC, planter.lastPlantWheelADC, cfg.minPlantWheelADC, cfg.maxPlantWheelADC);

		if(dataSPDCheck)
		{
			//convert spd ADC to degrees
			planter.PlantWheelDeg = MapF((float)planter.PlantWheelADC, (float)cfg.minPlantWheelADC,(float)cfg.maxPlantWheelADC,0.0f,360.0f);
		}
	}

	
    if(planter.estopValue)
    {
	   //Control Output when estop pin is high	
	   if(planter.enableValue && planter.speed >= MIN_PLANT_SPEED)
	   {
		io_error_control = IO_PWM_SetCur(PlantWheelValvePin, (ubyte2) (planter.targetAmp +0.5) , NULL );
		if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO current error %u \r\n", io_error_control);
	   }
	   else
	   {
		io_error_control = IO_PWM_SetCur(PlantWheelValvePin, 0, NULL );
		if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO current error %u \r\n", io_error_control);
	   }
	}
	else
	{
		io_error_control = IO_PWM_SetCur(PlantWheelValvePin, 0, NULL );
		if (io_error_control != IO_E_OK) UART_Printf (IO_UART, "IO current error %u \r\n", io_error_control);
	}

   //set history vars
   planter.lastPlantWheelADC = planter.PlantWheelADC;



}

void ControlUpdatePlantWheel(void)
{
	UpdatePlantWheelControl();
}

float CalculateWheelSpeed(ubyte2 wheelEncoderCount, ubyte2 pulsesPerMeter)
{
	ubyte2 pulseDifference;

	if (pulsesPerMeter == 0)
	{
		previousWheelEncoderCount = wheelEncoderCount;
		return 0.0f;
	}

	pulseDifference = (ubyte2)(wheelEncoderCount - previousWheelEncoderCount);
	previousWheelEncoderCount = wheelEncoderCount;

	return ((float)pulseDifference / (float)pulsesPerMeter) / 0.1f;
}

static void UpdatePlantWheelControl(void)
{
	float holderSpacing;
	float speedUpRatio;

	if (planter.enableValue == FALSE || planter.speed < MIN_PLANT_SPEED)
	{
		return;
	}

	if (plantWheelControlInitialized == FALSE)
	{
		previousPlantWheelDeg = planter.PlantWheelDeg;
		plantWheelControlInitialized = TRUE;
		return;
	}

	planter.measuredAngleDelta = planter.PlantWheelDeg - previousPlantWheelDeg;
	previousPlantWheelDeg = planter.PlantWheelDeg;

	if (planter.measuredAngleDelta > 180.0f) planter.measuredAngleDelta -= 360.0f;
	if (planter.measuredAngleDelta < -180.0f) planter.measuredAngleDelta += 360.0f;

	if (cfg.plantWheelPerimeter <= 0.0f || cfg.plantDistance <= 0.0f || cfg.targetAmpStep <= 0.0f)
	{
		return;
	}

	// holder spacing on the wheel (perimeter/holders) vs desired plant spacing sets how much faster the wheel must spin than 1:1 ground-driven
	holderSpacing = cfg.plantWheelPerimeter / PLANT_WHEEL_HOLDER_COUNT;
	speedUpRatio = holderSpacing / cfg.plantDistance;

	// loop runs every 100ms (dt = 0.1s); natural 1:1 ground-driven rate scaled by speedUpRatio
	planter.targetAngleDelta = (planter.speed / cfg.plantWheelPerimeter) * 360.0f * speedUpRatio * 0.1f;

	{
		float error = planter.targetAngleDelta - planter.measuredAngleDelta;
		float deadband = 0.05f; /* degrees; ignore noise-level errors to avoid hunting */

		if (error > deadband || error < -deadband)
		{
			planter.targetAmp += error * cfg.targetAmpStep;
		}
	}

	if (planter.targetAmp > (float)maxTargetAmp) planter.targetAmp = (float)maxTargetAmp;
	if (planter.targetAmp < (float)minTargetAmp) planter.targetAmp = (float)minTargetAmp;
}

bool ADCDataCheck(ubyte2 ADC, ubyte2 lastADC, ubyte2 minADC, ubyte2 maxADC)
{
	bool dataCheck = FALSE;

	if(ADC <= (lastADC + 10) && ((lastADC + 10) < maxADC) && (ADC > lastADC))
	{
		dataCheck = TRUE;
	}

	if((lastADC + 10) > maxADC)
	{
		if(ADC > lastADC && ADC < maxADC)
		{
			dataCheck = TRUE;
		}

		if(ADC <= (maxADC - lastADC + minADC))
		{
			dataCheck = TRUE;
		}
	}

	return dataCheck;
}

long Map(long x, long in_min, long in_max, long out_min, long out_max)
{
    long temp_x = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

    // limit to range

    if(temp_x<out_min) temp_x = out_min;
    if(temp_x>out_max) temp_x = out_max;

    return temp_x;
}

float MapF(float x, float in_min, float in_max, float out_min, float out_max)
{
    float temp_x = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

    // limit to range

    if(temp_x<out_min) temp_x = out_min;
    if(temp_x>out_max) temp_x = out_max;

    return temp_x;
}
