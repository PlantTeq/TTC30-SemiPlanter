/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_ADC.h
 *
 * \brief IO Driver functions for ADC.
 *
 *  Contains all service functions for the ADC.\n
 *  There are four groups of Analog Inputs available:\n
 *  - ADC 4-Mode: Can be configured as absolute, resistive (0-65535 Ohm),
 *                current (4-20mA) or ratiometric input. An additional
 *                sensor supply measurement (for the ratiometric case) will
 *                be configured and serves to correct the ADC signal.
 *                Input range is 0-5V for ratiometric and configurable
 *                (0-5V or 0-10V) for absolute mode.
 *                Up to 2 channels (4 channels for HY-TTC 32 variants) can be configured (#IO_ADC_00..#IO_ADC_01 for HY-TTC 30 variants and #IO_ADC_14..#IO_ADC_15 for HY-TTC 32 variants).\n
 *  - ADC 3-Mode: 6 ADC inputs (4 ADC inputs for HY-TTC 32 variants) for current (4-20mA sensors), absolute and ratiometric
 *                measurement (#IO_ADC_10..#IO_ADC_15 for HY-TTC 30 variants, #IO_ADC_10..#IO_ADC_13 for HY-TTC 32 variants). Input range is 0-5V for
 *                ratiometric and configurable (0-5V or 0-10V) for absolute mode.
 *                If ratiometric mode is used the measurement of
 *                the sensor supply will be initialized.\n
 *  - ADC 1-Mode: These inputs (#IO_ADC_20..#IO_ADC_41) can only be configured in
 *                absolute mode. Input range: 0-32V\n
 *  - Normal ADC: Various ADC inputs for retrieving onboard voltages
 *                (UBat, Sensor-Supply, Board-Temperature, ...)\n
 *
 *  \section adc_examples ADC Code Examples
 *
 *  Please refer to section \ref appl_structure for understanding where to place the initialization and task function calls.
 *
 *  Examples for ADC initialization:
 *  \code
 *      // ADC 4-Mode:
 *      IO_ADC_ChannelInit( IO_ADC_00
 *                        , IO_ADC_RESISTIVE      // configuration for resistive measurement
 *                        , 0                     // parameter is ignored for IO_ADC_RESISTIVE
 *                        , NULL );               // no safety configuration
 *
 *      // ADC 4-Mode:
 *      IO_ADC_ChannelInit( IO_ADC_01
 *                        , IO_ADC_ABSOLUTE       // configuration for absolute measurement
 *                        , IO_ADC_RANGE_5V       // measurement range 0-5V
 *                        , NULL );               // no safety configuration
 *
 *      // ADC 3-Mode:
 *      IO_ADC_ChannelInit( IO_ADC_10
 *                        , IO_ADC_CURRENT        // configuration for current measurement
 *                        , 0                     // parameter is ignored for IO_ADC_CURRENT
 *                        , NULL );               // no safety configuration
 *
 *      // Normal ADC:
 *      IO_ADC_ChannelInit( IO_ADC_SENSOR_SUPPLY
 *                        , IO_ADC_ABSOLUTE       // Only absolute allowed for normal ADCs
 *                        , 0                     // parameter is ignored for ADC channels other than 3-Mode and 2-Mode
 *                        , NULL );               // safety configuration; not supported
 *  \endcode
 *
 *  Example for ADC task function call:\n
 *  This function call is identical for every type of ADC inputs.
 *  \code
 *      ubyte2 adc_val_0;
 *      bool adc_fresh_0;
 *
 *      IO_ADC_Get( IO_ADC_00
 *                , &adc_val_0
 *                , &adc_fresh_0 );
 *
 *  \endcode
 *
 **************************************************************************/

#ifndef _IO_ADC_H
#define _IO_ADC_H

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
 * \name Types of configurable Analog inputs
 *
 * Input configuration for configurable ADC inputs.
 * These defines can be used for the \c mode parameter of the function
 * \c #IO_ADC_ChannelInit.
 */
/*@{*/
/** Ratiometric configuration \n
 *  use this configuration if the connected sensor is supplied by the sensor
 *  supply (\c #IO_ADC_SENSOR_SUPPLY) and delivers a voltage signal. \n
 *  Task function returns voltage in [mV]
 */
#define IO_ADC_RATIOMETRIC          0x01
/** Current loop configuration \n
 *  use this configuration if the connected sensor deliveres a current signal
 *  (4..20mA sensors) \n
 *  Task function returns current in [uA]
 */
#define IO_ADC_CURRENT              0x03
/** Resistive configuration \n
 *  use this configuration if the sensor value shall be determined by
 *  measuring its resistance \n
 *  Task function returns resistance in [Ohm]
 */
#define IO_ADC_RESISTIVE            0x00
/** Absolute voltage measurement \n
 *  use this configuration to measure an absolute voltage signal \n
 *  Task function returns voltage in [mV]
 */
#define IO_ADC_ABSOLUTE             0x02
/*@}*/

#define IO_ADC_NO_MODE              0xFF


/**
 * \name Range configuration for ADC inputs
 *
 * Configuration of the ADC input range.
 * These defines can be used for the \c range parameter of the function
 * \c IO_ADC_ChannelInit.
 */
/*@{*/
#define IO_ADC_RANGE_5V                 0       /**< ADC range 0 .. 5V  */
#define IO_ADC_RANGE_10V                1       /**< ADC range 0 .. 10V */
/*@}*/

/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/***********************************************************************//**
 *
 * \brief Safety configuration for the ADC inputs
 *
 *  Stores all relevant safety configuration parameters for the ADC inputs
 *  as an absolute value. The values shall be stated as mV, uA or Ohm -
 *  depending on the configuration of the channel.
 *
 *  It is not allowed to set the lower limit higher than or equal to the
 *  upper limit.
 *
 *  The valid min/max limits for each measured unit and range are like follows:
 *  - \c #IO_ADC_ABSOLUTE for  5V range configuration: 1mV ... 5000mV
 *  - \c #IO_ADC_ABSOLUTE for 10V range configuration: 1mV ... 10500mV
 *  - \c #IO_ADC_ABSOLUTE 32V inputs: 1mV ... 32780mV (1mV ... 33333mV for TTC32 variants)
 *  - \c #IO_ADC_RATIOMETRIC: 1mV ... 5000mV
 *  - \c #IO_ADC_CURRENT: 1uA ... 25000uA
 *  - \c #IO_ADC_RESISTIVE: 1 Ohm ... 65534 Ohm
 *
 *  The safety configuration is applicable for the following channels:
 *  - \c #IO_ADC_00 .. \c #IO_ADC_01
 *  - \c #IO_ADC_10 .. \c #IO_ADC_15
 *  - \c #IO_ADC_20 .. \c #IO_ADC_21
 *
 */
typedef struct _io_adc_safety_conf
{
    ubyte2 adc_val_upper;           /**< Upper limit for analog input value */
    ubyte2 adc_val_lower;           /**< Lower limit for analog input value */
}IO_ADC_SAFETY_CONF;

/**************************************************************************
 *
 * F U N C T I O N S
 *
 **************************************************************************/

/***********************************************************************//**
 *
 * \brief Setup one ADC channel
 *
 * \param adc_channel           ADC channel, one of:
 *                                       - \c #IO_ADC_00 .. \c #IO_ADC_01
 *                                       - \c #IO_ADC_10 .. \c #IO_ADC_15
 *                                       - \c #IO_ADC_20 .. \c #IO_ADC_41
 *                                       - \c #IO_K15
 *                                       - \c #IO_ADC_BOARD_TEMP
 *                                       - \c #IO_ADC_SENSOR_SUPPLY
 *                                       - \c #IO_ADC_5V2
 *                                       - \c #IO_ADC_UBAT
 *                                       - \c #IO_ADC_UBAT_CPU
 *                                       - \c #IO_ADC_NODE_ID_0
 *                                       - \c #IO_ADC_NODE_ID_1
 *                                       - \c #IO_INT_PIN_SHIFT_LB_HI
 *                                       - \c #IO_INT_PIN_SHIFT_LB_LO
 *                                       - \c #IO_INT_PIN_SHIFT1_LB_HI
 *                                       - \c #IO_INT_PIN_SHIFT1_LB_LO
 *
 * \param mode      Type of input:
 *   - \c #IO_ADC_RATIOMETRIC: voltage measurement
 *                             proportional to sensor supply voltage
 *   - \c #IO_ADC_CURRENT:     0-27600uA input
 *   - \c #IO_ADC_RESISTIVE:   0-65535Ohm input
 *   - \c #IO_ADC_ABSOLUTE:    normal voltage input
 *
 * \param range     Measurement-range when mode is set to IO_ADC_ABSOLUTE.
 *      This parameter is only evaluated for analog channels with
 *      configurable range, i.e.:
 *          - \c #IO_ADC_00 .. \c #IO_ADC_01
 *          - \c #IO_ADC_10 .. \c #IO_ADC_15
 *      All other channels have a non-configurable range of 0-32V
 *      (parameter is ignored). The parameter can be one of:
 *          - \c #IO_ADC_RANGE_5V
 *          - \c #IO_ADC_RANGE_10V
 *
 * \param safety_conf          Safety configuration.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID          the channel id does not exist
 * \retval #IO_E_INVALID_PARAMETER           parameter is out of range
 * \retval #IO_E_CHANNEL_BUSY                the ADC input channel is
 *                                           currently used by another function
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      The common driver init function has
 *                                           not been called before
 * \retval #IO_E_GROUP_CONFLICT              desired configuration not possible
 *                                           due to a group conflict.
 * \retval #IO_E_TASK_NO_FREE_SLOTS          No more free slots to setup task
 *                                           function
 * \retval #IO_E_SW_INTERNAL                 Internal software error
 * \retval #IO_E_CH_CAPABILITY               The ADC capability of this channel
 *                                           has not been activated
 * \retval #IO_E_SAFETY_NOT_SUPPORTED        the given channel does not support
*                                            safety features
 * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG  the driver has not been initialized
 *                                           as safety device - therefore the
 *                                           safety feature is not available for
 *                                           this channel
 *
 ***************************************************************************
 *
 * \remarks
 *    - The supported features depend on the selected channel:
 *        - \c #IO_ADC_00 .. \c #IO_ADC_01:
 *             - \c mode:    \c #IO_ADC_ABSOLUTE, \c #IO_ADC_RATIOMETRIC,
 *                           \c #IO_ADC_CURRENT or \c #IO_ADC_RESISTIVE
 *        - \c #IO_ADC_10 .. \c #IO_ADC_15:
 *             - \c mode:    \c #IO_ADC_ABSOLUTE,
 *                           \c #IO_ADC_RATIOMETRIC or \c #IO_ADC_CURRENT
 *        - \c #IO_ADC_20 .. \c #IO_ADC_41:
 *             - \c mode:    \c #IO_ADC_ABSOLUTE
 *        - \c #IO_K15:
 *             - \c mode:    \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_5V2:
 *             - \c mode:    \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_BOARD_TEMP:
 *             - \c mode:    \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_SENSOR_SUPPLY:
 *             - \c mode:    \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_UBAT:
 *             - \c mode:    \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_UBAT_CPU:
 *             - \c mode:    \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_NODE_ID_0/#IO_ADC_NODE_ID_1:
 *             - \c mode:    \c #IO_ADC_ABSOLUTE
 *        - \c #IO_INT_PIN_SHIFT_LB_LO/#IO_INT_PIN_SHIFT_LB_HI/#IO_INT_PIN_SHIFT1_LB_LO/#IO_INT_PIN_SHIFT1_LB_HI
 *             - \c mode:    \c #IO_ADC_RATIOMETRIC (ratiometric
 *                           measurement to internal 5V2_VM voltage)
 *
 *    - The following channels form groups. They have to be configured in the same mode within a group.
 *        - \c #IO_ADC_00 .. \c #IO_ADC_01
 *        - \c #IO_ADC_10 .. \c #IO_ADC_11
 *        - \c #IO_ADC_12 .. \c #IO_ADC_13
 *        - \c #IO_ADC_14 .. \c #IO_ADC_15
 *        - \c #IO_ADC_20 .. \c #IO_ADC_21
 *        - \c #IO_ADC_22 .. \c #IO_ADC_24
 *        - \c #IO_ADC_25 .. \c #IO_ADC_27
 *        - \c #IO_ADC_30 .. \c #IO_ADC_31
 *        - \c #IO_ADC_32 .. \c #IO_ADC_33
 *
 *    - Check the alternate functions of the pins used in each group. A pin can only be configured for one function
 *    at a time and it has to be the same function within the group.The alternate functions can be found at \ref IO_Pins.h
 *
 * \remarks
 *    - The channels #IO_ADC_UBAT, #IO_ADC_UBAT_CPU, #IO_ADC_BOARD_TEMP and
 *    #IO_K15 are initialized in the function #IO_Driver_Init. Therefore the
 *    return value is #IO_E_CHANNEL_BUSY if #IO_Driver_Init has been called
 *    before.
 *    - If the driver is initialized with safety-parameters, i.e.,
 *    #IO_Driver_Init has been called with a safety-structure != \c NULL,
 *    the channels #IO_ADC_5V2, #IO_ADC_SENSOR_SUPPLY, #IO_INT_PIN_SHIFT_LB_LO,
 *    #IO_INT_PIN_SHIFT_LB_HI, #IO_INT_PIN_SHIFT1_LB_LO and
 *    #IO_INT_PIN_SHIFT1_LB_HI are additionally initialized with the call
 *    to #IO_Driver_Init (the call to #IO_ADC_ChannelInit will return
 *    #IO_E_CHANNEL_BUSY #IO_Driver_Init has been called before)
 *    - The parameter \c range is only valid if the mode is set to
 *    #IO_ADC_ABSOLUTE
 *
 * \remarks
 *    - For HY-TTC 32 and HY-TTC 32S  channels \c #IO_ADC_14 and \c #IO_ADC_15 support also \c #IO_ADC_RESISTIVE mode
 ***************************************************************************/
IO_ErrorType IO_ADC_ChannelInit( IO_PIN adc_channel
                               , ubyte1 mode
                               , ubyte1 range
                               , const IO_ADC_SAFETY_CONF * const safety_conf );


/***********************************************************************//**
 *
 * \brief Deinitializes one ADC input
 *
 * deinitializes the given ADC channel, allows reconfiguration by
 * \c IO_ADC_ChannelInit()
 *
 * \param adc_channel           ADC channel, one of:
 *                                       - \c #IO_ADC_00 .. \c #IO_ADC_01
 *                                       - \c #IO_ADC_10 .. \c #IO_ADC_15
 *                                       - \c #IO_ADC_20 .. \c #IO_ADC_41
 *                                       - \c #IO_K15
 *                                       - \c #IO_ADC_BOARD_TEMP
 *                                       - \c #IO_ADC_SENSOR_SUPPLY
 *                                       - \c #IO_ADC_UBAT
 *                                       - \c #IO_ADC_UBAT_CPU
 *                                       - \c #IO_ADC_NODE_ID_0
 *                                       - \c #IO_ADC_NODE_ID_1
 *                                       - \c #IO_INT_PIN_SHIFT_LB_HI
 *                                       - \c #IO_INT_PIN_SHIFT_LB_LO
 *                                       - \c #IO_INT_PIN_SHIFT1_LB_HI (only relevant for HY-TTC 32 variants)
 *                                       - \c #IO_INT_PIN_SHIFT1_LB_LO (only relevant for HY-TTC 32 variants)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           The given channel is not a analog input
 *
 * \remarks
 *   - The following channels form groups. A group always has to be configured as a whole.
 *        - \c #IO_ADC_00 .. \c #IO_ADC_01
 *        - \c #IO_ADC_10 .. \c #IO_ADC_11
 *        - \c #IO_ADC_12 .. \c #IO_ADC_13
 *        - \c #IO_ADC_14 .. \c #IO_ADC_15
 *        - \c #IO_ADC_20 .. \c #IO_ADC_21
 *        - \c #IO_ADC_22 .. \c #IO_ADC_24
 *        - \c #IO_ADC_25 .. \c #IO_ADC_27
 *        - \c #IO_ADC_30 .. \c #IO_ADC_31
 *        - \c #IO_ADC_32 .. \c #IO_ADC_33
 *
 ***************************************************************************/
IO_ErrorType IO_ADC_ChannelDeInit( IO_PIN adc_channel );

/***********************************************************************//**
 *
 * \brief Returns the value of the given ADC channel
 *
 * \param adc_channel           ADC channel, one of:
 *                                       - \c #IO_ADC_00 .. \c #IO_ADC_01
 *                                       - \c #IO_ADC_10 .. \c #IO_ADC_15
 *                                       - \c #IO_ADC_20 .. \c #IO_ADC_41
 *                                       - \c #IO_ADC_BOARD_TEMP
 *                                       - \c #IO_ADC_SENSOR_SUPPLY
 *                                       - \c #IO_ADC_5V2
 *                                       - \c #IO_ADC_UBAT
 *                                       - \c #IO_ADC_UBAT_CPU
 *                                       - \c #IO_K15
 *                                       - \c #IO_ADC_NODE_ID_0
 *                                       - \c #IO_ADC_NODE_ID_1
 *                                       - \c #IO_INT_PIN_SHIFT_LB_HI
 *                                       - \c #IO_INT_PIN_SHIFT_LB_LO
 *                                       - \c #IO_INT_PIN_SHIFT1_LB_HI (only relevant for HY-TTC 32 variants)
 *                                       - \c #IO_INT_PIN_SHIFT1_LB_LO (only relevant for HY-TTC 32 variants)
 * \param adc_value             ADC value, the range depends on the input group
 *                              and its configuration (\c type parameter of IO_ADC_ChannelInit() ):
 *                                  - \c #IO_ADC_00 .. \c #IO_ADC_01
 *                                      - \c #IO_ADC_ABSOLUTE:                     0..10500  (0V..10.500V)
 *                                      - \c #IO_ADC_RATIOMETRIC:                0..5000   (0V..5.000V)
 *                                      - \c #IO_ADC_CURRENT:                    0..27600  (0mA..27.600mA)
 *                                      - \c #IO_ADC_RESISTIVE:                  0..65535  (0Ohm..65535Ohm)
 *                                  - \c #IO_ADC_10 .. \c #IO_ADC_15
 *                                      - \c #IO_ADC_ABSOLUTE:                     0..10500  (0V..10.500V)
 *                                      - \c #IO_ADC_RATIOMETRIC:                0..5000   (0V..5.000V)
 *                                      - \c #IO_ADC_CURRENT:                    0..27600  (0mA..27.600mA)
 *                                  - \c #IO_ADC_20 .. \c #IO_ADC_21
 *                                      - \c #IO_ADC_ABSOLUTE:                   0..32780  (0V..32.780V) (0V..33.333V for TTC32 variants)
 *                                  - \c #IO_ADC_22 .. \c #IO_ADC_27
 *                                      - \c #IO_ADC_ABSOLUTE:                   0..32780  (0V..32.780V)
 *                                  - \c #IO_ADC_28 .. \c #IO_ADC_41
 *                                      - \c #IO_ADC_ABSOLUTE:                   0..32780  (0V..32.780V) (0V..33.333V for TTC32 variants)
 *                                  - \c #IO_ADC_BOARD_TEMP
 *                                      - \c #IO_ADC_ABSOLUTE:                   4500..24055 (-60.17..148.62 degree C)
 *                                  - \c #IO_ADC_5V2
 *                                      - \c #IO_ADC_ABSOLUTE:                   0..5313   (0V..5.313V)
 *                                  - \c #IO_ADC_SENSOR_SUPPLY
 *                                      - \c #IO_ADC_ABSOLUTE:                   0..5313   (0V..5.313V)
 *                                  - \c #IO_ADC_UBAT
 *                                      - \c #IO_ADC_ABSOLUTE:                   0..55000  (0V..55.000V)
 *                                  - \c #IO_ADC_UBAT_CPU
 *                                      - \c #IO_ADC_ABSOLUTE:                   0..55000  (0V..55.000V)
 *                                  - \c #IO_ADC_NODE_ID_0/#IO_ADC_NODE_ID_1
 *                                      - \c #IO_ADC_ABSOLUTE:                   0..32780  (0V..32.780V)
 *                                  - \c #IO_K15
 *                                      - \c #IO_ADC_ABSOLUTE:                   0..32780  (0V..32.780V)
 * \param fresh                 Status of the ADC value
 *                                  - \c TRUE:  ADC value is fresh (channel has been converted)
 *                                  - \c FALSE: ADC value is old (channel has not been converted)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_ADC_CHANNEL_STARTUP    Channel is in initialization phase
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY          The given channel is not a analog input
 * \retval #IO_E_NULL_POINTER           A NULL pointer has been passed
 * \retval #IO_E_FET_PROTECTION         FET is disabled, protection is active
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED Channel has not been setup
 * \retval #IO_E_ADC_INVALID            The ADC value is invalid/not available
 *
 ***************************************************************************
 *
 * \remarks
 *    - The temperature value in degree C must recalculated in the following manner, since the value
 *    has an offset of 10000 and is multiplied with 100:\n
 *    degree_value = ((float) adc_value - 10000) / 100\n
 *    Please use the functions \c IO_ADC_BoardTempFloat() or \c IO_ADC_BoardTempSbyte() for this calculation
 *
 * \remarks
 *    - For the input \c #IO_ADC_00 .. \c #IO_ADC_01 and \c #IO_ADC_10 .. \c #IO_ADC_15 in \c #IO_ADC_CURRENT mode,
 *    the inputs will be switched off for 2s if the current exceed 27.600mA. In this time the
 *    functions returns \c #IO_E_FET_PROTECTION.
 * \remarks
 *    - For HY-TTC 32 and HY-TTC 32S channels \c #IO_ADC_14 and \c #IO_ADC_15 support also \c #IO_ADC_RESISTIVE mode
 *
 ***************************************************************************/
IO_ErrorType IO_ADC_Get( IO_PIN adc_channel
                       , ubyte2 * const adc_value
                       , bool * const fresh );
                       
#ifndef TTC30_ACT_TEST_CANTATA_COVERAGE
/***********************************************************************//**
 *
 * \brief Calculates the board temperature in tenth degree Celsius.
 *
 * The function converts the raw ADC value (retrieved from \c IO_ADC_Get())
 * to a temperature in degree Celsius and returns it as a float value.
 *
 * \param raw_value                     raw adc board temperature returned
 *                                      from the \c IO_ADC_Get() function
 *
 * \return the board temperature in degree celsius ( -55 .. 140.55 degree C )
 *
 *
 ***************************************************************************
 *
 * \remarks
 *    Usage:
 *    \code
 *         IO_ADC_Get( IO_ADC_BOARD_TEMP, &raw_value, &fresh );
 *         temp = IO_ADC_BoardTempFloat( raw_value );
 *    \endcode
 *
 ***************************************************************************/
float4 IO_ADC_BoardTempFloat( ubyte2 raw_value );
#endif // TTC30_ACT_TEST_CANTATA_COVERAGE
/***********************************************************************//**
 *
 * \brief Calculates the board temperature in degree Celsius.
 *
 * The function converts the raw ADC value (retrieved from \c IO_ADC_Get())
 * to a temperature in tenth degree Celsius and returns it as a sbyte1 value.
 *
 * \param raw_value                     raw adc board temperature returned
 *                                      from the \c IO_ADC_Get() function
 *
 * \return returns the board temperature in tenth degree celsius
 *         ( -550 .. 1405 which corresponds to -55 .. 140.5 degree C )
 *
 ***************************************************************************
 *
 * \remarks
 *    - Usage:
 *    \code
 *           IO_ADC_Get( IO_ADC_BOARD_TEMP, &raw_value, &fresh );
 *           temp = IO_ADC_BoardTempSbyte( raw_value );
 *    \endcode
 *
 ***************************************************************************/
sbyte2 IO_ADC_BoardTempSbyte( ubyte2 raw_value );


#endif /* _IO_ADC_H */
