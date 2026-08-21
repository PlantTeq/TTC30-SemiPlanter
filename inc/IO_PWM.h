/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_PWM.h
 *
 * \brief IO Driver functions for PWM channels.
 *
 * Contains all service functions for the PWM (Pulse Width Modulation).
 * Up to 8 channels can be configured:
 * - \c #IO_PWM_00 .. \c #IO_PWM_05: High-side PWM outputs with current-measurement
 * - \c #IO_PWM_10 .. \c #IO_PWM_11: High-side PWM outputs with overcurrent-monitoring.
 * - Not all PWM channels can have their own frequency time base.
 *   - \c #IO_PWM_00, \c #IO_PWM_01, \c #IO_PWM_10 and \c #IO_PWM_11 have their own
 *     frequency time base (variable frequency)
 *   - \c #IO_PWM_02, \c #IO_PWM_03, \c #IO_PWM_04 and \c #IO_PWM_05 share one
 *     frequency time base. It's only allowed to configure these outputs
 *     with the same PWM frequency.
 * - The associated precise current-measurement will be configured for the channels
 *   \c #IO_PWM_00 .. \c #IO_PWM_05. These PWM outputs will be switched off if the
 *   the continuous current is above 3.0A after a specific amount of time (1s)
 *   and switched off immediately if the current exceeds 4.0A. The step function
 *   for retrieving the current is \c IO_PWM_GetCur().
 * - The associated overcurrent-monitoring will be configured for the channels
 *   \c #IO_PWM_10 .. \c #IO_PWM_11. These PWM outputs will be switched off if the
 *   the continuous current is above 3.0A after a specific amount of time (1s).
 *   The outputs allow overcurrent for 1 second to allow switching of light-bulbs
 *   with high inrush current.
 *
 * - Additionally, current-controlled PWM outputs can be configured with
 *   #IO_PWM_CurrentInit. These outputs are controlled by a PID controller.
 *
 *  \section pwm_examples PWM code examples
 *
 *  Please refer to section \ref appl_structure for understanding where to place the initialization and task function calls.
 *
 *  PWM initialization examples:
 *  \code
 *  // Setup a PWM output with precise current-measurement
 *  IO_PWM_Init( IO_PWM_00
 *             , 100            // frequency is 100 Hz
 *             , TRUE           // positive polarity
 *             , TRUE           // diag margin
 *             , 1500           // signal overload above 1.5A
 *             , NULL );        // safety configuration; currently not supported
 *
 * IO_POWER_Set( IO_INT_POWERSTAGE_ENABLE, IO_POWER_ON );   // enable high-side outputs
 *
 *  \endcode
 *
 *  PWM task examples:
 *  \code
 *  ubyte2 curr;
 *  bool   new;
 *
 *  IO_PWM_SetDuty( IO_PWM_00
 *                , 0x8000      // set duty cycle to 50%
 *                , NULL );     // duty-cycle feedback is ignored
 *
 *  IO_PWM_GetCur( IO_PWM_00    // read current value of PWM output
 *               , &curr        // variable to store current
 *               , &new );      // variable to store information if a current value is available
 *
 *  \endcode
 *
 *  \section pwm_cc_examples Current-controlled PWM code examples
 *
 *  Current-controlled PWM initialization examples:
 *  \code
 *
 *  // PID controller configuration
 *  IO_PID_CONFIG pid_cfg =
 *  {
 *          .Kd = 500U,             // Differnetial gain scaled by 10000 => gain of 0.05
 *          .Kff = 10000U,          // Feed-forwared gain scaled by 1000 => gain of 10
 *          .Ki = 15000U,           // Integral gain scaled by 10000 => gain of 15
 *          .Kp = 500U,             // Proportional gain sclaed by 1000 => agin of 0.05
 *          .max_limit = 65535U,    // Maximal allowed duty cycle is 100% (2^16)
 *          .min_limit = 0U,        // Minimal allowed duty cycle is 0% (0)
 *  };
 *
 *
 *  // Setup a current-controlled PWM output
 *  IO_PWM_CurrentInit( IO_PWM_00
 *                    , 100            // frequency is 100 Hz
 *                    , &pid_cfg       // use custom PID configuration
 *                    , 10             // use 10ms as cycle time for the PID controller
 *                    , NULL           // Handle for PID controller not needed => pass NULL
 *                    , NULL );        // safety configuration; currently not supported
 *
 *  IO_POWER_Set( IO_INT_POWERSTAGE_ENABLE, IO_POWER_ON );   // enable high-side outputs
 *
 *  \endcode
 *
 *  PWM task examples:
 *  \code
 *  ubyte2 curr;
 *  bool   new;
 *
 *  IO_PWM_SetCur ( IO_PWM_00
 *                , 500         // set current to 500mA
 *                , NULL );     // duty-cycle feedback is ignored
 *
 *  IO_PWM_GetCur( IO_PWM_00    // read current value of PWM output
 *               , &curr        // variable to store current
 *               , &new );      // variable to store information if a current value is available
 *
 *
 *  \endcode
 *
  **************************************************************************/

