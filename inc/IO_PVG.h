/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_PVG.h
 *
 * \brief IO Driver functions for PVG channels.
 *
 *   Contains all service functions for the PVG (Proportional Valve Group) outputs.
 *   Up to 6 channels can be configured: \c #IO_PVG_00 .. \c #IO_PVG_05
 *
 *   - The pins for PVG- and Voltage-Outputs are organized in two groups of three:
 *     - Group 1: \c #IO_PVG_00/\c #IO_VOUT_00, \c #IO_PVG_01/\c #IO_VOUT_01 and
 *                \c #IO_PVG_02/\c #IO_VOUT_02
 *     - Group 2: \c #IO_PVG_03/\c #IO_VOUT_03, \c #IO_PVG_04/\c #IO_VOUT_04 and
 *                \c #IO_PVG_05/\c #IO_VOUT_05
 *
 *     Whenever a pin within a group is configured either as PVG or voltage output, all
 *     other pins within the same group must be either remain unconfigured, or be configured
 *     with the same type. For example, if \c #IO_PIN_J2 (\c #IO_PVG_01/\c #IO_VOUT_01)
 *     is configured as PVG output, pins \c #IO_PIN_K2 (\c #IO_PVG_00/\c #IO_VOUT_00)
 *     and \c #IO_PIN_H2 (\c #IO_PVG_02/\c #IO_VOUT_02) must either remain unconfigured
 *     or used as PVG-outputs. Initializing it as any other
 *     output type will result in a IO_E_GROUP_CONFLICT error.
 *
 *   - The outputs will only be activated after enabling them via
 *     \code
 *     IO_POWER_Set(IO_INT_PVG_VOUT_0_ENABLE, IO_POWER_ON);
 *     IO_POWER_Set(IO_INT_PVG_VOUT_1_ENABLE, IO_POWER_ON);
 *     \endcode
 *     After activating the outputs, it is not possible to initialize any further PVG
 *     channels.
 *
 *   - Recommended initialization/usage order:
 *     1) Initialize ALL needed PVG-channels.
 *     2) Call IO_POWER_Set(IO_INT_PVG_VOUT_x_ENABLE, IO_POWER_ON);
 *     3) Set desired output value with IO_PVG_SetOutput.
 *
 *   - After enabling the outputs by calling IO_POWER_Set, all configured PVG channels
 *     will output the desired value, while all other pins within this group will output
 *     0V constantly.
 *
 *   - When configuring a PVG output, the associated voltage feedback channel
 *     will also be configured.
 *     If the difference between the measured output (U_feedback) and the configured output
 *     calculated by U_diff = output_value / 100 * U_BAT - U_feedback
 *     is greater than abs(+-9.5V), the output protection will be activated:\n
 *     - If U_diff > 9.5V, the output protection will set the output to 1500 (i.e. 15%) for 1 second. \n
 *     - If U_diff < -9.5V, the output protection will set the output to 8000 (i.e. 80%) for 1 second.
 *
 * \section pvg_examples PVG code examples
 *
 * Please refer to section \ref appl_structure for understanding where to place the initialization and task function calls.
 *
 *     PVG initialization examples:
 *     \code
 *     // Setup a PVG output with output value of 50% of the supply voltage
 *     IO_PVG_Init( IO_PVG_00
 *                , 5000 );        // output value is 5000 = 50%
 *
 *     // Enable all configured PVG outputs
 *     IO_POWER_Set(IO_INT_PVG_VOUT_0_ENABLE, IO_POWER_ON);
 *     \endcode
 *
 *     PVG task examples:
 *     \code
 *
 *     IO_PVG_SetOutput( IO_PVG_00
 *                     , 6200 );     // set duty cycle to 62% of the supply voltage
 *
 *     \endcode
 *
 **************************************************************************/

#ifndef _IO_PVG_H
#define _IO_PVG_H

/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/

