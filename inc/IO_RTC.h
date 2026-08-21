/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_RTC.h
 *
 * \brief RTC functions, provides exact timing functions
 *
 * Provides setup and utility functions for the Real Time Clock.
 * The RTC is used for SW timings.
 *
 * The function \c IO_RTC_StartTime() returns a timestamp.
 * The function \c IO_RTC_GetTimeUS() returns the time which has passed
 * since the timestamp-value passed as an argument to this function.
 * The application can use these two functions as often as it needs to.
 * For different timing tasks only different timestamp variables need to be used.
 *
 * \section RTC code example:
 *
 * The example initializes the RTC driver, and implements a loop
 * which is executed every 5ms:
 *
 * \code{.c}
 *   ubyte4 time_stamp;
 *
 *   // RTC can be initialized:
 *   // by initializing the driver
 *   IO_Driver_Init ( IO_DRIVER_MODE_DEFAULT, NULL );
 *
 *   // or by only initializing the RTC module
 *   // IO_RTC_Init();
 *
 *   while (1)
 *   {
 *       IO_RTC_StartTime (&time_stamp);                 // start time (get timestamp)
 *
 *       task();      // user task function
 *
 *       while (IO_RTC_GetTimeUS (time_stamp) < 5000);   // wait until 5ms have passed
 *   }
 * \endcode
 *
 *
 **************************************************************************/
#ifndef _IO_RTC_H
#define _IO_RTC_H

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

#define IO_RTC_TICKS_PER_US  ((IO_DRIVER_SYSTEM_CLOCK) / 80)


/* typedef for function pointer for event handler of the periodic
 * interrupt handler
 */
typedef void (*rtc_eventhandler_ptr) (void);

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
 * \brief Initializes the RTC clock
 *
 * Initializes the RTC clock to a f_sys / 80.
 * For a system clock of 80MHz the RTC resolution is 1us
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                    everything fine
 * \retval #IO_E_CHANNEL_BUSY          the module has been initialized before
 *
 **************************************************************************
 *
 * \remarks
 *   Module is initialized only once.
 *
 * \remarks
 *   - The RTC driver is initialized when the \c IO_Driver_Init() function is called.
 *   Therefore it will return \c #IO_E_CHANNEL_BUSY if it is called after this
 *   function.
 *   This means that \c IO_RTC_Init() needs to be called only when \c IO_Driver_Init()
 *   is not used in the respective application.
 *
 **************************************************************************/
 IO_ErrorType IO_RTC_Init ( void );

/**********************************************************************//**
 *
 * \brief Returns an RTC timestamp
 *
 * Returns a timestamp which can be used for RTC timing functions
 *
 *
 * \param   timestamp   Pointer for the returned timestamp value
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                    everything fine
 * \retval #IO_E_NULL_POINTER          a null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED   the module has not been initialized
 *
 **************************************************************************/
 IO_ErrorType IO_RTC_StartTime ( ubyte4 * const timestamp );

/**********************************************************************//**
 *
 * \brief Returns the passed time
 *
 * The function returns the time in us which has passed since
 * the given timestamp has been taken
 * (via the function \c IO_RTC_StartTime() )
 *
 *
 * \param   timestamp   Timestamp received from a call of \c IO_RTC_StartTime()
 *
 * \return ubyte4
 *
 **************************************************************************
 *
 * \remarks
 *   - If the RTC module has not initialized, the function will return 0
 *
 * \remarks
 *   - Please keep in mind that the time between \c IO_RTC_StartTime() and
 *   \c IO_RTC_GetTimeUS() for one timestamp should not exceed 74min (overflow)
 *
 **************************************************************************/
 ubyte4 IO_RTC_GetTimeUS ( ubyte4 timestamp );

/**********************************************************************//**
 *
 * \brief Initializes the Periodic Timer
 *
 * Initializes a Periodic Timer
 *
 *
 * \param  period              Period on which the event handler should be called.
 *                             unit: us (500..65535)
 * \param  event_handler       Function pointer to the periodic event handler
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                   everything fine
 * \retval #IO_E_NULL_POINTER         a null pointer has been passed
 * \retval #IO_E_INVALID_PARAMETER    a invalid parameter has been passed
 * \retval #IO_E_BUSY                 the channel has been initialized before
 *
 **************************************************************************
 *
 * \remarks
 *   - This function can also be used without initializing the RTC driver
 *
 **************************************************************************/
 IO_ErrorType IO_RTC_PeriodicInit ( ubyte2 period
                                  , rtc_eventhandler_ptr event_handler );

/**********************************************************************//**
 *
 * \brief Deinitializes the Periodic Timer
 *
 * Deinitializes a Periodic Timer and stops it
 *
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_PERIODIC_NOT_CONFIGURED     the channel has not yet been initialized
 *
 **************************************************************************
 *
 * \remarks
 *   - This function can also be used without initializing the RTC driver
 *
 **************************************************************************/
 IO_ErrorType IO_RTC_PeriodicDeInit ( void );

#endif /* _IO_RTC_H */