#ifndef _IO_PWM_H
#define _IO_PWM_H

/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/

#include "IO_Driver.h"
#include "IO_RTC.h"
#include "IO_PID.h"


/**************************************************************************
 *
 * D E F I N I T I O N S
 *
 **************************************************************************/

/**
 * \name Size of current queue
 * \anchor pwm_curr_q_def
 *
 * Size of the queue holding the measured values of the equidistant
 * current measurement
 */
/*@{*/
#define IO_PWM_CURRENT_QUEUE_MAX        6U    /**< maximum number of items in the current queue */
/*@}*/


/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/**
 *
 * \brief PWM current measurement queue
 * \anchor pwm_curr_q_struct
 *
 *  Stores results of the equidistant current measurement.
 *
 *  The queue holds all current measurement since the last retrieval
 *  via the step function \c IO_PWM_GetCur().
 *
 */
typedef struct _io_pwm_current_queue
{
    ubyte1 count;                            /**< Number of results stored in the queue */
    bool   overrun;                          /**< Signal queue overrun. TRUE means queue
                                                  is full and older measurement results may have
                                                  been dropped. FALSE means queue is not overrun. */
    ubyte2 values[IO_PWM_CURRENT_QUEUE_MAX]; /**< Buffer holding the measurement values values */
}IO_PWM_CURRENT_QUEUE;

/**
 *
 * \brief Safety configuration for the PWM outputs
 *
 *  Stores all relevant safety configuration parameters for the PWM outputs.
 *
 */
typedef struct _io_pwm_safety_conf
{
    IO_PIN safety_switch;           /**< Connected safety switch (\c #IO_SAFETY_SWITCH_0 or
                                         \c #IO_SAFETY_SWITCH_1, or alternatively
                                         \c #IO_SAFETY_SWITCH_NONE if neither safety switch is
                                         connected). */
    ubyte2 margin_lower_lim;        /**< Limit in microseconds for the lower minimum pulse.
                                         This limit can be adjusted within 50us to 100us. */
    ubyte2 margin_upper_lim;        /**< Limit in microseconds for the upper minimum pulse pause.
                                         This limit can be adjusted within 50us to 150us */
    ubyte2 duty_cycle_tolerance;    /**< Tolerance in microseconds for the set duty cycle.
                                         This limit can be adjusted within 100us to 200us */
}IO_PWM_SAFETY_CONF;

/**
 *
 * \brief Safety configuration for the PWM outputs
 *
 *  Stores all relevant safety configuration parameters for the PWM outputs.
 *
 * \attention For the current check to work properly it is necessary that the
 *            application does not change the set-point of the PID controller
 *            before the dead time elapses! for example if a dead time of 10ms
 *            is set, the application shall change the set-point for the current
 *            value less frequently than 10ms!
 * \attention To disable the current check, set the value of the parameters
 *            current_tolerance to 65535 and dead_time to 4294967295.
 *
 */
typedef struct _io_pwm_current_safety_conf
{
    ubyte2             current_tolerance;   /**< Tolerance of the measured electric current in Milli-Ampere.
                                                 Allowed values: 50mA .. 500mA */
    ubyte4             dead_time;           /**< Time until current should have reached its target value in Microseconds.
                                                 Allowed values: 5000us ..10000000us */
    IO_PWM_SAFETY_CONF pwm_safety_conf;     /**< Safety configuration for PWM signal */
}IO_PWM_CURRENT_SAFETY_CONF;

/**************************************************************************
 *
 * F U N C T I O N S
 *
 **************************************************************************/

