/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_POWER.h
 *
 * \brief IO Driver functions for Power control
 *
 * The Power control functions allow enable/disable
 * the power-stages, the PVG/Voltage-outputs
 * as well as switching off the whole unit.
 *
 * \section power_examples Power code examples
 *
 * Please refer to section \ref appl_structure for understanding where to place the initialization and task function calls.
 *
 * Power initialization example: \n
 * The Power driver does not need an explicit initialization.
 * The Power driver is initialized by IO_Driver_Init().
 *
 * Power task example:
 * \code
 *
 * // switch on high-side power stages
 * IO_POWER_Set (IO_INT_POWERSTAGE_ENABLE, IO_POWER_ON);
 *
 *
 * // send ECU to sleep mode (switch off K15)
 * IO_POWER_Set (IO_K15, IO_POWER_OFF);
 * \endcode
 *
 **************************************************************************/
#ifndef _IO_POWER_H
#define _IO_POWER_H

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

/**
 * \name Power values
 * \anchor power_val_def
 *
 * Selects power configuration.
 */
/*@{*/
#define IO_POWER_OFF      0     /**< switch off - deactivates the respective power function */
#define IO_POWER_ON       1     /**< switch on - activates the respective power function */
/*@}*/

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
/**********************************************************************//**
 *
 * \brief Initialization of the power module driver
 *
 *   Initialization of power module driver.
 *      - Initializes internal data structure
 *      - disables powerstage enable signal
 *      - disables PVG/VOut enable signal
 *
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CHANNEL_BUSY           the module is already initialized
 * \retval #IO_E_TASK_NO_FREE_SLOTS     No more free slots to setup task function
 *
 **************************************************************************
 *
 * \remarks
 *   Module is initialized only once.
 *   To re-initialize the module, the function IO_POWER_DeInit()
 *   needs to be called.
 *
 **************************************************************************/
 IO_ErrorType IO_POWER_Init(void);

