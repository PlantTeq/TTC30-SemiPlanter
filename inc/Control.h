/*
 * Control.h
 *
 *  Created on: Dec 15, 2022
 *      Author: Matthijs
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include "APDB.h"
#include "stddefs.h"

void ControlInit (void);

void ControlUpdate (void);

void ControlUpdatePlantWheel (void);

float MapF(float x, float in_min, float in_max, float out_min, float out_max);









#endif /* CONTROL_H_ */
