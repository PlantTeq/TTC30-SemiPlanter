/*
 * Sysconf.h
 *
 *  Created on: Dec 15, 2022
 *      Author: Matthijs
 */

#ifndef SYSCONF_H_
#define SYSCONF_H_

#include "APDB.h"
#include "stddefs.h"


#define NODE_NR         1
#define UART_BUFSIZE  256


//#define	DEBUG
//#define AUTOMATICUP

#define USE_PARAM_FOR_EXTENDED_CAN 1
#define NR_OF_CAN 1
#define CAN_TX_BUFSIZE 20


typedef struct {

    float       speed;
  
    bool        estopValue;
    bool        enableValue;
    ubyte2      PWMcurrent;
    bool        PWMfresh;
    float		targetAmp;

    ubyte2		PlantWheelADC;
    ubyte2		lastPlantWheelADC;
    bool 		PlantWheelADCFresh;
    ubyte2      PlantWheelDeg;
    bool        spdCanReceived;

    ubyte2      wheelEncoderCount;
    bool        wheelEncoderFresh;

    


}Planter_t;

extern Planter_t planter;

typedef struct {
    float	plantDistance;
    float	wheelPerimeter;

    ubyte2	pulsesPerMeter;
    float	targetAmpStep;

    ubyte2	checkWord;


}CFG_t;

extern CFG_t cfg;




#define EstopPin            IO_PIN_F4 	
#define EnablePin           IO_PIN_G4 		
#define PlantWheelSensorPin IO_PIN_E4
#define WheelEncoderPin     IO_PIN_E4		


#define PlantWheelValvePin  IO_PIN_H1







#endif /* SYSCONF_H_ */