/***********************************************************************//**
 *
 * \brief Setup single PWM output
 *
 * \param pwm_channel        PWM channel (\c #IO_PWM_00 .. \c #IO_PWM_05 or \c #IO_PWM_10 .. \c #IO_PWM_11)
 * \param frequency          PWM frequency (15Hz .. 1000Hz, only frequencies with a
 *                                          period of an integral multiple of 1ms
 *                                          are possible)
 * \param polarity           Polarity of output signal
 *                              - \c FALSE: Low output signal is variable
 *                              - \c TRUE:  High output signal is variable
 * \param diag_margin        Indicate if a margin should be applied or not.
 *                             - \c TRUE:  margin is on
 *                             - \c FALSE: no margin will be applied
 * \param overload_limit     Limit in mA above which a #IO_E_PROT_USER_OVERLOAD error will be reported
 *                             - overload_limit configured to 0: overload limit disabled; #IO_E_PROT_USER_OVERLOAD
 *                                  error will not be reported
 *                             - overload_limit configured to 1mA .. 2999mA: #IO_E_PROT_USER_OVERLOAD error will be reported via function IO_PWM_SetDuty() or IO_PWM_SetCur()
 *                                  if the measured current is above defined overload_limit
 *                             - if measured current exceeds 3000mA other error codes than #IO_E_PROT_USER_OVERLOAD are reported
 *                                  (see error codes of function IO_PWM_SetDuty() or IO_PWM_SetCur(): #IO_E_PROT_TEMP_OVERLOAD, #IO_E_PROT_ACTIVE and #IO_E_PROT_FATAL)
 *
 *
 * \param safety_conf        Safety configuration.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_CHANNEL_BUSY                the PWM output channel or the timer input channel is
 *                                           currently used by another function
 * \retval #IO_E_INVALID_CHANNEL_ID          the PWM output channel or the timer input channel
 *                                           id don't exist
 * \retval #IO_E_INVALID_PARAMETER           a given parameter is out of range
 * \retval #IO_E_CH_CAPABILITY               The PWM capability of this channel
 *                                           has not been activated.
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      The common driver init function has not been called before
 * \retval #IO_E_SW_INTERNAL                 Internal SW error when handling configuration tables
 *                                           (index out of range)
 * \retval #IO_E_GROUP_CONFLICT              configuring the output not allowed due to conflicts with
 *                                           other in/outputs in the same group
 * \retval #IO_E_INVALID_SAFETY_CONFIG       one or more parameters of the safety configuration
 *                                           are invalid (out of range)
 * \retval #IO_E_SAFETY_NOT_SUPPORTED        the given channel does not support safety features
 * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG  the driver has not been initialized as safety device -
 *                                           therefore the safety feature is not available for this channel
 * \retval IO_E_TASK_NO_FREE_SLOTS           No more free slots to setup task function
 *
 ***********************************************************************//**
 *
 * \remarks
 *   - For the equidistant current measurement to work properly only frequencies that
 *   have a period which is an integral multiple of 1ms are allowed. Periods which are not a
 *   integral multiple of 1ms will be rounded down to the next lower integral multiple of 1ms
 *   which means that the frequency will be calculated with the next lower integral period. For
 *   example a wanted frequency of 180Hz means 5.55ms. The next lower integral value is 5ms
 *   therefore the resulting frequency is 200Hz in this case. Possible frequencies are:
 *
 * \verbatim[table]
 * ============== ==============================
 *   Available frequencies:
 * =============================================
 *   Period(ms)     Frequency(Hz)
 * -------------- ------------------------------
 *     \c 1         \c 1000
 * -------------- ------------------------------
 *     \c 2         \c 500
 * -------------- ------------------------------
 *     \c 3         \c 333 (\c 333.333333333333)
 * -------------- ------------------------------
 *     \c 4         \c 250
 * -------------- ------------------------------
 *     \c 5         \c 200
 * -------------- ------------------------------
 *     \c 6         \c 166 (\c 166.666666666667)
 * -------------- ------------------------------
 *     \c 7         \c 142 (\c 142.857142857143)
 * -------------- ------------------------------
 *     \c 8         \c 125
 * -------------- ------------------------------
 *     \c 9         \c 111 (\c 111.111111111111)
 * -------------- ------------------------------
 *    \c 10         \c 100
 * -------------- ------------------------------
 *    \c 11         \c 90 (\c 90.9090909090909)
 * -------------- ------------------------------
 *    \c 12         \c 83 (\c 83.3333333333333)
 * -------------- ------------------------------
 *    \c 13         \c 76 (\c 76.9230769230769)
 * -------------- ------------------------------
 *    \c 14         \c 71 (\c 71.4285714285714)
 * -------------- ------------------------------
 *    \c 15         \c 66 (\c 66.6666666666667)
 * -------------- ------------------------------
 *    \c 16         \c 62 (\c 62.5)
 * -------------- ------------------------------
 *    \c 17         \c 58 (\c 58.8235294117647)
 * -------------- ------------------------------
 *    \c 18         \c 55 (\c 55.5555555555556)
 * -------------- ------------------------------
 *    \c 19         \c 52 (\c 52.6315789473684)
 * -------------- ------------------------------
 *    \c 20         \c 50
 * -------------- ------------------------------
 *    \c 21         \c 47 (\c 47.61904762)
 * -------------- ------------------------------
 *    \c 22         \c 45 (\c 45.45454545)
 * -------------- ------------------------------
 *    \c 23         \c 43 (\c 43.47826087)
 * -------------- ------------------------------
 *    \c 24         \c 41 (\c 41.66666667)
 * -------------- ------------------------------
 *    \c 25         \c 40
 * -------------- ------------------------------
 *    \c 26         \c 38 (\c 38.46153846)
 * -------------- ------------------------------
 *    \c 27         \c 37 (\c 37.03703704)
 * -------------- ------------------------------
 *    \c 28         \c 35 (\c 35.71428571)
 * -------------- ------------------------------
 *    \c 29         \c 34 (\c 34.48275862)
 * -------------- ------------------------------
 *    \c 30         \c 33 (\c 33.33333333)
 * -------------- ------------------------------
 *    \c 31         \c 32 (\c 32.25806452)
 * -------------- ------------------------------
 *    \c 32         \c 31 (\c 31.25)
 * -------------- ------------------------------
 *    \c 33         \c 30 (\c 30.3030303)
 * -------------- ------------------------------
 *    \c 34         \c 29 (\c 29.41176471)
 * -------------- ------------------------------
 *    \c 35         \c 28 (\c 28.57142857)
 * -------------- ------------------------------
 *    \c 37         \c 27 (\c 27.02702703)
 * -------------- ------------------------------
 *    \c 38         \c 26 (\c 26.31578947)
 * -------------- ------------------------------
 *    \c 40         \c 25
 * -------------- ------------------------------
 *    \c 41         \c 24 (\c 24.3902439 )
 * -------------- ------------------------------
 *    \c 43         \c 23 (\c 23.25581395)
 * -------------- ------------------------------
 *    \c 45         \c 22 (\c 22.22222222)
 * -------------- ------------------------------
 *    \c 47         \c 21 (\c 21.27659574)
 * -------------- ------------------------------
 *    \c 50         \c 20
 * -------------- ------------------------------
 *    \c 52         \c 19 (\c 19.23076923)
 * -------------- ------------------------------
 *    \c 55         \c 18 (\c 18.18181818)
 * -------------- ------------------------------
 *    \c 58         \c 17 (\c 17.24137931)
 * -------------- ------------------------------
 *    \c 62         \c 16 (\c 16.12903226)
 * -------------- ------------------------------
 *    \c 64         \c 15 (\c 15.625)
 * ============== ==============================
 * \endverbatim
 *
 * \remarks
 *   - The associated timer loopback channel will also be configured for open load
 *   and short circuit detection for all channels.
 *
 * \remarks
 *   - The associated analog feedback channel will also be configured for all channels.
 *
 * \remarks
 *   - The associated precise current-measurement will be configured for the channels
 *   \c #IO_PWM_00 .. \c #IO_PWM_05. These PWM outputs will be switched off after 1 second
 *   if the the continuous current is above 3.0A but below 4.0A or switched off immediately
 *   if the current exceeds 4.0A. After a recovery time of 1 second the output stages get
 *   enabled again. \n
 *
 * \remarks
 *   - The associated overcurrent-measurement will be configured for the channels
 *   \c #IO_PWM_10 .. \c #IO_PWM_11. These PWM outputs will be switched off after 1 seconds
 *   if the the continuous current is above 3.0A but below 4.0A or switched off immediately
 *   if the current exceeds 4.0A. After a recovery time of 1 second the output stages get
 *   enabled again. \n
 *
 * \remarks
 *   - The duty cycle cannot exceed the margin of 100us(lower boundary) and
 *   250us(upper boundary) used for diagnostic
 *   if the parameter diag_margin is \c TRUE. This mode is important for hydraulic coils
 *   If the parameter diag_margin is \c FALSE, no duty cycle range margin will be applied
 *
 * \remarks
 *   - \c #IO_PWM_00, \c #IO_PWM_01, \c #IO_PWM_10 and \c #IO_PWM_11 have their own
 *     frequency time base (variable frequency)
 *   - \c #IO_PWM_02, \c #IO_PWM_03, \c #IO_PWM_04 and \c #IO_PWM_05 share one
 *     frequency time base. It's only allowed to configure these outputs
 *     with the same PWM frequency.
 *
 * \remarks
 *    - Static friction and stiction can cause a hysteresis and make the control of a
 *    hydraulic valve erratic and unpredictable. In order to counteract these hysteretic effects,
 *    small vibrations about the desired position shall be created in the valve.
 *    This constantly breaks the static friction ensuring that it will move even with small input changes,
 *    and the effects of hysteresis are average out. A proper setting of PWM frequency according to
 *    the resonance frequency of the actuator allows to adjust this desired small vibration,
 *    low enough in amplitude to prevent noticeable oscillations on the hydraulic output but sufficient
 *    high to prevent friction.
 *    The PWM frequency can be set in the range of 15 .. 1000Hz, a typical range for hydraulic valves
 *    to operate without friction is 90 .. 160Hz.
 *
 * \note
 *   - If a PWM channel is initialized as safety and a valid safety configuration is given,
 *   the diagnostic margin must be set to #TRUE. Otherwise the initialization will fail and
 *   the error code #IO_E_INVALID_PARAMETER is returned.
 *   - When one PWM channel of a frequency group is initialized,
 *   it's only allowed to initialize the other channels of this group with the
 *   same frequency. Otherwise the function will return #IO_E_GROUP_CONFLICT.
 *
 * \attention
 *   - For the current measurement circuitry of the channels \c #IO_PWM_00 .. \c #IO_PWM_05 a
 *   offset compensation will be made at startup of the channel. The application software has to make
 *   sure that no current is driven by the actuator coil (through the free wheeling diode) during this
 *   time. Otherwise the offset compensation will be wrong and the channel startup may fail.
 *   That means that in case a PWM channel with current measurement is re-initialized during runtime,
 *   the application has to make sure that there is enough time between the calls of \c #IO_PWM_DeInit
 *   and \c #IO_PWM_Init or \c #IO_PWM_CurrentInit and \c #IO_PWM_CurrentDeInit for the actuator coil
 *   to finish its recovery phase (time depends on inductance of the load). This is mainly relevant for
 *   channels that will be re-initialized during runtime and  are connected to a inductive load.
 *
 ***************************************************************************/
