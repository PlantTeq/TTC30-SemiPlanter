/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_PWD.h
 *
 * \brief IO Driver functions for timer input channels.
 *
 *   Contains all service functions for the PWD (Pulse Width Demodulation).
 *   There are two different groups of timer inputs:
 *   - \c #IO_PWD_00 .. \c #IO_PWD_03 and \c #IO_PWD_22 .. \c #IO_PWD_23:
 *           Complex timer inputs. Can be configured to measure
 *           frequency, pulse-width and duty-cycle at the same time.
 *           Furthermore pins IO_PWD_00 in combination with IO_PWD_01
 *           can read incremental (relative) encoders.
 *           In this case two inputs are reserved for one incremental encoder
 *           (clock and direction).
 *
 *   - \c #IO_PWD_10 .. \c #IO_PWD_13, \c #IO_PWD_20 .. \c #IO_PWD_21: \n
 *           Simple timer inputs. Can be configured to measure
 *           either frequency or pulse-width. These inputs evaluate only
 *           2 edges (frequency) or 3 edges (pulse-width).
 *
 * \section pwd_examples PWD code examples
 *
 * Please refer to section \ref appl_structure for understanding where to place the initialization and task function calls.
 *
 *   PWD initialization examples:
 *   \code
 *   // setup frequency measurement input
 *   IO_PWD_FreqInit( IO_PWD_20
 *                  , IO_PWD_FALLING_VAR );  // select falling edge as variable
 *
 *   // setup pulse measurement input
 *   IO_PWD_PulseInit( IO_PWD_21
 *                   , IO_PWD_HIGH_TIME );   // measure high time of the signal
 *
 *   // setup complex timer input (frequency and pulse measurement)
 *   IO_PWD_ComplexInit( IO_PWD_03
 *                     , IO_PWD_HIGH_TIME        // measure high time
 *                     , IO_PWD_FALLING_VAR      // select falling edge as variable
 *                     , IO_PWD_RESOLUTION_0_8   // set timer resolution to 0.8us
 *                     , 8                       // set number of accumulations
 *                     , IO_PWD_PU               // configure pull-up resistor
 *                     , NULL );                 // Handle for PID controller not needed => pass NULL
 *
 *   // setup incremental input
 *   IO_PWD_IncInit( IO_PWD_00
 *                 , IO_PWD_INC_2_COUNT
 *                 , 0x7FFF                  // set initial value for counter
 *                 , IO_PWD_PU               // configure pull-up resistor
 *                 , NULL );                 // Handle for PID controller not needed => pass NULL
 *   \endcode
 *
 *   PWD task examples:
 *   \code
 *   ubyte2 freq_20_val, freq_3_val, duty_3_val, adc_3_val, inc_0_val, adc_4_val, adc_5_val;
 *   ubyte4 pulse_21_val, pulse_3_val;
 *   bool adc_3_fresh, adc_4_fresh, adc_5_fresh;
 *
 *   // read frequency value
 *   IO_PWD_FreqGet( IO_PWD_20
 *                 , &freq_20_val );
 *
 *   // read pulse value
 *   IO_PWD_PulseGet( IO_PWD_21
 *                  , &pulse_21_val );
 *
 *   // read complex timer values (frequency and pulse value)
 *   IO_PWD_ComplexGet( IO_PWD_03
 *                    , &freq_3_val
 *                    , &pulse_3_val
 *                    , NULL             // pulse samples not needed
 *                    , &duty_3_val
 *                    , &adc_3_val
 *                    , &adc_3_fresh );
 *
 *   // read incremental counter value
 *   IO_PWD_IncGet ( IO_PWD_00
 *                 , &inc_0_val
 *                 , &adc_4_val
 *                 , &adc_5_val
 *                 , &adc_4_fresh
 *                 , &adc_5_fresh);
 *   \endcode
 *
 **************************************************************************/

#ifndef _IO_PWD_H
#define _IO_PWD_H

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
 * \name High- / Low time
 * \anchor pwd_htlt_def
 *
 * Specifies whether the high,low or both(period) time shall be captured
 */
/*@{*/
#define IO_PWD_LOW_TIME                 0   /**< capture the low time of the input signal  */
#define IO_PWD_HIGH_TIME                1   /**< capture the high time of the input signal */
#define IO_PWD_PERIOD_TIME              2   /**< capture the high and low time of the input signal */
/*@}*/


/**
 * \name Variable edge
 * \anchor pwd_var_edge_def
 *
 * Specify the variable edge of the input signal. \n
 * If the rising edge is variable, the frequency is measured between
 * the surrounding falling edges.
 */
/*@{*/
#define IO_PWD_RISING_VAR               2   /**< rising edge of the input signal is the variable one  */
#define IO_PWD_FALLING_VAR              3   /**< falling edge of the input signal is the variable one */
/*@}*/


/**
 * \name Counting mode
 * \anchor pwd_count_mode_def
 *
 * Specify the counting mode of a incremental channel.
 */
