/***************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/***********************************************************************//**
 * \file IO_LED.h
 *
 * \brief IO driver functions for LED
 *
 * The LED module allows to turn on and off LEDs on the designated channels
 * \c #IO_LED_00 ... \c #IO_LED_07.
 *
 * \section led_examples LED Code Examples
 *
 * Please refer to section \ref appl_structure for understanding where to place the initialization and task function calls.
 *
 * Example for a LED initialization
 * \code
 * IO_LED_ChannelInit( IO_LED_00 );
 * IO_LED_ChannelInit( IO_LED_01 );
 * \endcode
 *
 * Example to turn on/off a LED
 * \code
 * ubyte2   led0_fb;
 * ubyte2   led1_fb;
 *
 * IO_LED_Set( IO_LED_00, TRUE, &led0_fb );      // turns LED on
 * IO_LED_Set( IO_LED_01, FALSE, &led1_fb );     // turns LED off
 * \endcode
 *
 *
 **************************************************************************/

#ifndef _IO_LED_H
#define _IO_LED_H

/***************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/
#include "IO_Driver.h"

/***************************************************************************
 *
 * D E F I N I T I O N S
 *
 **************************************************************************/

/***************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/***************************************************************************
 *
 * F U N C T I O N S
 *
 **************************************************************************/
/***********************************************************************//**
 *
 * \brief Setup one LED channel
 *
 * \param led_channel           LED channel, one of:
 *                                       - \c #IO_LED_00 ... \c #IO_LED_07
 *
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID          the channel id does not exist
 * \retval #IO_E_CHANNEL_BUSY                the ADC input channel is currently used by another function
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      The common driver init function has not been called before
 * \retval #IO_E_CH_CAPABILITY               The LED capability of this channel has not been activated
 *
 * \remarks
 *    - The following channels form groups. They have to be configured in the same mode within a group.
 *        - \c #IO_LED_00 .. \c #IO_LED_01
 *        - \c #IO_LED_02 .. \c #IO_LED_03
 *        - \c #IO_LED_04 .. \c #IO_LED_05
 *        - \c #IO_LED_06 .. \c #IO_LED_07
 *    - Check the alternate functions of the pins used in each group. A pin can only be configured for one function
 *    at a time and it has to be the same function within the group.The alternate functions can be found at \ref IO_Pins.h
 *
 ***************************************************************************/
IO_ErrorType IO_LED_ChannelInit( IO_PIN led_channel );

/***********************************************************************//**
 *
 * \brief Deinitializes one LED channel
 *
 * deinitializes the given LED channel, allows re-initialization by \c IO_LED_ChannelInit()
 *
 * \param led_channel           LED channel, one of:
 *                                       - \c #IO_LED_00 ... \c #IO_LED_07
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 *
 * \remarks
 *    - The following channels form groups. They have to be configured in the same mode within a group.
 *        - \c #IO_LED_00 .. \c #IO_LED_01
 *        - \c #IO_LED_02 .. \c #IO_LED_03
 *        - \c #IO_LED_04 .. \c #IO_LED_05
 *        - \c #IO_LED_06 .. \c #IO_LED_07
 *
 ***************************************************************************/
IO_ErrorType IO_LED_ChannelDeInit( IO_PIN led_channel );

/***********************************************************************//**
 *
 * \brief Sets the state of a LED channel
 *
 * \param led_channel               LED channel, one of:
 *                                      - \c #IO_LED_00 ... \c #IO_LED_07
 *
 * \param led_value                 Input value:
 *                                      - \c TRUE:  Turns LED on
 *                                      - \c FALSE: Turns LED off
 *
 * \param measurement               If the LED was turned on (led_value = TRUE) this parameter
 *                                  returns the actual current value (Range: 0...27.600mA)
 *                                  If the LED was turned off (led_value = FALSE) this parameter
 *                                  returns the actual voltage on the pin in mV (Range: 0...10.500V)
 *
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_NULL_POINTER                A NULL pointer has been passed
 * \retval #IO_E_INVALID_CHANNEL_ID          the channel id does not exist
 * \retval #IO_E_BUSY                        the channel is not settled so far (after switching a LED)
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED      the given channel is not configured
 * \retval #IO_E_ADC_CHANNEL_STARTUP         Channel is in initialization phase
 * \retval #IO_E_FET_PROTECTION              an overcurrent was detected
 * \retval #IO_E_ADC_INVALID                 The ADC value is invalid/not available
 * \retval #IO_E_INVALID_PARAMETER           invalid parameter has been passed
 *
 ***************************************************************************/
IO_ErrorType IO_LED_Set( IO_PIN led_channel
                       , bool led_value
                       , ubyte2 * measurement );


#endif /* _IO_LED_H */