#include "IO_Driver.h"


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
 * \brief Setup one PVG channel
 *
 * \param pvg_channel           PVG channel (\c #IO_PVG_00 .. \c #IO_PVG_05)
 *
 * \param output_value          Output value with which the PVG-channel will be
 *                              initialized in percent * 100 ( 1500..8500 ) of the supply voltage.
 *                              The configured value will be output after enabling the PVG-outputs
 *                              via IO_POWER_Set ( IO_INT_PVG_VOUT_x_ENABLE) until the first call of
 *                              #IO_PVG_SetOutput.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_GROUP_CONFLICT              configuring the output not allowed due to conflicts with
 *                                           other in/outputs in the same group
 * \retval #IO_E_INVALID_CHANNEL_ID          the channel id does not exist
 * \retval #IO_E_INVALID_PARAMETER           parameter is out of range
 * \retval #IO_E_CHANNEL_BUSY                the ADC input channel is
 *                                           currently used by another function
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      The common driver init function has not been called before
 * \retval #IO_E_TASK_NO_FREE_SLOTS          No more free slots to setup task function
 * \retval #IO_E_SW_INTERNAL                 Internal software error
 * \retval #IO_E_CH_CAPABILITY               The ADC capability of this channel
 *                                           has not been activated
 *
 * \remarks
 *    - The following channels form groups. They have to be configured in the same mode within a group.
 *        - \c #IO_PVG_00 .. \c #IO_PVG_02
 *        - \c #IO_PVG_03 .. \c #IO_PVG_05
 *    - Check the alternate functions of the pins used in each group. A pin can only be configured for one function
 *    at a time and it has to be the same function within the group - however mixing DO functionality together with
 *    PVG functionality in the same group is a valid configuration and vice versa.
 *    The alternate functions can be found at \ref IO_Pins.h
 *
 ***************************************************************************/
IO_ErrorType IO_PVG_Init ( IO_PIN pvg_channel
                         , ubyte2 output_value );

/***********************************************************************//**
 *
 * \brief Deinitializes one PVG output
 *
 * \param pvg_channel           PVG output (\c #IO_PVG_00 .. \c #IO_PVG_05)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 *
 * \remarks
 *    - The following channels form groups. A group always has to be configured as a whole.
 *        - \c #IO_PVG_00 .. \c #IO_PVG_02
 *        - \c #IO_PVG_03 .. \c #IO_PVG_05
 *
 ***************************************************************************/
IO_ErrorType IO_PVG_DeInit ( IO_PIN pvg_channel );

/***********************************************************************//**
 *
 * \brief Sets the output value of one PVG channel
 *
 * \param pvg_channel           PVG channel (\c #IO_PVG_00 .. \c #IO_PVG_05)
 *
 * \param output_value          Output value in percent * 100 (1500..8500)
 *                              of the supply voltage
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID          the channel id does not exist
 * \retval #IO_E_INVALID_PARAMETER           parameter is out of range
 * \retval #IO_E_CHANNEL_BUSY                the ADC input channel is
 *                                           currently used by another function
 * \retval #IO_E_PVG_SHORT_BATTERY           short to UBAT has been detected
 * \retval #IO_E_PVG_SHORT_CIRCUIT           short to GND has been detected
 * \retval #IO_E_PVG_OUTPUT_DISABLED         the PVG output is disabled
 * \retval #IO_E_PROT_ACTIVE                 Protection is active and output value was automatically
 *                                           set to a value to protect the outputs.
 * \retval #IO_E_PROT_REENABLE               The IO-Driver signals that the protection was disabled and
 *                                           the output functionality was restored.
 * \retval #IO_E_ADC_INVALID                 the ADC returned an invalid result
 * \retval #IO_E_CH_CAPABILITY               the chosen channel does not support the requested feature
 *
 ***************************************************************************/
IO_ErrorType IO_PVG_SetOutput ( IO_PIN pvg_channel
                              , ubyte2 output_value );

#endif
