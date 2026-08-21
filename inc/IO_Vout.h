/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_Vout.h
 *
 * \brief IO Driver functions for voltage outputs.
 *
 *  Contains all service functions for the voltage outputs.
 *  Up to 6 channels can be configured: \c #IO_VOUT_00 .. \c #IO_VOUT_05
 *
 *  - The pins for PVG- and Voltage-Outputs are organized in two groups of three
 *    - Group 1: \c #IO_PVG_00/\c #IO_VOUT_00, \c #IO_PVG_01/\c #IO_VOUT_01 and
 *               \c #IO_PVG_02/\c #IO_VOUT_02
 *
 *    - Group 2: \c #IO_PVG_03/\c #IO_VOUT_03, \c #IO_PVG_04/\c #IO_VOUT_04 and
 *               \c #IO_PVG_05/\c #IO_VOUT_05
 *
 *    Whenever a pin within a group is configured either as PVG or voltage output, all
 *    other pins within the same group must be either remain un-configured, or be configured
 *    with the same type. For example, if \c #IO_PIN_J2 (\c #IO_PVG_01/\c #IO_VOUT_01)
 *    is configured as voltage output, pins \c #IO_PIN_K2 (\c #IO_PVG_00/\c #IO_VOUT_00)
 *    and \c #IO_PIN_H2 (\c #IO_PVG_02/\c #IO_VOUT_02) must either remain unconfigured
 *    or also used as voltage outputs. Initializing it as any other
 *    output type will result in a \c #IO_E_GROUP_CONFLICT error.
 *
 *  - The outputs will only be activated after enabling them via\n
 *    \code
 *    IO_POWER_Set(IO_INT_PVG_VOUT_0_ENABLE, IO_POWER_ON);
 *    IO_POWER_Set(IO_INT_PVG_VOUT_1_ENABLE, IO_POWER_ON);
 *    \endcode
 *    After activating the outputs, it is not possible to initialize any further PVG/VOut
 *    channels.
 *  - Initialization/usage order:
 *    -# Initialize ALL needed voltage output channels.
 *    -# Call IO_POWER_Set(IO_INT_PVG_VOUT_x_ENABLE, IO_POWER_ON);
 *    -# Set desired output value with \c #IO_VOut_SetVoltage.
 *
 *  - After enabling the outputs by calling #IO_POWER_Set, all configured VOut channels
 *    will output the desired value, while all other pins within this group will output
 *    0V constantly.
 *  - When configuring a voltage output, the associated voltage feedback channel
 *    will also be configured.
 *
 *  - On initialization of a voltage output, a configuration for the PID controller can
 *    be specified via the parameter \c pid_config.\n
 *    If this parameter is \c NULL, following default values will be used:
 *    \code
 *     IO_PID_CONFIG pid_config = {
 *       .Kff = 1000,
 *       .Kp = 200,
 *       .Ki = 2500,
 *       .Kd = 300,
 *       .max_limit = 65535,
 *       .min_limit = 0
 *     };
 *    \endcode
 *    The PID configuration heavily depends on the load attached to the output and
 *    most likely need to be modified.
 *  - The cycle time of the PID controller used for voltage outputs is 3ms.
 *
 *  \section vout_examples VOut code examples
 *
 *  Please refer to section \ref appl_structure for understanding where to place the initialization and task function calls.
 *
 *      VOut initialization examples:
 *      \code
 *
 *      // Setup a voltage output with the default PID-parameters
 *      IO_VOut_Init( IO_VOUT_00
 *                  , NULL          // use default PID-parameters
 *                  , NULL );       // use no PID handler
 *
 *      // Enable all configured Voltage/PVG outputs
 *      IO_POWER_Set(IO_INT_PVG_VOUT_0_ENABLE, IO_POWER_ON);
 *      \endcode
 *
 *      VOut task examples:
 *      \code
 *
 *      ubyte2 voltage_fb;
 *
 *      IO_VOut_SetVoltage( IO_VOUT_00
 *                        , 6200           // set output voltage to 6200mV
 *                        , &voltage_fb);  // variable to store voltage feedback
 *
 *      \endcode
 *
 **************************************************************************/
#ifndef _IO_VOUT_H
#define _IO_VOUT_H

/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/

