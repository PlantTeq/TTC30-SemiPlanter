/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_NodeID.h
 *
 * \brief IO Driver functions for reading the NodeID pins.
 * 
 * \attention NodeID pin functionality is NOT provided for HY-TTC32H or HY-TTC32SH, since those pins are used for the second CAN interface.
 * 
 * The HY-TTC 30 has two connector pins #IO_ADC_NODE_ID_0 and
 * #IO_ADC_NODE_ID_1 which allow two change the NodeID of the ECU.
 * \n
 * A NodeID pin can be connected to either battery voltage, sensor supply,
 * ground or left floating.
 *
 * The final NodeID used by the bootloader and/or the application, is calculated
 * with
 * \code
 * NodeID = BaseID + Modifier
 * \endcode
 * where \c BaseID is the value stored in the field \c NodeNumber of the APDB (see #ApdbType)
 * and \c Modifier the value read from the NodeID pins according to the following
 * table for non-safety related devices:
 *
 * \verbatim[table]
 * ============= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========
 * Modifier      +0        +1        +2        +3        +4        +5        +6        +7        +8        +9
 * ------------- --------- --------- --------- --------- --------- --------- --------- --------- --------- ---------
 * \bgb NODEID_0 Floating  Ground    SS        UBAT+     Ground    SS        UBAT+     Ground    SS        UBAT+
 * ------------- --------- --------- --------- --------- --------- --------- --------- --------- --------- ---------
 * \bgb NODEID_1 Floating  Ground    Ground    Ground    SS        SS        SS        UBAT+     UBAT+     UBAT+
 * ------------- --------- --------- --------- --------- --------- --------- --------- --------- --------- ---------
 * \endverbatim
 *
 * For safety-related devices the following table applies:
 *
 * \verbatim[table]
 * ============= ========= ========= ========= =========
 * Modifier      +0        +3        +5        +7
 * ------------- --------- --------- --------- ---------
 * \bgb NODEID_0 Floating  UBAT+     SS        Ground
 * ------------- --------- --------- --------- ---------
 * \bgb NODEID_1 Floating  Ground    SS        UBAT+
 * ------------- --------- --------- --------- ---------
 * \endverbatim
 *
 * All combinations not mentioned in the above table are an invalid state.
 * The \c Modifier determined by the bootloader is saved to the EEPROM
 * automatically.\n
 *
 * The \c Modifier and actual NodeID determined by the bootloader can be
 * retrieved using the function #IO_NodeID_GetModifierStartup. The current
 * state of the NodeID pins can be retrieve dusing the function #IO_NodeID_GetModifier.
 *
 * \section nodeid_examples NodeID code examples
 *
 * Please refer to section \ref appl_structure for understanding where to place the initialization and task function calls.
 *
 * Startup NodeID initialization example: \n
 * Reading the NodeID at startup does not need initialization.
 *
 * Startup NodeID read example:
 * \code
 * ubyte1 modifier;
 * ubyte1 nodeid;
 *
 * IO_NodeID_GetModifierStartup( &modifier   // will contain the NodeID modifier (0..9)
 *                             , &nodeid);   // will contain the used NodeID (0..127)
 *
 * \endcode
 * Current NodeID initialization example: \n
 * \code
 * // The range parameter is ignored for these inputs
 * IO_ADC_ChannelInit( IO_ADC_UBAT, IO_ADC_ABSOLUTE, 0, NULL );          // Initialize analog channel of UBAT
 * IO_ADC_ChannelInit( IO_ADC_NODE_ID_0, IO_ADC_ABSOLUTE, 0, NULL );     // Initialize analog channel of NODEID_0
 * IO_ADC_ChannelInit( IO_ADC_NODE_ID_1, IO_ADC_ABSOLUTE, 0, NULL );     // Initialize analog channel of NODEID_1
 * \endcode
 *
 * Current NodeID read example:
 * \code
 * bool fresh;
 * ubyte2 ubat;
 * ubyte2 nodeid_0;
 * ubyte2 nodeid_1;
 * ubyte1 modifier;
 *
 * IO_ADC_Get( IO_ADC_UBAT, &ubat, &fresh);            // Get voltage level of UBAT
 * IO_ADC_Get( IO_ADC_NODE_ID_0, &nodeid_0, &fresh);   // Get voltage level of NODEID_0
 * IO_ADC_Get( IO_ADC_NODE_ID_1, &nodeid_1, &fresh);   // Get voltage level of NODEID_1
 *
 * IO_NodeID_GetModifier( ubat
 *                      , nodeid_0
 *                      , nodeid_1
 *                      , &modifier );     // Contains the modifier corresponding
 *                                         // to the current voltage levels
 *                                         // on NodeID pins
 * \endcode
 *
 **************************************************************************/
#ifndef _IO_NODEID_H
#define _IO_NODEID_H

/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/

#include "IO_Driver.h"

/**************************************************************************
 *
 * D E F I N I T I O N S
 *
 **************************************************************************/

/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/**************************************************************************
 *
 * F U N C T I O N S
 *
 **************************************************************************/
/***********************************************************************//**
 *
 * \brief Retrieves the Modifier and the NodeID used on ECU startup by
 *        the bootloader.
 *
 * \param modifier              returned NodeID Modifier (Range: 0..9)
 * \param nodeid                returned NodeID (Range: 0..127 for non-safety
 *                              variants, 0..64 for safety-variants)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_NODEID_PINS_INVALID         content of EEPROM and pins invalid (no valid
 *                                           modifier could be determined)
 * \retval #IO_E_NODEID_EEPROM_FALLBACK      Value of NodeID pins was invalid but
 *                                           EEPROM values could be used as Modifier
 * \retval #IO_E_NODEID_EEPROM_INVALID       Modifier stored in the EEPROM was not valid and rewritten
 *                                           with the current Modifier read from NodeID pins
 * \retval #IO_E_NODEID_EEPROM_MISMATCH      Both modifier gotten from pins and EEPROM are valid,
 *                                           but did not match on startup (values from the NodeID
 *                                           pins have been used)
 * \retval #IO_E_SW_INTERNAL                 an internal error has occurred
 * \retval #IO_E_NULL_POINTER                a NULL pointer has been passed to the function
 *
 *
 *
 * \remarks
 *   The following five combinations are possible for the calculation of the
 *   \c modifier by the bootloader:
 *
 * \remarks
 *   - Both, the current value of the NodeID pins and the values stored in the
 *   EEPROM are valid and match: \n
 *   The function returns #IO_E_OK, the value
 *   of the parameter \c modifier corresponds to the modifier determined via the
 *   NodeID pins (and matches the EEPROM values).
 *
 * \remarks
 *   - Both, the current value of the NodeID pins and the values stored in the
 *   EEPROM are valid but do not match (because, e.g., wiring has changed): \n
 *   The function returns #IO_E_NODEID_EEPROM_MISMATCH, the value
 *   of the parameter \c modifier corresponds to the modifier determined via the
 *   NodeID pins. Before starting the application, the bootloader has updated
 *   the EEPROM values with the current state of the pins such that upon the
 *   next start-up the function returns #IO_E_OK if the pin values do not change.
 *
 * \remarks
 *   - The value of the NodeID pins is valid but the values stored in the EEPROM
 *   are invalid (e.g. first start-up of device or after the EEPROM has been
 *   cleared):\n
 *   The function returns #IO_E_NODEID_EEPROM_INVALID, the value
 *   of the parameter \c modifier corresponds to the modifier determined via the
 *   NodeID pins. Before starting the application, the bootloader has updated
 *   the EEPROM values with the current state of the pins such that upon the
 *   next start-up the function returns #IO_E_OK if the pin values do not change.
 *
 * \remarks
 *   - The value of the NodeID pins is invalid and the values stored in the EEPROM
 *   are valid:\n
 *   The bootloader will use the values in the EEPROM to calculate the modifier,
 *   the function returns #IO_E_NODEID_EEPROM_FALLBACK. This is an error case
 *   typically provoked by errors in wiring and should be checked by the
 *   application - the state of the NodeID pins has to be valid in a
 *   typical execution.
 *
 * \remarks
 *   - Both, the current value of the NodeID pins and the values stored in the
 *   EEPROM are invalid: \n
 *   This bootloader will use the default NodeID 0xA, the function returns
 *   #IO_E_NODEID_PINS_INVALID.
 *
 * \remarks
 *   If necessary, the application can reproduce this check by using the
 *   \c modifier parameter and the \c nodeNr field from the APDB to calculate
 *   the acutal \c nodeid and comparing it to the value provided by this function.
 *
 *
 *
 ***************************************************************************/
IO_ErrorType IO_NodeID_GetModifierStartup ( ubyte1 * const modifier
                                          , ubyte1 * const nodeid );

/***********************************************************************//**
 *
 * \brief Calculates the modifier for the given voltage levels.
 *
 * \param ubat         Battery voltage (#IO_ADC_UBAT) in mV
 * \param nodeid_0     Voltage of pin #IO_ADC_NODE_ID_0 [mV]
 * \param nodeid_1     Voltage of pin #IO_ADC_NODE_ID_1 [mV]
 * \param modifier     Calculated modifier (Range: 0..9)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                    everything fine
 * \retval #IO_E_NODEID_PINS_INVALID   the given voltage levels represent
 *                                     no valid Modifier
 * \retval #IO_E_NULL_POINTER          a null pointer has been passed
 *
 * \remarks
 *   It is recommended to check the validity of the sensor supply voltage before
 *   calling the #IO_NodeID_GetModifier() function. Otherwise it is possible
 *   that an invalid value is computed (e.g. if the sensor supply is
 *   short-circuited to GND).
 *
 ***************************************************************************/
IO_ErrorType IO_NodeID_GetModifier ( ubyte2 ubat
                                   , ubyte2 nodeid_0
                                   , ubyte2 nodeid_1
                                   , ubyte1 * const modifier );

#endif /* _IO_NODEID_H */
