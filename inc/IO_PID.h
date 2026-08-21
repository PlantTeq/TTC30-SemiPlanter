/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_PID.h
 *
 * \brief Contains the data structure for configuring the PID controller
 *
 */

#ifndef _PID_H
#define _PID_H

/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/

#include "IO_Driver.h"

/**
 * Number maximal allowed PID handles.\n
 * In total 12 PID controllers are supported: 6 for Voltage Outputs and
 * 6 for current-controlled PWM outputs.
 *
 */
#define IO_PID_MAX_HANDLES      12U

#define IO_PID_INVALID_HANDLE   255U

/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/**
 *
 * \brief PID configuration structure
 *
 *  Data structure that contains all configuration parameters for PID control.\n
 *  The following formula is used for the PID:\n
 *  \code
 *  output = Kff/10^3 * setpoint + Kp/10^3 * error + Kd/10^4 * delta(error) + Ki/10^4 * sum(error)
 *  \endcode
 *
 *  Please note that Kff and Kp are scaled by a factor of 1,000 and Ki and
 *  Kd by a factor of 10,000 to improve the resolution on small gains.
 *
 *  \remarks
 *  - Typical values for Kff are between 10.000 and 15.000
 *  - Typical values for Ki are around 20.000
 *  - Typical values for Kp and Kd are smaller than 1.000
 *
 *  \attention
 *  Please keep in mind that the unit of \c output has to be
 *  - Volts for voltage outputs
 *  - Duty cycle in digits [0 .. 65535] for PWM outputs
 *
 *
 */
typedef struct _io_pid_config
{
    sbyte4 Kff;             /**< Gain constant for feed-forward control scaled by a factor
                                 of 1,000. A value of 0 disables the feed-forward
                                 control, a value of 1,000 leads to a 1:1 loop
                                 through of the setpoint. */
    sbyte4 Kp;              /**< Proportional gain constant scaled by a factor of 1,000.
                                 A value of 0 disables the proportional term in the PID. */
    sbyte4 Ki;              /**< Integral gain constant scaled by a factor of 10,000.
                                 A value of 0 disables the integral term in the PID. */
    sbyte4 Kd;              /**< Derivative gain constant scaled by a factor of 10,000.
                                 A value of 0 disables the derivative term in the PID. */
    sbyte4 max_limit;       /**< Upper boundary for output value. The output will
                                 saturate on this value. */
    sbyte4 min_limit;       /**< Lower boundary for output value. The output will
                                 saturate on this value. */
} IO_PID_CONFIG;


/**************************************************************************
 *
 * F U N C T I O N S
 *
 **************************************************************************/

/**********************************************************************//**
 *
 * \brief Sets the integral term of a PID controller
 *
 * Sets the integral term of a PID controller to a given value or freezes it.
 *
 * \param   pid_handle   handle of PID controller.
 * \param   integrator   new value of integral term; parameter is ignored
 *                       if \c hold is \c TRUE
 * \param   hold         TRUE to freeze the integral term to the current
 *                       value. Can be unfrozen by calling again with \c hold=FALSE
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_INVALID_PARAMETER   an invalid parameter has been passed
 *
 **************************************************************************/
IO_ErrorType IO_PID_SetIntegrator ( ubyte1 pid_handle
                                  , sbyte4 integrator
                                  , bool hold );


#endif /* _PID_H */
