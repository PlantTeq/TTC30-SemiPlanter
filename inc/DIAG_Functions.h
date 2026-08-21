/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file DIAG_Functions.h
 *
 * \brief Auxiliary functions for the diagnostic state machine
 *
 *      Provides the interface to the diagnostic state machine
 *
 **************************************************************************/
#ifndef _DIAG_FUNCTIONS_H
#define _DIAG_FUNCTIONS_H

/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/

#include "IO_Driver.h"
#include "IO_Constants.h"
#include "DIAG_Constants.h"

/**************************************************************************
 *
 * D E F I N I T I O N S
 *
 **************************************************************************/

/**
 * \brief control modes for blocking startup tests
 */
typedef enum
{
      DIAG_STARTUP_TEST_INHIBIT     /**< inhibit startup tests */
    , DIAG_STARTUP_TEST_ACTIVATE    /**< activate startup tests */
}DIAG_STARTUP_TEST_CTRL;

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
 * \brief status function for diagnostic state machine
 *
 *      Returns the current satus as well as the error codes of
 *      the diagostic state machine and the watchdog CPU.
 *
 *
 * \param   diag_state      current state of the diagnostic state machine
 * \param   diag_error      error codes of the diagnostic state machine
 *
 * \return #IO_ErrorType
 * \retval #IO_E_NULL_POINTER   null pointer has been passed
 * \retval #IO_E_OK             everything fine
 *
 **************************************************************************/
 IO_ErrorType DIAG_Status ( ubyte1 * diag_state
                          , DIAG_ERRORCODE * diag_error );

 /**********************************************************************//**
  *
  * \brief allows an application driven safe state
  *
  *      When this function is called the diagnostic state machine
  *      enters the safe state.
  *
  *
  * \return IO_ErrorType
  * \retval #IO_E_OK                            everything fine
  * \retval #IO_E_CHANNEL_NOT_CONFIGURED        Diagnostic state machine still in init state
  * \retval #IO_E_ECU_ALREADY_IN_SAFE_STATE     ECU is already in safe state
  *
  **************************************************************************/
 IO_ErrorType DIAG_EnterSafestate ( void );

 /**********************************************************************//**
  *
  * \brief Inhibits the startup tests
  *
  * \param ctrl    Control command for inhibiting/activating startup tests
  *
  * \return IO_ErrorType
  * \retval #IO_E_OK                   Everything fine
  * \retval #IO_E_INVALID_PARAMETER    Invalid parameter has been passed to this function
  * \retval #IO_E_UNKNOWN              Function has not been called before startup
  *                                    tests are being executed
  *
  **************************************************************************
  *
  * \remark
  *   This function needs to be called before the startup test are being
  *   executed in order to inhibit them.\n
  *   After the startup test are inhibited, this function has to be called
  *   once more in order to activate the startup tests.
  *
  **************************************************************************/
 IO_ErrorType DIAG_StartupTestCtrl( DIAG_STARTUP_TEST_CTRL ctrl );

 /**********************************************************************//**
  *
  * \brief Enables the usage of the integrated discharge circuit (on ECU HW V5.00)
  *
  * \return IO_ErrorTpye
  * \retval #IO_E_OK                  Everything fine
  * \retval #IO_E_CHANNEL_BUSY        Discharge circuit has been already enabled
  * \retval #IO_E_DRIVER_INITIALIZED  IO_Driver_Init() has been already called
  *
  * \remark
  *   - This function has only influence on ECUs that provide an integrated
  *     discharge circuit (ECU HW V5.00).
  *   - This function needs to be called before IO_Driver_Init() in order to
  *     enable the discharge circuit.
  *
  **************************************************************************/
 IO_ErrorType DIAG_EnableDischargeCircuit( void );


#endif /* _DIAG_FUNCTIONS_H */