/*@{*/
#define IO_PWD_INC_2_COUNT              0x03    /**< count on any edge of the two inputs      */
#define IO_PWD_INC_1_COUNT              0x01    /**< count only on one edge of the two inputs */
/*@}*/

/**
 * \name PWD resolution configuration
 * \anchor pwd_res_def
 *
 * Resolution of the timer of a complex timer input.
 * Note: the timing measurement is based on a 16bit timer,
 * therefore the product (65535 * resolution) must be greater than the period
 * that shall be measured
 */
/*@{*/
#define IO_PWD_RESOLUTION_0_2           0x01    /**< resolution is set to 0.2 us */
#define IO_PWD_RESOLUTION_0_4           0x02    /**< resolution is set to 0.4 us */
#define IO_PWD_RESOLUTION_0_8           0x03    /**< resolution is set to 0.8 us */
#define IO_PWD_RESOLUTION_1_6           0x04    /**< resolution is set to 1.6 us */
#define IO_PWD_RESOLUTION_3_2           0x05    /**< resolution is set to 3.2 us */
/*@}*/

/**
 * \name Pull up / down configuration
 * \anchor pwd_pupd_def
 * Pull up/down resistor for the PWD inputs
 */
/*@{*/
#define IO_PWD_PU                          0x01 /**< pull-up resistor */
#define IO_PWD_PD                          0x02 /**< pull-down resistor */
/*@}*/


/**
 * \name edge count
 * \anchor pwd_count_edge_def
 *
 * Specify on which edge shall be counted
 */
/*@{*/
#define IO_PWD_RISING_COUNT                1   /**< count on a rising edge  */
#define IO_PWD_FALLING_COUNT               2   /**< count on a falling edge */
#define IO_PWD_BOTH_COUNT                  3   /**< count on both edges     */
/*@}*/

/**
 * \name count direction
 * \anchor pwd_count_dir_def
 *
 * Specify the counting direction
 */
/*@{*/
#define IO_PWD_UP_COUNT                    0   /**< count up  */
#define IO_PWD_DOWN_COUNT                  1   /**< count down */
/*@}*/


/**
 * \name PWD maximum pulse pulse-width samples
 * \anchor pwd_max_pulsewidth_def
 *
 * Maximum pulse-width samples that can be stored in the datastructure
 * \c IO_PWD_PULSE_SAMPLES
 */
/*@{*/
#define IO_PWD_MAX_PULSE_SAMPLES        9
/*@}*/

/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/**
 *
 * \brief PWD pulse-width data structure
 *
 *  stores each captured pulse-width for one measurement.
 *
 */
typedef struct _io_pwd_pulse_samples
{
    ubyte1 pulse_samples_count;                         /**< number of pulse_samples                              */
    ubyte4 pulse_sample[IO_PWD_MAX_PULSE_SAMPLES];      /**< stores each captured pulse-width for one measurement */
} IO_PWD_PULSE_SAMPLES;


/**
 *
 * \brief Safety configuration for the Incremental or Counter PWD inputs
 *
 *  Stores all relevant safety configuration parameters for the Incremental PWD inputs.
 *
 */
typedef struct _io_pwd_inc_safety_conf
{
    ubyte2 pwd_cnt_val_upper;           /**< Upper PWD counter limit [1..65534]             */
    ubyte2 pwd_cnt_val_lower;           /**< Lower PWD counter limit [1..65534]             */
} IO_PWD_INC_SAFETY_CONF;

/**
 *
 * \brief Safety configuration for the Complex PWD inputs
 *
 *  Stores all relevant safety configuration parameters for the Complex PWD inputs.
 *
 */
typedef struct _io_pwd_cplx_safety_conf
{
    ubyte2 pwd_freq_val_upper;           /**< Upper PWD frequency limit in Hz [1Hz..65534Hz]    */
    ubyte2 pwd_freq_val_lower;           /**< Lower PWD frequency limit in Hz [1Hz..65534Hz]    */
    ubyte4 pwd_pulse_val_upper;          /**< Upper limit of the pulse time in us for a single PWD pulse [1us..4294967294us]. \n
                                              Depending on the configuration of the channel (whether the high or low time
                                              of the pulse should be measured) this value represents the respective time segment. */
    ubyte4 pwd_pulse_val_lower;          /**< Lower limit of the pulse time in us for a single PWD pulse [1us..4294967294us]. \n
                                              Depending on the configuration of the channel (whether the high or low time
                                              of the pulse should be measured) this value represents the respective time segment. */
} IO_PWD_CPLX_SAFETY_CONF;


/**************************************************************************
 *
 * F U N C T I O N S
 *
 **************************************************************************/

