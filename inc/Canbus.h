/*
 * Canbus.h
 *
 *  Created on: Dec 15, 2022
 *      Author: Matthijs
 */

#ifndef CANBUS_H_
#define CANBUS_H_

void CanbusInit(void);

void SendCanInfo(void);

void SendConfig1(void);


extern float speedDegCAN;

#endif /* CANBUS_H_ */