/**********************************************************************//**
 *
 * \brief Deinitializes the power module driver
 *
 * Deinitializes the module.
 * Allows re-initialization by \c IO_POWER_Init()
 *
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED module has not been initialized
 *
 **************************************************************************
 *
 * \remarks
 *   This function disables all powerstage-enable signals (PVG/Vout, PWM/DO)
 *   and switches off the power outputs permanently. To switch the outputs
 *   to a operational state again, the outputs have to be re-initialized
 *   (together with the POWER driver)
 *
 * \remarks
 *   Re-initializing the POWER driver is only possible on non-safety ECUs
 *   or if the IO-Driver was configured not safety-critical.
 *
 **************************************************************************/
 IO_ErrorType IO_POWER_DeInit(void);

 /**********************************************************************//**
  *
  * \brief Sets a certain mode of a POWER feature
  *
  * \param   pin     pin for which the mode shall be set, one of:
  *                      - \c #IO_INT_POWERSTAGE_ENABLE
  *                      - \c #IO_INT_PVG_VOUT_0_ENABLE
  *                      - \c #IO_INT_PVG_VOUT_1_ENABLE
  *                      - \c #IO_SAFETY_SWITCH_0
  *                      - \c #IO_SAFETY_SWITCH_1
  *                      - \c #IO_K15 (for power down)
  * \param   mode    sets a certain mode, one of:
  *                      - \c #IO_POWER_ON
  *                      - \c #IO_POWER_OFF
  *
  * \return #IO_ErrorType
  * \retval #IO_E_OK                            everything fine
  * \retval #IO_E_BUSY                          driver is in startup phase
  * \retval #IO_E_INVALID_PARAMETER             an invalid parameter has been passed
  * \retval #IO_E_INVALID_DIAG_STATE            the instruction is not permitted in the current diagnostic state.
  * \retval #IO_E_CHANNEL_NOT_CONFIGURED        module has not been initialized
  * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG    driver is initialized as non-safety
  * \retval #IO_E_DRIVER_NOT_INITIALIZED        driver is not initialized
  * \retval #IO_E_INVALID_CHANNEL_ID            an invalid channel has been chosen
  * \retval #IO_E_GROUP_CONFLICT                a pin of this group has been initialized in a different mode
  * \retval #IO_E_SW_INTERNAL                   trying to set a PVG/Vout group which is not initialized
  * \retval #IO_E_NO_SAFETY_SWITCH_CONFIGURED   the safety switch is not used or was not configured
  *
  **************************************************************************
  *
  * \remarks
  *   - \c #IO_INT_POWERSTAGE_ENABLE and IO_INT_PVG_VOUT_x_ENABLE are internal pins.
  *   - \c #IO_INT_POWERSTAGE_ENABLE controls the internal powerstage enable
  *          signal. Without enabling this signal all high-side power outputs remain
  *          low (switched off).
  *   - \c #IO_INT_PVG_VOUT_0_ENABLE and \c #IO_INT_PVG_VOUT_1_ENABLE
  *          controls the internal enable signal for each group
  *          of the PVG/Voltage-outputs.
  *          Without switching this signal to ON, the PVG/Voltage-outputs
  *          will remain deactivated.
  *   - \c #IO_SAFETY_SWITCH_0 and \c #IO_SAFETY_SWITCH_1 allow to en- and disable both
  *          safety switches separately. To be able to switch the safety switches the driver
  *          must be initialized as safety and running in main state.
  *
  * \remarks
  *   If the high side outputs are used as safety critical, turning off the
  *   powerstage by calling \c IO_POWER_Set( IO_INT_POWERSTAGE_ENABLE, IO_POWER_OFF )
  *   will activate the safe state and return an error code \c #DIAG_E_PWM_PERIOD_MISMATCH.
  *
  * \note
  * Both \c #IO_INT_POWERSTAGE_ENABLE and
  * \c #IO_INT_PVG_VOUT_0_ENABLE/#IO_INT_PVG_VOUT_1_ENABLE
  * are "software-switches", i.e. they are pure software functionality
  * without the use of any hardware-parts.
  *
  **************************************************************************/
 IO_ErrorType IO_POWER_Set(IO_PIN pin, ubyte1 mode);


 /**********************************************************************//**
  *
  * \brief Returns the current state of a POWER feature
  *
  * Returns the state of power stage enable, PVG/VOut enable or K15
  *
  *
  * \param   pin     pin for which the mode shall be set, one of:
  *                      - \c #IO_INT_POWERSTAGE_ENABLE
  *                      - \c #IO_INT_PVG_VOUT_0_ENABLE
  *                      - \c #IO_INT_PVG_VOUT_1_ENABLE
  *                      - \c #IO_SAFETY_SWITCH_0
  *                      - \c #IO_SAFETY_SWITCH_1
  *                      - \c #IO_K15
  * \param   state   pointer to ubyte1, returns the state of the selected feature, one of:
  *                      - \c #IO_POWER_ON
  *                      - \c #IO_POWER_OFF
  *
  * \return #IO_ErrorType
  * \retval #IO_E_OK                            everything fine
  * \retval #IO_E_INVALID_CHANNEL_ID            an invalid channel has been chosen
  * \retval #IO_E_CHANNEL_NOT_CONFIGURED        module has not been initialized
  * \retval #IO_E_NULL_POINTER                  a NULL pointer has been passed
  * \retval #IO_E_ADC_INVALID                   the ADC value of the KL15 sampling is invalid
  * \retval #IO_E_NO_SAFETY_SWITCH_CONFIGURED   the safety switch is not used or was not configured
  *
  **************************************************************************
  *
  * \remarks
  *   - \c IO_INT_POWERSTAGE_ENABLE and \c IO_INT_PVG_VOUT_ENABLE are internal pins.
  *   - \c IO_INT_POWERSTAGE_ENABLE controls the internal powerstage enable
  *          signal. Without enabling this signal all high-side power outputs remain
  *          low (switched off).
  *   - \c IO_INT_PVG_VOUT_ENABLE controls the internal enable signal for
  *          the PVG/Voltage-outputs.
  *          Without switching this signal to ON, the PVG/Voltage-outputs
  *          will remain deactivated.
  *
  * \note
  * Both \c #IO_INT_POWERSTAGE_ENABLE and
  * \c #IO_INT_PVG_VOUT_0_ENABLE/#IO_INT_PVG_VOUT_1_ENABLE are
  * "software-switches", i.e. they are pure software functionality
  * without the use of any hardware-parts.
  *
  **************************************************************************/
 IO_ErrorType IO_POWER_Get(IO_PIN pin, ubyte1 * const state);

 /**********************************************************************//**
   *
   * \brief Sets the threshold in mV below which a low-level is detected
   *        on the K15 pin.
   *
   * \param  threshold                voltage-threshold below which
   *                                  \c #IO_POWER_OFF shall be reported
   *                                  for K15.
   *                                  Range: = 0 .. 2500 (0 .. 2500mV) \n
   *                                  Default: 2500mV
   * \return IO_ErrorType
   * \retval #IO_E_OK                        everything fine
   * \retval #IO_E_INVALID_PARAMETER         an invalid parameter has been passed
   * \retval #IO_E_CHANNEL_NOT_CONFIGURED    module has not been initialized
   *
   **************************************************************************/
 IO_ErrorType IO_POWER_SetK15Threshold( ubyte2 threshold );


#endif /* _IO_POWER_H */