IO_ErrorType IO_PWM_Init( IO_PIN pwm_channel
                        , ubyte2 frequency
                        , bool polarity
                        , bool diag_margin
                        , ubyte2 overload_limit
                        , const IO_PWM_SAFETY_CONF * const safety_conf);


/***********************************************************************//**
 *
 * \brief Setup a current controlled PWM output
 *
 * \param pwm_channel        PWM channel (\c #IO_PWM_00 .. \c #IO_PWM_05)
 * \param frequency          PWM frequency (15Hz .. 250Hz, only frequencies with a
 *                                          period of an integral multiple of 1ms
 *                                          are possible)
 * \param pid_config         Coefficients (Ki, Kd, Kp) for PID controller
 * \param pwm_period_multi   Period multiplier for PID controller. The cycle time for
 *                           the PID controller is calculated by multiples of the PWM
 *                           period time.
 *                           - Range: ( (1 .. 255) / PWM_period_[ms] )
 * \param pid_handle         Contains handle of the PID controller used for this output.
 *                           Can be used to interact with the PID controller
 *                           after initializing it (e.g. calling #IO_PID_SetIntegrator)
 * \param safety_conf        Safety configuration for PWM output
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_CHANNEL_BUSY                the PWM output channel or the timer input channel is
 *                                           currently used by another function
 * \retval #IO_E_INVALID_CHANNEL_ID          the PWM output channel or the timer input channel
 *                                           id don't exist
 * \retval #IO_E_INVALID_PARAMETER           a given parameter is out of range
 * \retval #IO_E_CH_CAPABILITY               The PWM capability of this channel
 *                                           has not been activated.
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      The common driver init function has not been called before
 * \retval #IO_E_SW_INTERNAL                 Internal SW error when handling configuration tables
 *                                           (index out of range)
 * \retval #IO_E_GROUP_CONFLICT              configuring the output not allowed due to conflicts with
 *                                           other in/outputs in the same group
 * \retval #IO_E_INVALID_SAFETY_CONFIG       one or more parameters of the safety configuration
 *                                            are invalid (out of range)
 * \retval #IO_E_SAFETY_NOT_SUPPORTED         the given channel does not support safety features
 * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG   the driver has not been initialized as safety device -
 *                                            therefore the safety feature is not available for this channel
 * \retval IO_E_TASK_NO_FREE_SLOTS  No more free slots to setup task function
 *
 ***********************************************************************//**
 *
 * \remarks
 *   - The product of (PWM_period_[ms] x multiplier) must not exceed 255!
 *   So for a frequency of 250Hz the maximum multiplier value equals 63.
 *   For a frequency of 15Hz (15.625Hz, 64ms period time) the maximum multiplier value equals 3.
 *
 * \remarks
 *   - The associated timer loopback channel will also be configured for open load
 *   and short circuit detection for all channels.
 *
 * \remarks
 *   - The associated analog feedback channel will also be configured for all channels.
 *
 * \remarks
 *   - The associated precise current-measurement will be configured for the channels
 *   \c #IO_PWM_00 .. \c #IO_PWM_05. These PWM outputs will be switched off after 1 second
 *   if the the continuous current is above 3.0A but below 4.0A or switched off immediately
 *   if the current exceeds 4.0A. After a recovery time of 1 second the output stages get
 *   enabled again. \n
 * \remarks
 *   - \c #IO_PWM_00 and \c #IO_PWM_01 have their own
 *     frequency time base (variable frequency)
 *   - \c #IO_PWM_02, \c #IO_PWM_03, \c #IO_PWM_04 and \c #IO_PWM_05 share one
 *     frequency time base. It's only allowed to configure these outputs
 *     with the same PWM frequency.
 *
 * \note
 *   When one PWM channel of a frequency group is initialized,
 *   it's only allowed to initialize the other channels of this group with the
 *   same frequency. Otherwise the function will return #IO_E_GROUP_CONFLICT.
 *
 * \attention
 *   The parameter \c pwm_period_multi specifies the cycle time in multiples
 *   of the period time of the PWM signal. However for backwards compatibility
 *   the cycle time of the PID controller can not exceed 255ms. Therefore the
 *   upper limit of this parameter depends on the set PWM frequency. For example
 *   for a 100Hz PWM signal (10ms period time) the highest number for the period
 *   multiplier of the PID controller is 25.
 *
 ***************************************************************************/
 IO_ErrorType IO_PWM_CurrentInit ( IO_PIN pwm_channel
                                 , ubyte2 frequency
                                 , const IO_PID_CONFIG * const pid_config
                                 , ubyte1 pwm_period_multi
                                 , ubyte1 * const pid_handle
                                 , const IO_PWM_CURRENT_SAFETY_CONF * const safety_conf );

 /***********************************************************************//**
  *
  * \brief Deinitializes a current-controlled PWM output.
  * Allows the re-initialization of the output by other functions
  *
  * \param pwm_channel    PWM channel (\c #IO_PWM_00 .. \c #IO_PWM_05)
  *
  * \return #IO_ErrorType
  * \retval #IO_E_OK                      everything fine
  * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
  * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
  * \retval #IO_E_CH_CAPABILITY           The given channel has no PWM capability
  *
  ***************************************************************************/
 IO_ErrorType IO_PWM_CurrentDeInit (IO_PIN pwm_channel );

