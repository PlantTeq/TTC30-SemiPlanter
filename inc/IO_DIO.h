/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_DIO.h
 *
 * \brief IO Driver functions for Digital Input/Output.
 *
 * Contains all service functions for the digital in/outputs.\n
 *
 * \section dio_examples DIO Code Examples
 *
 * Please refer to section \ref appl_structure for understanding where to place the initialization and task function calls.
 *
 * DIO initialization examples:
 * \code
 * IO_DRIVER_DI_LIMITS limits = { 0, 3000, 3000, 32000 };
 *
 * IO_DI_Init( IO_DI_00, IO_DI_PU, &limits );               // configure digital input with pull-up
 *
 * IO_DO_Init( IO_DO_00, 2500 );                            // digital output with 2500mA overload configuration
 *
 * IO_POWER_Set( IO_INT_POWERSTAGE_ENABLE, IO_POWER_ON );   // enable high-side outputs
 * \endcode
 *
 * DIO task function examples:
 * \code
 * bool di_val_0;
 * ubyte2 do_voltage_fb;
 *
 * IO_DI_Get( IO_DI_00             // read value of digital input
 *          , &di_val_0 );
 *
 * IO_DO_Set( IO_DO_00             // set digital output value and get analog feedback
 *          , TRUE
 *          , &do_voltage_fb );
 *
 * \endcode
 *
 **************************************************************************/

#ifndef _IO_DIO_H
#define _IO_DIO_H

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
 * \name Pull up / down configuration
 * \anchor di_pupd_def
 *
 * Pull up/down resistor for the digital inputs
 */
/*@{*/
#define IO_DI_PU                0x01    /**< pull-up resistor */
#define IO_DI_PD                0x02    /**< pull-down resistor */
/*@}*/

/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/**********************************************************************//**
 *
 * \brief Voltage limits for digital inputs
 *
 * Contains the thresholds for valid low- and high-levels for digital inputs.\n
 * The range for the low-level is defined by the voltages \c low_thresh1 and \c low_thresh2, where
 * \c low_thresh1 is the lower limit for a low-level and \c low_thresh2 the upper limit.\n
 * The range for the high-level is defined by the voltages \c high_thresh1 and \c high_thresh2, where
 * \c high_thresh1 is the lower limit for a high-level and \c high_thresh2 the upper limit.\n
 * \n
 * The value of \c low_thresh1 must always be smaller than \c low_thresh2 and \c high_thresh1 must
 * always be smaller than \c high_thresh2.\n
 * \n
 * It is possible to configure a hysteresis by setting \c low_thresh2 bigger than \c high_thresh1.
 * In this mode an already detected logic level will be hold as long as the analog voltage varies
 * within the hysteresis band \c low_thresh2 - \c high_thresh1. Please see the following examples:
 *
 * Examples:
 * \code
 * // voltage limits without hysteresis
 * IO_DRIVER_DI_LIMITS limits1 = { 0, 2000, 3000, 5000 };
 * \endcode
 *
 * In the above example \c limits1 defines the range 0-2000mV as valid low-level and
 * 3000-5000mV as valid high-level. The voltage range between 2000 and 3000mV represents an
 * invalid area. In this case the error code \c #IO_E_DI_INVALID_VOLTAGE will be returned.
 *
 * \code
 * // voltage limits with hysteresis
 * IO_DRIVER_DI_LIMITS limits2 = { 0, 3000, 2000, 5000 };
 * \endcode
 *
 * In the above example \c limits1 defines the range 0-2000mV as valid low-level and
 * 3000-5000mV as valid high-level. The range 2000-3000mV forms a hysteresis. If the voltage
 * value rises or drops within this band (between 2000 and 3000mV) the former state of the channel
 * will be held.
 *
 */
