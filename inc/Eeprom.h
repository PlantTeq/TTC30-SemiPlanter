/*
 * eeprom.h
 *
 *  Created on: Jan 13, 2022
 *      Author: Matthijs
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_
#include "APDB.h"


void EepromInit (void);
ubyte1 EepromWrite (ubyte2 dataAddress, ubyte2 length, bool CRC, ubyte1 *bufferPtr);
ubyte1 EepromRead (ubyte2 dataAddress, ubyte2 length, bool CRC, ubyte1 *bufferPtr);
void Save(void);

#endif /* INC_EEPROM_H_ */