/***********************************************************************//**
 *
 * \brief Deinitializes a PWM output.
 *   Allows the re-initialization of the output by other functions
 *
 * \param pwm_channel    PWM channel (\c #IO_PWM_00 .. \c #IO_PWM_05,
 *                                    \c #IO_PWM_10 .. \c #IO_PWM_11)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           The given channel has no PWM capability
 *
 ***************************************************************************
 *
 * \remarks
 *   - If a current measurement is configured for the given PWM channel,
 *   the current measurement channel will also be deinitialized.
 *
 ***************************************************************************/
IO_ErrorType IO_PWM_DeInit( IO_PIN pwm_channel );


/***********************************************************************//**
 *
 * \brief Set the duty cycle for a PWM channel.
 *
 * \param pwm_channel        PWM channel, one of
 *                           - \c #IO_PWM_00 .. \c #IO_PWM_05
 *                           - \c #IO_PWM_10 .. \c #IO_PWM_11
 * \param duty_cycle         Duty cycle for the channel (0..65535)
 * \param duty_cycle_fb      Duty cycle feedback for the channels (optional):
 *                           high-time in us
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                             everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED         the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID             the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY                  The given channel has no PWM capability
 * \retval #IO_E_PWM_OPEN_LOAD_OR_SHORT_BATTERY an open-load or short-to-battery condition has been
 *                                              detected (at the point in time of the occurrence of
 *                                              this error it is not yet possible to distinguish
 *                                              between open-load and short-to-battery conditions)
 * \retval #IO_E_PWM_OPEN_LOAD                  open load has been detected
 * \retval #IO_E_PWM_SHORT_CIRCUIT              short circuit to GND has been detected
 * \retval #IO_E_PWM_SHORT_BATTERY              short to UBAT has been detected
 * \retval #IO_E_PWM_OUTPUT_DISABLED            The PWM output has been disabled by the main CPU and there
 *                                              was no output signal detected on the output.
 * \retval #IO_E_PWM_CHANNEL_STARTUP            the PWM output is in it's startup phase
 * \retval #IO_E_PWM_OUTPUT_STARTUP_ERROR       an error occurred on the channel mode
 * \retval #IO_E_ADC_INVALID                    A ADC error occured
 * \retval #IO_E_SW_INTERNAL                    Internal SW error when handling configuration tables
 *                                              (index out of range)
 * \retval #IO_E_PWM_CAPTURE_ERROR              A capture error occured on the loopback channel. This error
 *                                              can occur if two edges of the feedback signal are to close
 *                                              to each other and the internal timer can not measure the
 *                                              time difference anymore (for example spikes caused by
 *                                              the switching of inductive loads in electric motors).
 * \retval #IO_E_PROT_USER_OVERLOAD             Output current is above the threshold configured on initialization.
 *                                              The output remains active.
 * \retval #IO_E_PROT_TEMP_OVERLOAD             Output current is above 3.0A and output will temporarily
 *                                              be switched off if the current does not decrease to or below
 *                                              3.0A within the next 1 seconds (only \c #IO_PWM_00..\c #IO_PWM_05)
 * \retval #IO_E_PROT_ACTIVE                    Output is disabled, protection is active because
 *                                              of too high output current (over 4A). The driver will
 *                                              try to re-enable the output again in 1 second
 * \retval #IO_E_PROT_FATAL                     Output is disabled, protection is active because
 *                                              of too high output current (over 6A). The driver will
 *                                              try to re-enable the output again in 1 second
 * \retval #IO_E_PROT_REENABLE                  The IO-Driver signals that the switched off PWM output
 *                                              is going to be re-enabled.
 *
 ***************************************************************************
 *
 * \remarks
 *   - The duty cycle cannot exceed a margin of 100us(lower boundary) and 250us(upper boundary)
 *   used for diagnostic if the parameter diag_margin was set TRUE (via the function \c IO_PWM_Init() ).
 *   This mode is important for hydraulic coils.
 *   If the parameter diag_margin is \c FALSE, no duty cycle range margin will be applied.
 *
 * \remarks
 *   - If the parameter \c duty_cycle_fb is \c NULL, the parameter is ignored.
 *   The parameter \c duty_cycle_fb returns the measured pulse-width of the PWM signal
 *   in the last round in us. If the duty cycle measurement is not finished yet, the parameter
 *   \c duty_cycle_fb holds the value 0.
 *
 * \note
 *   - The PWM outputs must be enabled with #IO_POWER_Set. Otherwise the
 *   outputs remain disabled.
 *
 * \note
 *   - PWM outputs configured with frequencies above 250Hz can only sustain a continuous current
 *   of 100mA.
 *
 ***************************************************************************/