typedef struct _io_driver_di_limits
{
    ubyte2 low_thresh1;  /* Defines the lower voltage limit of valid low signal (0mV ... 32780mV) (0mV..33333mV for TTC32 variants) */
    ubyte2 low_thresh2;  /* Defines the upper voltage limit of valid low signal (0mV ... 32780mV) (0mV..33333mV for TTC32 variants) */
    ubyte2 high_thresh1; /* Defines the lower voltage limit of valid high signal (0mV ... 32780mV) (0mV..33333mV for TTC32 variants) */
    ubyte2 high_thresh2; /* Defines the upper voltage limit of valid high signal (0mV ... 32780mV) (0mV..33333mV for TTC32 variants) */
} IO_DRIVER_DI_LIMITS;

/**************************************************************************
 *
 * F U N C T I O N S
 *
 **************************************************************************/

/**********************************************************************//**
 *
 * \brief Setup the Digital Inputs
 *
 * \param di_channel                Digital input:
 *                                      - \c #IO_DI_00..\c #IO_DI_07
 *                                      - \c #IO_DI_10..\c #IO_DI_31
 *
 * \param pupd                      Pull-up/down configuration, one of (only for \c #IO_DI_02..\c #IO_DI_07):
 *                                      - \c IO_DI_PU: configure with pull-up resistor
 *                                      - \c IO_DI_PD: configure with pull-down resistor
 *
 * \param limits                    voltage limits for low/high-levels.
 *                                  If \c NULL, default limits will be used.
 *                                  See #IO_DRIVER_DI_LIMITS for details.
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID  the channel id does not exist
 * \retval #IO_E_CHANNEL_BUSY        the digital output channel is
 *                                   currently used by another function
 * \retval #IO_E_INVALID_PARAMETER   parameter is out of range
 * \retval #IO_E_GROUP_CONFLICT      desired configuration not possible due
 *                                   to a group conflict.
 * \retval #IO_E_CH_CAPABILITY       The DI capability of this channel
 *                                   has not been activated
 * \retval #IO_E_DI_INVALID_LIMITS   the given voltage limits are not valid
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function \c IO_Driver_Init() has not been called
 *
 ***************************************************************************
 * \remarks
 *   - If the parameter \c limits is \c NULL, default values will be used:\n
 *      - low-level: 0..1500mV
 *      - high-level: 3500..32000mV
 *
 * \remarks
 *  -  The parameter \c pupd is only considered for digital inputs which allow the
 *     configuration of a pull-up/down resistor; i.e. #IO_DI_02..#IO_DI_07.
 *     For all other digital inputs this parameter is ignored.
 *
 * \remarks
 *   - All digital inputs are connected via multiplexers to the CPU. A changing
 *     value on the connector pin may only be available after some delay.
 *
 * \remarks
 *   - The following channels form groups. They have to be configured in the same mode within a group.
 *        - \c #IO_DI_00 .. \c #IO_DI_01
 *        - \c #IO_DI_02 .. \c #IO_DI_03
 *        - \c #IO_DI_04 .. \c #IO_DI_05
 *        - \c #IO_DI_06 .. \c #IO_DI_07
 *        - \c #IO_DI_10 .. \c #IO_DI_11
 *        - \c #IO_DI_12 .. \c #IO_DI_13
 *        - \c #IO_DI_14 .. \c #IO_DI_15
 *        - \c #IO_DI_16 .. \c #IO_DI_18
 *        - \c #IO_DI_19 .. \c #IO_DI_21
 *    - Check the alternate functions of the pins used in each group. A pin can only be configured for one function
 *    at a time and it has to be the same function within the group.The alternate functions can be found at \ref IO_Pins.h
 *
 ***************************************************************************/
IO_ErrorType IO_DI_Init( IO_PIN di_channel
                       , ubyte1 pupd
                       , const IO_DRIVER_DI_LIMITS * const limits);

