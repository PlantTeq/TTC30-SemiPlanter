/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_WDTimer.h
 *
 * \brief IO Driver functions for the CPU's Watchdog timer
 *
 *  The Watchdog Timer (WDT) is a secure mechanism to overcome
 *  life- and dead-locks.
 *  An enabled WDT generates a reset for the system if not
 *  serviced in a configured time frame.
 *
 * \section wdt_remark Remark
 *  After the first watchdog timer overrun (ie. when the watchdog
 *  timer is not serviced within the configured timeout),
 *  the CPU will be reset and the application starts again.
 *  After the second overrun the CPU will be held in a reset
 *  state to avoid endless resets.
 *
 *  \note These IO Driver functions are only available if IO_Driver_Init() is called without safety configuration i.e.: IO_Driver_Init(IO_DRIVER_MODE_DEFAULT, NULL);
 *
 * \section wdt_examples Watchdog timer code examples
 *  Example for watchdog timer initialization:
 *  \code
 *  IO_WDTimer_Init(350000); //setup the wdtimer with a timeout of 350ms
 *  \endcode
 *
 *  Example for watchdog timer service:
 *  \code
 *  IO_WDTimer_Service(); //service the watchdog timer
 *  \endcode
 *
 **************************************************************************/
#ifndef _IO_WDTIMER
#define _IO_WDTIMER

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
#define IO_WDTIMER_TIMEOUT_MAX  13421772
#define IO_WDTIMER_TIMEOUT_MIN  3

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
 * \brief Initialization of the Watchdog Timer
 *
 *   The function
 *      - configures the timeout of the watchdog timer
 *      - enables the timer
 *
 *
 * \param   timeout         timeout for the watchdog timer in us
 *                          the CPU will be reseted if the watchdog
 *                          timer is not serviced within this period
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_BUSY                Module already initialized
 * \retval #IO_E_INVALID_PARAMETER   Given parameter is invalid
 * \retval #IO_E_WD_INT_ONLY_NON_SAFETY  Watchdog already initialized via IO_Driver_Init()
 *
 **************************************************************************/
IO_ErrorType IO_WDTimer_Init( ubyte4 timeout );

/**********************************************************************//**
 *
 * \brief Disable the Watchdog Timer
 *
 *   disables the watchdog timer functionality.
 *
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK
 *         everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED   internal Watchdog not initialized
 * \retval #IO_E_WD_INT_ONLY_NON_SAFETY   IO_Driver is safety relevant configured, therefore it's not allowed to disabled the internal Watchdog
 *
 **************************************************************************/
IO_ErrorType IO_WDTimer_DeInit( void );

/**********************************************************************//**
 *
 * \brief Service the Watchdog timer
 *
 *   Service routine for the watchdog timer.
 *   If the time between two Service function calls exceeds
 *   the timeout (set with IO_WDTimer_Init) the CPU will be resetted.
 *
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                       everything fine
 * \retval #IO_E_WD_INT_ONLY_NON_SAFETY   Watchdog is serviced automatically
 *
 **************************************************************************/
IO_ErrorType IO_WDTimer_Service( void );

#endif /* _IO_WDTIMER */