IO_ErrorType IO_PWM_SetDuty( IO_PIN pwm_channel
                           , ubyte2 duty_cycle
                           , ubyte4 * const duty_cycle_fb );


/***********************************************************************//**
 *
 * \brief Sets the current for a current-controlled PWM channel.
 *
 * \param pwm_channel        PWM channel, one of
 *                           - \c #IO_PWM_00 .. \c #IO_PWM_05
 * \param current            Current in mA which shall be output
 * \param duty_cycle_fb      Duty cycle feedback for the channels (optional):
 *                           high-time in us
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                             everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED         the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID             the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY                  The given channel has no PWM capability
 * \retval #IO_E_PWM_OPEN_LOAD                  open load has been detected
 * \retval #IO_E_PWM_SHORT_CIRCUIT              short circuit to GND has been detected
 * \retval #IO_E_PWM_SHORT_BATTERY              short to UBAT has been detected
 * \retval #IO_E_PWM_OUTPUT_DISABLED            The PWM output has been disabled by the main CPU and there
 *                                              was no output signal detected on the output.
 * \retval #IO_E_PWM_CHANNEL_STARTUP            the PWM output is in it's startup phase
 * \retval #IO_E_PWM_OUTPUT_STARTUP_ERROR       an error occurred on the channel mode
 * \retval #IO_E_ADC_INVALID                    A ADC error occurred
 * \retval #IO_E_SW_INTERNAL                    Internal SW error when handling configuration tables
 *                                              (index out of range)
 * \retval #IO_E_PWM_CAPTURE_ERROR              A capture error occurred on the loopback channel. This error
 *                                              can occur if two edges of the feedback signal are to close
 *                                              to each other and the internal timer can not measure the
 *                                              time difference anymore (for example spikes caused by
 *                                              the switching of inductive loads in electric motors).
 * \retval #IO_E_PROT_USER_OVERLOAD             Output current is above the threshold configured on initialization.
 *                                              The output remains active.
 * \retval #IO_E_PROT_TEMP_OVERLOAD             Output current is above 3.0A and output will temporarily
 *                                              be switched off if the current does not decrease to or below
 *                                              3.0A within the next 1 seconds.
 * \retval #IO_E_PROT_ACTIVE                    Output is disabled, protection is active because
 *                                              of too high output current (over 4A). The driver will
 *                                              try to re-enable the output again in 1 second
 * \retval #IO_E_PROT_FATAL                     Output is disabled, protection is active because
 *                                              of too high output current (over 6A). The driver will
 *                                              try to re-enable the output again in 1 second
 * \retval #IO_E_PROT_REENABLE                  The IO-Driver signals that the switched off PWM output
 *                                              is going to be re-enabled.
 *
 ***************************************************************************/