/**********************************************************************//**
 *
 * \brief Setup the Digital Outputs
 *
 * \param do_channel    Digital Output:
 *                      - \c #IO_DO_00..\c #IO_DO_07 \ref pin_feat_30x_pwm_ocm_do "Digital High-Side Outputs with analog feedback"
 *                      - \c #IO_DO_10..\c #IO_DO_11 \ref pin_feat_30x_do_ls "Digital Low-Side with analog feedback"
 *                      - \c #IO_DO_20..\c #IO_DO_25 \ref pin_feat_30x_pwm_cm_do "Digital High-Side Outputs with analog and current feedback"
 *                      - \c #IO_DO_30..\c #IO_DO_35 \ref pin_feat_30x_pvg_do_pp "Push-Pull Digital Outputs"
 *
 * \param overload_limit  Configurable limit [mA] above which #IO_E_PROT_USER_OVERLOAD is reported.\n
 *                        Parameter is ignored for  \c #IO_DO_10..\c #IO_DO_11 and \c #IO_DO_30 .. \c #IO_DO_35
 *                             - overload_limit configured to 0: overload limit disabled; #IO_E_PROT_USER_OVERLOAD
 *                                  error will not be reported
 *                             - overload_limit configured to 1mA .. 2999mA: #IO_E_PROT_USER_OVERLOAD error will be reported via function IO_DO_Set()
 *                                  if the measured current is above defined overload_limit
 *                             - if measured current exceeds 3000mA other error codes than #IO_E_PROT_USER_OVERLOAD are reported
 *                                  (see error codes of function IO_DO_Set(): #IO_E_PROT_TEMP_OVERLOAD, #IO_E_PROT_ACTIVE and #IO_E_PROT_FATAL)
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID  the channel id does not exist
 * \retval #IO_E_CHANNEL_BUSY        the digital output channel is
 *                                   currently used by another function
 * \retval #IO_E_SW_INTERNAL         Internal software error
 * \retval #IO_E_CH_CAPABILITY       The DO capability of this channel
 *                                   has not been activated
 * \retval #IO_E_TASK_NO_FREE_SLOTS   No more free slots to setup task function
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function \c IO_Driver_Init() has not been called
 *
 ***************************************************************************
 *
 * \remarks
 *   For detection of open-load/short-circuit on the digital output channels
 *   \c #IO_DO_00..\c #IO_DO_07, \c #IO_DO_10..\c #IO_DO_11 and
 *   \c #IO_DO_20..\c #IO_DO_25 the associated ADC channel will also be configured.
 *
 * \remarks
 *   For detection of overload on the digital output channels
 *   \c #IO_DO_20..\c #IO_DO_25 the associated current feedback channel will also be
 *   configured.
 *
 * \remarks
 *   - The following channels form groups. They have to be configured in the same mode within a group.
 *        - \c #IO_DO_00, \c #IO_DO_20
 *        - \c #IO_DO_01, \c #IO_DO_21
 *        - \c #IO_DO_02, \c #IO_DO_22
 *        - \c #IO_DO_03, \c #IO_DO_23
 *        - \c #IO_DO_04, \c #IO_DO_24
 *        - \c #IO_DO_05, \c #IO_DO_25
 *
 *   - Check the alternate functions of the pins used in each group. A pin can only be configured for one function
 *   at a time and it has to be the same function within the group.The alternate functions can be found at \ref IO_Pins.h
 *
 ***************************************************************************/
IO_ErrorType IO_DO_Init( IO_PIN do_channel
                       , ubyte2 overload_limit);

/**********************************************************************//**
 *
 * \brief Deinitializes a DI channel
 *
 * \param di_channel                Digital input:
 *                                      - \c #IO_DI_00..\c #IO_DI_07
 *                                      - \c #IO_DI_10..\c #IO_DI_31
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                        everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID        the given core id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY       The DI capability of this channel
 *                                   has not been activated
 *
 * \remarks
 *   - The following channels form groups. A group always has to be configured as a whole.
 *        - \c #IO_DI_00 .. \c #IO_DI_01
 *        - \c #IO_DI_02 .. \c #IO_DI_03
 *        - \c #IO_DI_04 .. \c #IO_DI_05
 *        - \c #IO_DI_06 .. \c #IO_DI_07
 *        - \c #IO_DI_10 .. \c #IO_DI_11
 *        - \c #IO_DI_12 .. \c #IO_DI_13
 *        - \c #IO_DI_14 .. \c #IO_DI_15
 *        - \c #IO_DI_16 .. \c #IO_DI_18
 *        - \c #IO_DI_19 .. \c #IO_DI_21
 *
 ***************************************************************************/