#include "IO_Driver.h"
#include "IO_PID.h"

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
 * \brief Setup one voltage output channel
 *
 * \param vout_channel          voltage output channel (\c #IO_VOUT_00 .. \c #IO_VOUT_05)
 *
 * \param pid_config            Pointer to the data structure containing a
 *                              PID configuration (optional).
 *                              If NULL, default values will be used.
 *
 * \param pid_handle            PID handle of the used PID controller (optional).\n
 *                              Can be used to call PID functions.
 *                              If NULL, no handle will be returned.
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_GROUP_CONFLICT              configuring the output not allowed due to conflicts with
 *                                           other in/outputs in the same group
 * \retval #IO_E_PID_NO_FREE_HANDLES         No PID controller could be initialized
 * \retval #IO_E_INVALID_CHANNEL_ID          the channel id does not exist
 * \retval #IO_E_CHANNEL_BUSY                the ADC input channel is
 *                                           currently used by another function
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      The common driver init function has not been called before
 * \retval #IO_E_TASK_NO_FREE_SLOTS          No more free slots to setup task function
 * \retval #IO_E_SW_INTERNAL                 Internal software error
 * \retval #IO_E_CH_CAPABILITY               The ADC capability of this channel
 *                                           has not been activated
 *
 *
 * \remarks
 * The associated voltage feedback channel will also be configured for diagnosis.
 *
 * \remarks
 * The associated PID controller will be initialized and activated.
 *
 * \remarks
 * When NULL is passed as \c pid_config, following default configuration will be used:
 * \code
 *  IO_PID_CONFIG pid_config = {
 *    .Kff = 1000,
 *    .Kp = 200,
 *    .Ki = 2500,
 *    .Kd = 300,
 *    .max_limit = 65535,
 *    .min_limit = 0
 *  };
 * \endcode
 *
 * \remarks
 * The cycle time of the PID controller is 3ms and not configurable.
 *
 * \remarks
 * The values \c min_limit and \c max_limit of the parameter \c pid_config
 * are ignored by the PID controller for the voltage outputs.
 *
 * \remarks
 *    - The following channels form groups. A group always has to be configured as a whole.
 *        - \c #IO_VOUT_00 .. \c #IO_VOUT_02
 *        - \c #IO_VOUT_03 .. \c #IO_VOUT_05
 *
 *    - Check the alternate functions of the pins used in each group. A pin can only be configured for one function
 *    at a time and it has to be the same function within the group - however mixing DO functionality together with
 *    VOUT functionality in the same group is a valid configuration and vice versa.
 *    The alternate functions can be found at \ref IO_Pins.h
 *
 ***************************************************************************/
IO_ErrorType IO_VOut_Init ( IO_PIN vout_channel
                          , const IO_PID_CONFIG * const pid_config
                          , ubyte1 * const pid_handle );

 /***********************************************************************//**
  *
  * \brief Deinitializes one voltage output channel
  *
  * \param vout_channel           voltage output channel (\c #IO_VOUT_00 .. \c #IO_VOUT_05)
  *
  * \return #IO_ErrorType
  * \retval #IO_E_OK                      everything fine
  * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
  * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
  *
  * \remarks
  *    - The following channels form groups. A group always has to be configured as a whole.
  *        - \c #IO_VOUT_00 .. \c #IO_VOUT_02
  *        - \c #IO_VOUT_03 .. \c #IO_VOUT_05
  *
  ***************************************************************************/
IO_ErrorType IO_VOut_DeInit ( IO_PIN vout_channel );

/***********************************************************************//**
 *
 * \brief Sets the output voltage of one VOut channel
 *
 * \param vout_channel          voltage output channel (\c #IO_VOUT_00 .. \c #IO_VOUT_05)
 *  
 * \param output_voltage        Output voltage in mV (0..32000)
 *
 * \param voltage_fb            Voltage feedback in mV (optional)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_INVALID_PARAMETER           parameter is out of range
 * \retval #IO_E_INVALID_CHANNEL_ID          the channel id does not exist
 * \retval #IO_E_CH_CAPABILITY               the given channel does not support the requested feature
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED      the given channel is not configured
 * \retval #IO_E_CHANNEL_BUSY                the given channel is configured for another purpose
 * \retval #IO_E_VOUT_OUTPUT_DISABLED        the voltage output is disabled
 * \retval #IO_E_VOUT_SHORT_BATTERY          short to UBAT has been detected
 * \retval #IO_E_VOUT_SHORT_CIRCUIT          short to GND has been detected
 * \retval #IO_E_VOUT_PRECISION              the desired output voltage was not reached
 *                                           within the settling time
 * \retval #IO_E_PROT_ACTIVE                 Protection is active and output value was automatically
 *                                           set to a value to protect the outputs
 * \retval #IO_E_PROT_REENABLE               The IO-Driver signals that the protection was disabled and
 *                                           the output functionality was restored
 * \retval #IO_E_ADC_INVALID                 the received ADC value is corrupted
 *
 * \remarks
 * The output voltage is dependent of the supply voltage. It is not possible
 * to set an output voltage higher than the supply voltage.
 *
 * \remarks
 * An output voltage level close to the supply voltage can cause an
 * #IO_E_VOUT_SHORT_BATTERY error.
 *
 ***************************************************************************/
IO_ErrorType IO_VOut_SetVoltage ( IO_PIN vout_channel
                                , ubyte2 output_voltage
                                , ubyte2 * const voltage_fb);

#endif