IO_ErrorType IO_PWM_SetCur ( IO_PIN pwm_channel
                           , ubyte2 current
                           , ubyte4 * const duty_cycle_fb );

/***********************************************************************//**
 *
 * \brief Returns the measured current of the given channel
 *
 * \param pwm_channel        PWM channel, one of
 *                           - \c #IO_PWM_00 .. \c #IO_PWM_05
 *
 * \param current            Measured current
 *                           Range: 0..7575 (0mA..7575mA)
 *
 * \param fresh              Indicates if new values are available since
 *                           the last call.
 *                           - \c TRUE: Value in "current" is valid
 *                           - \c FALSE: No new value available.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                        Everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    The given PWM channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID        The given channel id does not exist
 * \retval #IO_E_ADC_INVALID               An ADC error occured
 * \retval #IO_E_PWM_CURRENT_INACCURATE    Current measurement has reduced accuracy
 * \retval #IO_E_CH_CAPABILITY             The given channel is not a PWM channel or the
 *                                         given PWM channel has no current measurement
 * \retval #IO_E_NULL_POINTER              A NULL pointer has been passed to the function
 *
 ***************************************************************************
 *
 * \attention
 *   The current measurement for all PWM channels is equidistant, which
 *   means that the sampling happens synchronous to the PWM period. Every 1ms
 *   a current value will be captured. The captured current values will be
 *   averaged over the time of one period of the PWM signal and then provided
 *   to the application.
 *
 * \attention
 *  Only PWM channels \c #IO_PWM_00 .. \c #IO_PWM_05 have current-measurement.\n
 *
 * \remarks
 *   - When the function \c IO_PWM_GetCur() is called, the internal queue holding the
 *   values of the current measurement is flushed. If the function is called
 *   more than once in a cycle it may or may not deliver new values, depending
 *   on how many values the equidistant current measurement has sampled since
 *   the last call.
 *
 * \remarks
 *   - If there is no new current value available (for example the function
 *   \c IO_PWM_GetCur() gets called more frequently than the PWM period) the
 *   flag \c fresh will be set to \c FALSE.
 *
 * \remarks
 *   - If the functions IO_PWM_GetCurQueue() IO_PWM_GetCur() are called after
 *   another, only the first function will deliver a current value because both
 *   functions will empty the internal measurement queue.
 *
 ***************************************************************************/