IO_ErrorType IO_DI_DeInit( IO_PIN di_channel );

/**********************************************************************//**
 *
 * \brief Deinitializes a DO channel
 *
 * \param do_channel                Digital Output:
 *                                      - \c #IO_DO_00..\c #IO_DO_07
 *                                      - \c #IO_DO_10..\c #IO_DO_11
 *                                      - \c #IO_DO_20..\c #IO_DO_25
 *                                      - \c #IO_DO_30..\c #IO_DO_35
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given core id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY       The DO capability of this channel
 *                                   has not been activated
 * \remarks
 *   - The following channels form groups. A group always has to be configured as a whole.
 *        - \c #IO_DO_00, \c #IO_DO_20
 *        - \c #IO_DO_01, \c #IO_DO_21
 *        - \c #IO_DO_02, \c #IO_DO_22
 *        - \c #IO_DO_03, \c #IO_DO_23
 *        - \c #IO_DO_04, \c #IO_DO_24
 *        - \c #IO_DO_05, \c #IO_DO_25
 *
 ***************************************************************************/
IO_ErrorType IO_DO_DeInit( IO_PIN do_channel );

/**********************************************************************//**
 *
 * \brief Returns the state of a Digital Input
 *
 * \param di_channel                Digital input:
 *                                      - \c #IO_DI_00 .. \c #IO_DI_07
 *                                      - \c #IO_DI_10 .. \c #IO_DI_31
 * \param di_value                  Input value:
 *                                      - \c TRUE:  High Level
 *                                      - \c FALSE: Low Level
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the channel id does not exist
 * \retval #IO_E_CH_CAPABILITY           The given channel is not a digital input
 * \retval #IO_E_NULL_POINTER            null pointer passed as argument
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_ADC_INVALID             ADC measurement error
 * \retval #IO_E_DI_SHORT_CIRCUIT        short to ground was detected
 * \retval #IO_E_DI_SHORT_BATTERY        short to battery was detected
 * \retval #IO_E_DI_OPEN_LOAD            open-load situation was detected
 * \retval #IO_E_DI_INVALID_VOLTAGE      voltage measured not within configured
 *                                       voltage limits
 * \retval #IO_E_DI_OPEN_LOAD_OR_SHORT_CIRCUIT open-load or short to GND detected
 *                                       (not distinguishable)
 *
 ****************************************************************************
 *
 * \remarks
 *   The digital input is determined according to the following rules and order:
 *   -# If the measured voltage lies between low_thresh1 and low_thresh2 configured
 *      with IO_DI_Init, \c FALSE is reported as digital value. If the
 *      voltage lies between high_thresh1 and high_thresh2, \c TRUE is reported as
 *      digital value. In both cases, #IO_E_OK is returned as error-code.
 *   -# If a pull-up is configured on the input and the voltage lies between
 *      4.75V and 5.5V, the value in \c di_value is not valid and the function
 *      returns #IO_E_DI_OPEN_LOAD.
 *   -# If a pull-down or no pull-resistor is configured on the input and
 *      the voltage lies between  0V and 1.25V, the value in \c di_value
 *      is not valid and the function returns #IO_E_DI_OPEN_LOAD_OR_SHORT_CIRCUIT
 *   -# If the measured voltage lies between UBat and UBat - 1.25V, TRUE is
 *      stored in \c di_value and the function returns #IO_E_DI_SHORT_BATTERY.
 *   -# In all other cases, the value in \c di_value is not valid and the
 *      function returns #IO_E_DI_INVALID_VOLTAGE, as the measured voltage
 *      neither lies in the range defined by the user, nor in any range that allows
 *      diagnosis.
 *
 * \note
 *   The voltage levels defined by the user via the parameter \c limits of
 *   IO_DI_Init always have priority over diagnosis functionality.\n
 *   For example, if an input is configured with pull-up resistor and
 *   the voltage range 0-6V is defined as valid low-level, no open-load error
 *   will be returned by this function.
 *
 *
 ***************************************************************************/