/***********************************************************************//**
 *
 * \brief Setup single incremental interface
 *
 * \param inc_channel           Incremental channel (\c #IO_PWD_00 or \c #IO_PWD_01)
 * \param mode                  Defines the counter behavior
 *                                 - \c #IO_PWD_INC_2_COUNT: Counts up/down on any edge
 *                                                     of the two inputs
 *                                 - \c #IO_PWD_INC_1_COUNT: Counts up/down on any edge
 *                                                     of one channel (\c #IO_PWD_00 or \c #IO_PWD_01)
 * \param count_init            Init value of incremental counter (0..65535)
 * \param pupd                  Specify which pull-resistor to use
 *                                  - \c #IO_PWD_PU: pull-up resistor to 5V
 *                                  - \c #IO_PWD_PD: pull-down resistor to ground
 * \param safety_conf           Safety configuration
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_CHANNEL_BUSY                the channel is
 *                                           currently used by another function
 * \retval #IO_E_INVALID_CHANNEL_ID          the channel id does not exist
 * \retval #IO_E_INVALID_PARAMETER           a parameter is out of range
 * \retval #IO_E_CH_CAPABILITY               The PWD-Inc capability of this channel
 *                                           has not been activated
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      The common driver init function has not been called before
 * \retval #IO_E_SW_INTERNAL                 Internal SW malfunction
 * \retval #IO_E_INVALID_SAFETY_CONFIG       one or more parameters of the safety configuration
 *                                           are invalid (out of range)
 * \retval #IO_E_SAFETY_NOT_SUPPORTED        the given channel does not support safety features
 * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG  the driver has not been initialized as safety device -
 *                                           therefore the safety feature is not available for this channel
 *
 ***************************************************************************
 *
 * \remarks
 *   \c #IO_PWD_00 in combination with \c #IO_PWD_01 defines the incremental interface.
 *   Both PWD channels are used (needed) for the incremental interface.
 *
 * \remarks
 *    The given channel is the primary channel. All further operations (step and deinit functions)
 *    must be performed with this channel.
 *
 * \remarks
 *    - Check the alternate functions of the pins used. A pin can only be configured for one function
 *      at a time.The alternate functions can be found at \ref IO_Pins.h
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_IncInit( IO_PIN inc_channel
                           , ubyte1 mode
                           , ubyte2 count_init
                           , ubyte1 pupd
                           , const IO_PWD_INC_SAFETY_CONF * const safety_conf );

/***********************************************************************//**
 *
 * \brief Get the counter value of a incremental interface
 *
 * \param inc_channel               Incremental channel (\c #IO_PWD_00 .. \c #IO_PWD_01)
 * \param count                     Value of incremental counter (0..65535)
 * \param adc_value_0               ADC value channel 0, Range: 0..32780 (in mV; 0..33333 for TTC32 variants)
 * \param adc_value_1               ADC value channel 1, Range: 0..32780 (in mV; 0..33333 for TTC32 variants)
 * \param fresh_0                   Status of the ADC value channel 0
 *                                      - \c TRUE:  ADC value is fresh
 *                                      - \c FALSE: ADC value is old
 * \param fresh_1                   Status of the ADC value channel 1
 *                                      - \c TRUE:  ADC value is fresh
 *                                      - \c FALSE: ADC value is old
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED      the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID          the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY               The given channel is not a incremental
 *                                           timer input
 * \retval #IO_E_NULL_POINTER                a NULL pointer has been passed to the function
 * \retval #IO_E_ADC_INVALID                 the ADC value is invalid
 *
 ***************************************************************************
 *
 * \remarks
 *    Use the primary channel (same channel that is used in the init function).
 *
 * \remarks
 *    The parameters \c adc_value_0, \c adc_value_1, \c fresh_0 and \c fresh_1 are optional.
 *    If not needed, these parameters can be set \c NULL to ignore them. If the
 *    parameters should be calculated one has to provide both, a valid pointer
 *    to the location of the adc value and the fresh indication for both
 *    input channels!
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_IncGet( IO_PIN inc_channel
                          , ubyte2 * const count
                          , ubyte2 * const adc_value_0
                          , ubyte2 * const adc_value_1
                          , bool * const fresh_0
                          , bool * const fresh_1 );

/***********************************************************************//**
 *
 * \brief Set the counter value of a incremental interface
 *
 * \param inc_channel               Incremental channel (\c #IO_PWD_00 .. \c #IO_PWD_01)
 * \param count                     New value to set of incremental counter (0..65535)
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                        everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID        the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY             The given channel is not a incremental
 *                                         timer input
 *
 ***************************************************************************
 *
 * \remarks
 *    Use the primary channel (same channel that is used in the init function).
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_IncSet( IO_PIN inc_channel
                          , ubyte2 count );

/***********************************************************************//**
 *
 * \brief Deinitializes a single incremental interface
 *
 * \param inc_channel                   Incremental channel (\c #IO_PWD_00 .. \c #IO_PWD_01)
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           The given channel is not a incremental
 *                                       timer input
 *
 ***************************************************************************
 *
 * \remarks
 *    Use the primary channel (same channel that is used in the init function).
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_IncDeInit( IO_PIN inc_channel );

/***********************************************************************//**
 *
 * \brief Setup single timer channel that measures frequency and pulse-width
 *        at the same time
 *
 * \param timer_channel        Timer channel, one of:
 *                                  - \c #IO_PWD_00 .. \c #IO_PWD_03
 *                                  - \c #IO_PWD_22 .. \c #IO_PWD_23
 * \param pulse_mode           Specify which pulse time to measure:
 *                                  - \c #IO_PWD_HIGH_TIME:   configuration to measure
 *                                                            pulse-high-time
 *                                  - \c #IO_PWD_LOW_TIME:    configuration to measure
 *                                                            pulse-low-time
 *                                  - \c #IO_PWD_PERIOD_TIME: configuration to measure
 *                                                            pulse-high and low-time (Period).
 *                                                            The parameter \c pulse_width of the
 *                                                            function \c IO_PWD_ComplexGet will contain the
 *                                                            period time instead of the pulse-width.
 * \param freq_mode            Specify the variable edge
 *                                  - \c #IO_PWD_RISING_VAR:  rising edge is variable
 *                                                            this means that frequency is measured
 *                                                            on falling edges
 *                                  - \c #IO_PWD_FALLING_VAR: falling edge is variable
 *                                                            this means that frequency is measured
 *                                                            on rising edges
 * \param timer_res            Specify the timer resolution (only for \c #IO_PWD_00 .. \c #IO_PWD_03)
 *                                  - \c #IO_PWD_RESOLUTION_0_2:  0.2us
 *                                  - \c #IO_PWD_RESOLUTION_0_4:  0.4us
 *                                  - \c #IO_PWD_RESOLUTION_0_8:  0.8us
 *                                  - \c #IO_PWD_RESOLUTION_1_6:  1.6us
 *                                  - \c #IO_PWD_RESOLUTION_3_2:  3.2us
 * \param capture_count        Number of frequency/pulse-width measurements that will be accumulated (0..8)
 * \param pupd                 Specify which pull-resistor to use (only for \c #IO_PWD_00 .. \c #IO_PWD_03)
 *                                  - \c #IO_PWD_PU: pull-up resistor to 5V
 *                                  - \c #IO_PWD_PD: pull-down resistor to ground
 * \param safety_conf          Safety configuration.
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_CHANNEL_BUSY                the channel is
 *                                           currently used by another function
 * \retval #IO_E_INVALID_CHANNEL_ID          the channel id does not exist
 * \retval #IO_E_INVALID_PARAMETER           parameter capture_count, timebase or mode is out of range
 * \retval #IO_E_CH_CAPABILITY               The PWD-Complex capability of this channel
 *                                           has not been activated
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      The common driver init function has not been called before
 * \retval #IO_E_GROUP_CONFLICT              configuring the output not allowed due to conflicts with
 *                                           other in/outputs in the same group
 * \retval #IO_E_SAFETY_NOT_SUPPORTED        the given channel does not support to be configured safety critical
 * \retval #IO_E_SW_INTERNAL                 an internal software error occurred
 * \retval #IO_E_INVALID_SAFETY_CONFIG       one or more parameters of the safety configuration
 *                                           are invalid (out of range)
 * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG  the driver has not been initialized as safety device -
 *                                           therefore the safety feature is not available for this channel
 *
 ***************************************************************************
 *
 * \remarks
 *   A channel that is initalized with this function can retrieve frequency
 *   and duty cycle by calling the function: \c IO_PWD_ComplexGet()
 *
 * \remarks
 *   The timing measurement is based on a 16bit timer + 8bit overflow timer,
 *   therefore the product (65535 * 255 * resolution) must be greater than the
 *   period that shall be measured. If this period is greater, the function
 *   will return \c #IO_E_PWD_TIMER_OVERFLOW.
 *   The maximum frequency that can be measured is around 10kHz
 *   (restricted by low pass filtering)
 *
 * \remarks
 *   The parameters \c timer_res and \c pupd are only consired for \c #IO_PWD_00 .. \c #IO_PWD_03.
 *   When configuring \c #IO_PWD_22 .. \c #IO_PWD_23 these parameters are ignored.
 *
 * \remarks
 *   If \c capture_count = 0, the driver accumulates all the measurements captured in the last
 *   round. Note: In this mode at least 4 edges are required for one measurement.
 *   If \c capture_count = 1..8, the driver captures exactly as many measurements are given in
 *   the parameter capture_count. Note: Until not all configured samples are captured, the driver
 *   don't return a value.
 *
 * \remarks
 *   The channels \c #IO_PWD_22 and \c #IO_PWD_23 are not allowed to be
 *   configured safety critical.
 *
 * \remarks
 *   The resolution of the channels \c #IO_PWD_22 and \c #IO_PWD_23
 *   is fixed to 0.2 microseconds (=200ns).
 *
 *    - The following channels form groups. They have to be configured in the same mode within a group.
 *        - \c #IO_PWD_00 .. \c #IO_PWD_01
 *        - \c #IO_PWD_02 .. \c #IO_PWD_03
 *
 *    - Check the alternate functions of the pins used in each group. A pin can only be configured for one function
 *    at a time and it has to be the same function within the group.The alternate functions can be found at \ref IO_Pins.h
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_ComplexInit( IO_PIN timer_channel
                               , ubyte1 pulse_mode
                               , ubyte1 freq_mode
                               , ubyte1 timer_res
                               , ubyte1 capture_count
                               , ubyte1 pupd
                               , const IO_PWD_CPLX_SAFETY_CONF * const safety_conf );

/***********************************************************************//**
 *
 * \brief Get the frequency and the pulse-width from the specified timer channel
 *
 * \param timer_channel            Timer channel, one of:
 *                                  - \c #IO_PWD_00 .. \c #IO_PWD_03
 *                                  - \c #IO_PWD_22 .. \c #IO_PWD_23
 * \param frequency                Accumulated frequency in Hz
 * \param pulse_width              Accumulated pulse-width in us
 * \param pulse_samples            contains each pulse-width measure sample
 * \param duty_cycle               contains the measured duty cycle in
 *                                 percent * 10 (e.g. 200 for 20%).
 * \param adc_value                ADC value, range: 0..32780 (in mV; 0..33333 for TTC32 variants)
 * \param fresh                    Status of the ADC value
 *                                    - \c TRUE:  ADC value is fresh
 *                                    - \c FALSE: ADC value is old
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                        everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID        the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY             The given channel is either not a PWD input
 * \retval #IO_E_PWD_NOT_FINISHED          not enough edges to accumulate a result
 * \retval #IO_E_PWD_CAPTURE_ERROR         the frequency was too high
 * \retval #IO_E_NULL_POINTER              A NULL pointer has been passed
 * \retval #IO_E_ADC_INVALID               A ADC error occured
 * \retval #IO_E_PWD_TIMER_OVERFLOW        A timer overflow occured
 *
 ***************************************************************************
 *
 * \remarks
 *   - The timing measurement is based on a 16bit timer + 8bit overflow timer,
 *   therefore the product (65535 * 255 * resolution) must be greater than the
 *   period that shall be measured. If this period is greater, the function
 *   return \c #IO_E_PWD_TIMER_OVERFLOW.
 *   The maximum frequency that can be measured is around 10kHz
 *   (restricted by low pass filtering)
 *
 * \remarks
 *    - The parameters \c adc_value and \c fresh are optional.
 *    If not needed, these parameters can be set NULL to ignore them. If the
 *    parameters should be calculated one has to provide both, a valid pointer
 *    to the location of the adc_value and the fresh indication.
 *
 * \remarks
 *   - if each individual measured pulse-width sample is not needed, the parameter
 *   pulse_samples should be set to \c NULL
 *
 * \remarks
 *   - The parameter \c duty_cycle contains the duty cycle of the active signal part.
 *     - If \c pulse_mode is \c #IO_PWD_HIGH_TIME, \c duty_cycle contains the duty cycle of the high time
 *     - If \c pulse_mode is \c #IO_PWD_LOW_TIME, \c duty_cycle contains the duty cycle of the low time
 *
 * \attention
 *   - The calculation of the parameter \c duty_cycle is heavily time consuming due to the slow division unit
 *   of the XC2000, especially when the pulse-width is greater or equal 0.4294967s. It is recommended to set
 *   this parameter to \c #NULL if it is not needed (the calculation of the parameter \c duty_cycle will then
 *   be skipped)!
 *
 * \remarks
 *   - The parameter \c pulse_width contains the
 *     - high time of the measured signal in microseconds, if \c pulse_mode was set to \c #IO_PWD_HIGH_TIME
 *     - low time of the measured signal in microseconds, if \c pulse_mode was set to \c #IO_PWD_LOW_TIME
 *     - period-time of the measured signal in microseconds, if \c pulse_mode was set to \c #IO_PWD_PERIOD_TIME
 *
 * \attention
 *   - The parameter \c frequency only shows integral frequency values. For input frequencies smaller
 *   than 1Hz this value is set to 0 and only the parameters \c pulse-width and \c duty_cycle are valid.
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_ComplexGet( IO_PIN timer_channel
                              , ubyte2 * const frequency
                              , ubyte4 * const pulse_width
                              , IO_PWD_PULSE_SAMPLES * const pulse_samples
                              , ubyte2 * const duty_cycle
                              , ubyte2 * const adc_value
                              , bool * const fresh );

/***********************************************************************//**
 *
 * \brief Deinitializes a complex PWD input
 *
 * \param timer_channel                  Timer channel, one of:
 *                                         - \c #IO_PWD_00 .. \c #IO_PWD_03
 *                                         - \c #IO_PWD_22 .. \c #IO_PWD_23
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           The given channel is not a PWD input
 *
 * \remarks
 *   - The following channels form groups. A group always has to be configured as a whole.
 *        - \c #IO_PWD_00 .. \c #IO_PWD_01
 *        - \c #IO_PWD_02 .. \c #IO_PWD_03
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_ComplexDeInit( IO_PIN timer_channel );

/***********************************************************************//**
 *
 * \brief Setup single timer channel that measures frequency only
 *
 * \param timer_channel         Timer channel, one of:
 *                                  - \c #IO_PWD_10 .. \c #IO_PWD_13
 *                                  - \c #IO_PWD_20 .. \c #IO_PWD_21
 * \param freq_mode             Specify the variable edge
 *                                  - \c #IO_PWD_RISING_VAR:  rising edge is variable
 *                                  - \c #IO_PWD_FALLING_VAR: falling edge is variable
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                         everything fine
 * \retval #IO_E_CHANNEL_BUSY               the input channel is
 *                                          currently used by another function
 * \retval #IO_E_INVALID_CHANNEL_ID         the input channel id does not exist
 * \retval #IO_E_INVALID_PARAMETER          parameter mode is out of range
 * \retval #IO_E_CH_CAPABILITY              The PWD-Freq capability of this channel
 *                                          has not been activated
 * \retval #IO_E_DRIVER_NOT_INITIALIZED     The common driver init function has not been called before
 *
 ***************************************************************************
 *
 * \remarks
 *   The resolution of the channels \c #IO_PWD_10 .. \c #IO_PWD_13, \c #IO_PWD_20
 *   and \c #IO_PWD_21 is fixed to 1.6 microseconds.
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_FreqInit( IO_PIN timer_channel
                            , ubyte1 freq_mode );

/***********************************************************************//**
 *
 * \brief Setup single timer channel that measures pulse-width only
 *
 * \param  timer_channel        Timer channel, one of:
 *                                  - \c #IO_PWD_10 .. \c #IO_PWD_13
 *                                  - \c #IO_PWD_20 .. \c #IO_PWD_21
 * \param  pulse_mode           Capture mode
 *                                  - \c #IO_PWD_HIGH_TIME: configuration to measure
 *                                                          pulse-high-time
 *                                  - \c #IO_PWD_LOW_TIME:  configuration to measure
 *                                                          pulse-low-time
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                         everything fine
 * \retval #IO_E_CHANNEL_BUSY               the input channel is
 *                                          currently used by another function
 * \retval #IO_E_INVALID_CHANNEL_ID         the input channel id does not exist
 * \retval #IO_E_INVALID_PARAMETER          parameter mode is out of range
 * \retval #IO_E_CH_CAPABILITY              The PWD-Pulse capability of this channel
 *                                          has not been activated
 * \retval #IO_E_DRIVER_NOT_INITIALIZED     The common driver init function has not been called before
 *
 ***************************************************************************
 *
 * \remarks
 *   The resolution of the channels \c #IO_PWD_10 .. \c #IO_PWD_13, \c #IO_PWD_20
 *   and \c #IO_PWD_21 is fixed to 1.6 microseconds.
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_PulseInit( IO_PIN timer_channel
                             , ubyte1 pulse_mode );

/***********************************************************************//**
 *
 * \brief Setup single timer channel that measures pulse-width
 *        and frequency
 *
 * \param  timer_channel        Timer channel, one of:
 *                                         - \c #IO_PWD_10 .. \c #IO_PWD_13
 *                                         - \c #IO_PWD_20 .. \c #IO_PWD_21
 * \param  capture_mode         Capture mode
 *                              - IO_PWD_HIGH_TIME:
 *                                - measure pulse-high-time and
 *                                - measure frequency from falling to falling edge
 *                              - IO_PWD_LOW_TIME:  configuration to measure
 *                                - measure pulse-low-time and
 *                                - measure frequency from rising to rising edge
 *
 * \return IO_ErrorType:
 * \retval #IO_E_OK                         everything fine
 * \retval #IO_E_CHANNEL_BUSY               the input channel is
 *                                          currently used by another function
 * \retval #IO_E_INVALID_CHANNEL_ID         the input channel id does not exist
 * \retval #IO_E_INVALID_PARAMETER          parameter mode is out of range
 * \retval #IO_E_CH_CAPABILITY              The PWD-Pulse capability of this channel
 *                                          has not been activated
 * \retval #IO_E_DRIVER_NOT_INITIALIZED     The common driver init function has not been called before
 *
 ***************************************************************************
 *
 * \remarks
 *   The corresponding step function \c IO_PWD_PulseFreqGet has to be called
 *   cyclically with a cycle time smaller than 100ms
 *
 * \remarks
 *   The resolution of the channels \c #IO_PWD_10 .. \c #IO_PWD_13, \c #IO_PWD_20
 *   and \c #IO_PWD_21 is fixed to 1.6 microseconds.
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_PulseFreqInit( IO_PIN timer_channel
                                 , ubyte1 capture_mode );

/***********************************************************************//**
 *
 * \brief Get the frequency
 *
 * \param timer_channel            Timer channel, one of:
 *                                  - \c #IO_PWD_10 .. \c #IO_PWD_13
 *                                  - \c #IO_PWD_20 .. \c #IO_PWD_21
 * \param frequency                Measured frequency in Hz
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                        everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID        the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY             The given channel is not a PWD input
 * \retval #IO_E_PWD_CAPTURE_ERROR         frequency too high
 * \retval #IO_E_NULL_POINTER              A NULL pointer has been passed
 * \retval #IO_E_PWD_HIGH_LEVEL            only a constant high level is detected
 * \retval #IO_E_PWD_LOW_LEVEL             only a constant low level is detected
 * \retval #IO_E_PWD_NOT_FINISHED          not enough edges to accumulate a result
 *
 ***************************************************************************
 *
 * \remarks
 *   The lowest frequency that can be measured
 *   is 10Hz plus a jitter in the size of the cycle time.
 *   If the signal has a lower frequency,
 *   the function returns a frequency value of 0 and a return a
 *   status of \c #IO_E_PWD_HIGH_LEVEL or \c #IO_E_PWD_LOW_LEVEL after 104ms.
 *   The maximum frequency that can be measured is around 10KHz
 *   (restricted by low pass filtering)
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_FreqGet( IO_PIN timer_channel
                           , ubyte2 * const frequency );

/***********************************************************************//**
 *
 * \brief Get the pulse-width
 *
 * \param timer_channel            Timer channel, one of:
 *                                  - \c #IO_PWD_10 .. \c #IO_PWD_13
 *                                  - \c #IO_PWD_20 .. \c #IO_PWD_21
 * \param pulse_width              Measured pulse-width in us
 *
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                        everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID        the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY             The given channel is not a PWD input
 * \retval #IO_E_PWD_CAPTURE_ERROR         frequency too high
 * \retval #IO_E_NULL_POINTER              A NULL pointer has been passed
 * \retval #IO_E_PWD_HIGH_LEVEL            only a constant high level is detected
 * \retval #IO_E_PWD_LOW_LEVEL             only a constant low level is detected
 * \retval #IO_E_PWD_NOT_FINISHED          not enough edges to accumulate a result
 *
 ****************************************************************************
 *
 * \remarks
 *   The largest pulse that can be measured
 *   is 100ms plus a jitter in the size of the cycle time.
 *   If the signal has larger pulses,
 *   the function returns a pulse measurement value of 0 and a return a
 *   status of \c #IO_E_PWD_HIGH_LEVEL or \c #IO_E_PWD_LOW_LEVEL after 104ms.
 *   The maximum frequency that can be measured is around 10KHz
 *   (restricted by low pass filtering)
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_PulseGet( IO_PIN timer_channel
                            , ubyte4 * const pulse_width );

/***********************************************************************//**
 *
 * \brief Get the pulse-width
 *
 * \param timer_channel             Timer channel, one of:
 *                                         - \c #IO_PWD_10 .. \c #IO_PWD_13
 *                                         - \c #IO_PWD_20 .. \c #IO_PWD_21
 *
 * \param frequency                 Measured frequency in Hz (optional parameter)
 * \param pulse_width               Measured pulse-width in us (optional parameter)
 *
 *
 * \return IO_ErrorType:
 * \retval #IO_E_OK                        everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID        the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY             The given channel is not a PWD input
 * \retval #IO_E_PWD_CAPTURE_ERROR         frequency too high
 * \retval #IO_E_PWD_HIGH_LEVEL            only a constant high level is detected
 * \retval #IO_E_PWD_LOW_LEVEL             only a constant low level is detected
 * \retval #IO_E_PWD_NOT_FINISHED          not enough edges to accumulate a result
 *
 ****************************************************************************
 *
 * \remarks
 *   - The largest pulse that can be measured
 *   is 100ms plus a jitter in the size of the cycle time.
 *   If the signal has larger pulses,
 *   the function returns a pulse measurement value of 0 and a return a
 *   status of \c #IO_E_PWD_HIGH_LEVEL or \c #IO_E_PWD_LOW_LEVEL.
 *   The maximum frequency that can be measured is around 10KHz
 *   (restricted by low pass filtering)
 *
 *
 * \attention
 *   - This function has to be called cyclically with a cycle time
 *   smaller than 100ms. Cycle times greater 100ms are not allowed
 *   for this function!
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_PulseFreqGet( IO_PIN timer_channel
                                , ubyte2 * const frequency
                                , ubyte4 * const pulse_width );

/***********************************************************************//**
 *
 * \brief Deinitializes a PWD input for frequency measurement.
 *        Allows the re-initialization of the input by other functions
 *
 * \param  timer_channel                 Timer channel, one of:
 *                                         - \c #IO_PWD_10 .. \c #IO_PWD_13
 *                                         - \c #IO_PWD_20 .. \c #IO_PWD_21
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           The given channel is not a PWD input
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_FreqDeInit( IO_PIN timer_channel );

/***********************************************************************//**
 *
 * \brief Deinitializes a PWD input for pulse-width measurement.
 *        Allows the re-initialization of the input by other functions
 *
 * \param  timer_channel                 Timer channel, one of:
 *                                         - \c #IO_PWD_10 .. \c #IO_PWD_13
 *                                         - \c #IO_PWD_20 .. \c #IO_PWD_21
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           The given channel is not a PWD input
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_PulseDeInit( IO_PIN timer_channel );

/***********************************************************************//**
 *
 * \brief De-initializes a PWD input for pulse-width and
 *        frequency measurement.
 *        Allows the re-initialization of the input by other functions
 *
 * \param  timer_channel                Timer channel , one of:
 *                                         - \c #IO_PWD_10 .. \c #IO_PWD_13
 *                                         - \c #IO_PWD_20 .. \c #IO_PWD_21
 * \return IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           The given channel is not a PWD input
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_PulseFreqDeInit( IO_PIN timer_channel );

/***********************************************************************//**
 *
 * \brief Setup single counter channel
 *
 * \param count_channel        Counter channel (\c #IO_PWD_01)
 * \param mode                 Specify on wich edge shall be count
 *                                  - \c #IO_PWD_RISING_COUNT:  count on a rising edge
 *                                  - \c #IO_PWD_FALLING_COUNT: count on a falling edge
 *                                  - \c #IO_PWD_BOTH_COUNT:    count on a both edges
 * \param direction            Specify the counting direction
 *                                  - \c #IO_PWD_UP_COUNT:      counts up
 *                                  - \c #IO_PWD_DOWN_COUNT:    counts down
 * \param count_init           Init value of counter (0..65535)
 * \param pupd                  Specify which pull-resistor to use
 *                                  - \c #IO_PWD_PU: pull-up resistor to 5V
 *                                  - \c #IO_PWD_PD: pull-down resistor to ground
 * \param safety_conf           Safety configuration
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_CHANNEL_BUSY                the channel is
 *                                           currently used by another function
 * \retval #IO_E_INVALID_CHANNEL_ID          the channel id does not exist
 * \retval #IO_E_INVALID_PARAMETER           a parameter is out of range
 * \retval #IO_E_CH_CAPABILITY               The PWD-Inc capability of this channel
 *                                           has not been activated
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      The common driver init function has not been called before
 * \retval #IO_E_SW_INTERNAL                 Internal SW malfunction
 * \retval #IO_E_INVALID_SAFETY_CONFIG       one or more parameters of the safety configuration
 *                                           are invalid (out of range)
 * \retval #IO_E_SAFETY_NOT_SUPPORTED        the given channel does not support safety features
 * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG  the driver has not been initialized as safety device -
 *                                           therefore the safety feature is not available for this channel
 *
 ***************************************************************************
 *
 * \remarks
 *   - Only channel \c #IO_PWD_01 can be used for counting functionality. \c #IO_PWD_00 must not be configured.
 *   - Check the alternate functions of the pin. A pin can only be configured for one function
 *    at a time.The alternate functions can be found at \ref IO_Pins.h
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_CountInit( IO_PIN count_channel
                             , ubyte1 mode
                             , ubyte1 direction
                             , ubyte2 count_init
                             , ubyte1 pupd
                             , IO_PWD_INC_SAFETY_CONF const * const safety_conf );

/***********************************************************************//**
 *
 * \brief Get the counter value of a single counter channel
 *
 * \param count_channel             Counter channel (\c #IO_PWD_01)
 * \param count                     Value of counter (0..65535)
 * \param adc_value                 ADC value, Range: 0..32780 (in mV; 0..33333 for TTC32 variants)
 * \param fresh                     Status of the ADC value
 *                                      - \c TRUE:  ADC value is fresh
 *                                      - \c FALSE: ADC value is old
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED      the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID          the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY               The given channel is not a incremental
 *                                           timer input
 * \retval #IO_E_NULL_POINTER                a NULL pointer has been passed to the function
 * \retval #IO_E_ADC_INVALID                 the ADC value is invalid
 *
 *
 ***************************************************************************
 *
 * \remarks
 *   Only channel \c #IO_PWD_01 can be used for counting functionality
 *
 * \remarks
 *    The parameters adc_value and fresh are optional.
 *    If not needed, these parameters can be set \c NULL to ignore them. If the
 *    parameters should be calculated one has to provide both, a valid pointer
 *    to the location of the adc_value and the fresh indication.
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_CountGet( IO_PIN count_channel
                            , ubyte2 * const count
                            , ubyte2 * const adc_value
                            , bool * const fresh );

/***********************************************************************//**
 *
 * \brief Set the counter value of a single counter channel
 *
 * \param count_channel             Counter channel (\c #IO_PWD_01)
 * \param count                     New value to set of incremental counter (0..65535)
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                        everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID        the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY             The given channel is not a incremental
 *                                         timer input
 *
 ***************************************************************************
 *
 * \remarks
 *   Only channel \c #IO_PWD_01 can be used for counting functionality
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_CountSet( IO_PIN count_channel
                            , ubyte2 count );

/***********************************************************************//**
 *
 * \brief Deinitializes a single counter channel
 *
 * \param count_channel        Counter channel (\c #IO_PWD_01)
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           The given channel is not a incremental
 *                                       timer input
 *
 ***************************************************************************
 *
 * \remarks
 *   Only channel \c #IO_PWD_01 can be used for counting functionality
 *
 ***************************************************************************/
IO_ErrorType IO_PWD_CountDeInit( IO_PIN count_channel );


#endif /* _IO_PWD_H */