IO_ErrorType IO_PWM_GetCur( IO_PIN         pwm_channel
                          , ubyte2 * const current
                          , bool   * const fresh );

/***********************************************************************//**
 *
 * \brief Returns the measured current values since the last call
 *        of the given channel
 *
 * \param pwm_channel        PWM channel, one of
 *                           - \c #IO_PWM_00 .. \c #IO_PWM_05
 *                           - \c #IO_PWM_10 .. \c #IO_PWM_11
 * \param current_queue      Queue holding the current values since
 *                           the last call of the step function including
 *                           a queue overrun flag and a counter for the
 *                           quantity of available values
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                         Everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED     The given PWM channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID         The given channel id does not exist
 * \retval #IO_E_ADC_INVALID                An ADC error occured
 * \retval #IO_E_PWM_CURRENT_INACCURATE     Current measurement has reduced accuracy
 * \retval #IO_E_CH_CAPABILITY              The given channel is not a PWM channel or the
 *                                          given PWM channel has no current measurement
 * \retval #IO_E_NULL_POINTER               A NULL pointer has been passed to the function
 *
 ***************************************************************************
 *
 * \attention
 *   The current measurement for all PWM channels is equidistant which
 *   means that the sampling happens synchronous to the PWM period. Every 1ms
 *   a current value will be captured. The captured current values will be
 *   averaged over the time of one period of the PWM signal and then provided
 *   to the application in a queue. The queue storage was chosen to avoid the
 *   loss of any measurement value if the user application runs asynchronous to
 *   the current measurement or if its cycle time is lower than the PWM period.
 *   (Size of queue: \c #IO_PWM_CURRENT_QUEUE_MAX, Queue data type: \c #IO_PWM_CURRENT_QUEUE)
 *
 * \attention
 *  Only PWM channels \c #IO_PWM_00 .. \c #IO_PWM_05 have current-measurement.
 *
 * \remarks
 *   - When the function \c IO_PWM_GetCurQueue() is called, the internal queue
 *   holding the values of the current measurement is flushed. If the function is called
 *   more than once in a cycle it may or may not deliver new values, depending
 *   on how many values the equidistant current measurement has sampled since
 *   the last call.
 *
 * \remarks
 *   - If there is no new current value available (for example the function
 *   \c IO_PWM_GetCurQueue() gets called more frequently than the PWM period)
 *   the value \c count in the structure \c current_queue will be 0.
 *
 * \remarks
 *   - If the functions IO_PWM_GetCurQueue() and IO_PWM_GetCur() are called after
 *   another, only the first function will deliver a current value because both
 *   functions will empty the internal measurement queue.
 *
 ***************************************************************************/
IO_ErrorType IO_PWM_GetCurQueue( IO_PIN                       pwm_channel
                               , IO_PWM_CURRENT_QUEUE * const current_queue );


#endif /* _IO_PWM_H */