IO_ErrorType IO_DI_Get( IO_PIN di_channel
                      , bool * const di_value );

/**********************************************************************//**
 *
 * \brief Sets the state of a Digital Output
 *
 * \param do_channel                Digital Output:
 *                                      - \c #IO_DO_00 .. \c #IO_DO_07
 *                                      - \c #IO_DO_10 .. \c #IO_DO_11
 *                                      - \c #IO_DO_20 .. \c #IO_DO_25
 *                                      - \c #IO_DO_30 .. \c #IO_DO_35
 * \param do_value                  Input value:
 *                                      - \c TRUE:  High Level
 *                                      - \c FALSE: Low Level
 * \if TTC32_LAGE
 * \param voltage_fb                ADC value in mV of the feedback signal
 *                                      - Range: 0..32780  (0mV..32780mV)
 *                                      - For the low-side outputs see the remarks bellow
 *
 * \else
 * \param voltage_fb                ADC value in mV of the feedback signal
 *                                      - Range: 0..32780  (0mV..32780mV)
 * \endif
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID          the channel id does not exist
 * \retval #IO_E_INVALID_DIAG_STATE          the instruction is not permitted in the current diagnostic state.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED      the given channel is not configured
 * \retval #IO_E_DO_SHORT_CIRCUIT            a short to GND has been detected
 * \retval #IO_E_DO_OPEN_LOAD                open load is detected on the output
 * \retval #IO_E_DO_SHORT_BATTERY            short to battery detected on the output
 * \retval #IO_E_DO_OPEN_LOAD_OR_SHORT_BATTERY open load or short battery is detected on the output
 * \retval #IO_E_DO_DIAG_TRANSIENT_OSC       low pass of DO outputs with analog
 *                                           feedback is being tuned in.
 * \retval #IO_E_DO_CHANNEL_STARTUP          The digital output is in its startup phase
 * \retval #IO_E_DO_OUTPUT_DISABLED          The digital outputs are disabled by the powerstage (see \ref IO_POWER.h for details)
 * \retval #IO_E_DO_OUTPUT_STARTUP_ERROR     The digital output could not be started
 * \retval #IO_E_PROT_USER_OVERLOAD          Output current is above the threshold configured on initialization.
 *                                           The output remains active.
 * \if TTC32_LAGE
 * \retval #IO_E_PROT_TEMP_OVERLOAD          Output current is between 3.0A and 4.0A (HS outputs) or 3.5A
 *                                           and 5.5A (LS outputs) and output will be switched off if the
 *                                           current does not decrease to or below 3.0A within the next 1
 *                                           second (HS outputs) or to or below 3.5A within the next 15
 *                                           seconds (LS outputs) (see \ref IO_Constants.h for details).
 * \retval #IO_E_PROT_ACTIVE                 Output is disabled, protection is active because
 *                                           of too high output current (over 4A for HS outputs or 5.5A for
 *                                           LS Outputs). The driver will try to re-enable the output again
 *                                           in 1 second (HS outputs) or 60 seconds (LS outputs)
 *                                           (see \ref IO_Constants.h for details).
 * \retval #IO_E_PROT_FATAL                  Output is disabled, protection is active because of too high
 *                                           output current (over 7A). The driver will try to re-enable the
 *                                           output again in 1 second (HS outputs with current measurement)
 *                                           or in 10 seconds (HS outputs with overcurrent protection)
 *                                           or in 60 seconds (LS outputs) (see \ref IO_Constants.h for details).
 * \else
 * \retval #IO_E_PROT_TEMP_OVERLOAD          Output current is between 3.0A and 4.0A (HS outputs) or 3.5A
 *                                           and 5.5A (LS outputs) and output will be switched off if the
 *                                           current does not decrease to or below 3.0A (HS outputs) or to or below
 *                                           3.5A (LS outputs) within the next 1 second
 *                                           (see \ref IO_Constants.h for details).
 * \retval #IO_E_PROT_ACTIVE                 Output is disabled, protection is active because
 *                                           of too high output current (over 4A for HS outputs or 5.5A for
 *                                           LS Outputs). The driver will try to re-enable the output again
 *                                           in 1 second (both HS and LS outputs)
 *                                           (see \ref IO_Constants.h for details).
 * \retval #IO_E_PROT_FATAL                  Output is disabled, protection is active because of too high
 *                                           output current (over 7A). The driver will try to re-enable the
 *                                           output again in 1 second (HS outputs with current measurement, LS Outputs)
 *                                           or in 10 seconds (HS outputs with overcurrent protection)
 *                                           (see \ref IO_Constants.h for details).
 * \endif
 * \retval #IO_E_PROT_REENABLE               The IO driver tries to re-enable the given output channel
 * \retval #IO_E_SW_OUTPROT_SM               Software malfunction (state machine error)
 * \retval #IO_E_ADC_INVALID                 the ADC driver reported an error
 * \retval #IO_E_CH_CAPABILITY               The DO capability of this channel
 *                                           has not been activated
 *
 ***************************************************************************
 *
 * \remarks
 *   - For the low-side outputs \c #IO_DO_10 .. \c #IO_DO_11 the measurement of
 *   the voltage feedback \c adc_value is only possibly in OFF-state.
 * \if TTC32_LAGE
 *   During ON-state the current feedback value is measured and set to \c voltage_fb
 *   pointer. The current measurement ranges are described in detail  (\ref IO_Constants.h,
 *   #IO_E_PROT_ACTIVE).
 * \else
 *  During ON-state the measured value will always be zero.
 * \endif
 *
 * \remarks
 *   - If the driver is initialized as safety, the low-side outputs \c #IO_DO_10 .. \c #IO_DO_11
 *   are allowed to be en- and disabled only while the driver is running in main state.
 *
 * \remarks
 *   - For detection of open-load/short-circuit on all digital output channels
 *   the associated ADC channel will be used.
 *
 * \remarks
 *   - In order to obtain a plausible value in voltage_fb, the IO_DO_Set()
 *   function has to be called a second time at least after 10 ms.
 *
 * \note
 *   - The high-side digital outputs \c #IO_DO_00 .. \c #IO_DO_05 and
 *   \c #IO_DO_20 .. \c #IO_DO_25 have to be enabled with #IO_POWER_Set.
 *   Otherwise the outputs remain disabled.
 *   - The push-pull digital outputs \c #IO_DO_30 .. \c #IO_DO_35 have to be
 *   enabled with #IO_POWER_Set as well, otherwise these outputs remain
 *   disabled.
 *
 * \attention
 *   - If the driver is initialized as non-safety, driving the low-side outputs
 *   \c #IO_DO_10 .. \c #IO_DO_11 immediately after an ECU power reset can trigger
 *   a fatal output protection (#IO_E_PROT_FATAL) due to an initial current peak.
 *
 ***************************************************************************/
IO_ErrorType IO_DO_Set( IO_PIN   do_channel
                      , bool     do_value
                      , ubyte2 * const voltage_fb );

/***********************************************************************//**
 *
 * \brief Returns the measured current of the given channel
 *
 * \param do_channel            Digital Output:
 *                              - \c #IO_DO_20..\c #IO_DO_25
 * \param current               Measured current
 *                              - Range: 0..7575 (0mA..7575mA)
 * \param fresh                 Fresh flag for current value
 *                              - \c #TRUE  value is valid
 *                              - \c #FALSE no new value available
 *
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                         everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED     the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID         the given channel id does not exist
 * \retval #IO_E_ADC_INVALID                an ADC error occured
 * \retval #IO_E_PWM_CURRENT_INACCURATE     Current measurement has reduced accuracy
 * \retval #IO_E_NULL_POINTER               A NULL pointer has been passed to the function
 * \retval #IO_E_CH_CAPABILITY              The DO capability of this channel
 *                                          has not been activated
 *
 ***************************************************************************/
IO_ErrorType IO_DO_GetCur( IO_PIN         do_channel
                         , ubyte2 * const current
                         , bool   * const fresh );

#endif /* _IO_DIO_H */
