/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 *
 * \file IO_WD.h
 *
 * \brief IO-Driver for the Window Watchdog
 *
 * Contains a function to service the external Watchdog.\n
 * The Window Watchdog needs no initialization, at it is initialized
 * automatically by #IO_Driver_Init.
 *
 *
 **************************************************************************/

#ifndef _IO_WD_H
#define _IO_WD_H

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
 * \brief Services the Window Watchdog.
 *
 * This function services the CPU-external Window Watchdog.
 * If the flag #IO_DRIVER_MODE_SERVICE_WD was not set on IO-Driver configuration
 * (see \ref IO_Driver.h), this function must be called
 * at least every 40ms, otherwise the Watchdog will deactivate the low-side digital
 * outputs.\n
 *
 * \remarks
 * - The Window Watchdog also has a lower time limit of 2ms, i.e. it
 *   must not be serviced with a period shorter than 2ms. This function
 *   handles the lower boundary automatically, that is, if it's called more frequent
 *   than 2ms it does only service the Watchdog if 2ms have passed.
 *
 * \remarks
 * - Even if #IO_DRIVER_MODE_SERVICE_WD was set as mode on IO-Driver initialization,
 *   this function can be called. This can be useful if some operations (e.g. during
 *   initialization phase) take more time than 40ms, but the automatic servicing feature
 *   shall be used.
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                             everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED         module has not been initialized
 * \retval #IO_E_WD_TRIGGER_DISABLED            trigger has been disabled permanently
 * \retval #IO_E_SW_INTERNAL                    a internal software error occured
 *
 **************************************************************************/
IO_ErrorType IO_WD_Service( void );


#endif /* _IO_WD_H */
