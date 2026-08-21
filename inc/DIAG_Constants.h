/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file DIAG_Constants.h
 *
 * \brief Global defines for IO Driver diagnostic state machine and WD
 *
 *      This header file defines the Error Codes for diagnostic state machine and WD.
 *
 **************************************************************************/

#ifndef _DIAG_CONSTANTS_H
#define _DIAG_CONSTANTS_H

/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/
#include "IO_Pins.h"
#include "ptypes_xe167.h"


/**************************************************************************
 *
 * D E F I N I T I O N S
 *
 **************************************************************************/

/**********************************************************************//**
 * \page diag_errors Diagnostic state machine error codes
 *
 * \anchor diag_err_values_table
 *
 * \brief Details about the errors of the diagnostic state machine.
 *
 * They are either returned by the #DIAG_Status(..) function or by the callback function (\c #DIAG_ERR_CALLBACK).
 *
 * The following table gives an overview of the error codes and their types.
 *
 * There are <b>four types of errors</b>:
 *  - <b>Fatal errors</b>\n
 *    If a fatal error is detected, the diagnostic state machine always activates the safe state.
 *  - <b>Permanent errors</b>\n
 *    For non-fatal errors the application can decide which action to take
 *    (via the error callback, see \c #DIAG_ERR_CALLBACK).
 *  - <b>Temporary fatal errors</b>\n
 *    For these errors a glitch filter (de-bounce) is implemented, which means that they are
 *    reported after the configured anti-glitch time. After the glitch filter time has expired,
 *    the error will be treated like a fatal error.
 *  - <b>temporary permanent errors</b>\n
 *    For these errors a glitch filter (de-bounce) is implemented, which means that they are
 *    reported after the configured anti-glitch time. After the glitch filter time has expired,
 *    the error will be treated like a permanent error.
 *
 *  For the error values please refer to \ref diag_err_values_doc "Diagnostic State Machine Error Values"
 *
 * \verbatim[table]
 * ==================================== ========== =============== ===================================================================== ==============================================
 *   Name                                 fatal?     temporary?      short description                                                     content of faulty value
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_NOERROR                       TRUE         FALSE        no error                                                              not available
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_ADC_LIMITS                    FALSE        TRUE         an ADC limit was exceeded or underrun                                 Measured voltage value in [mV]
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_ADC_5V2_SUPPLY                TRUE         TRUE         the 5.2V supply voltage deviates from its normal level                Measured supply voltage in [mV]
 *                                                                   4900mV - 5380mV
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_ADC_SENSOR_SUPPLY             FALSE        TRUE         the sensor supply voltage deviates from its normal level              Measured sensor supply voltage in [mV]
 *                                                                   4850mV - 5150mV
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_ADC_KL30_MAIN                 FALSE        TRUE         the KL30 voltage level exceeds or underruns its allowed limits        Measured battery voltage in [mV]
 *                                                                   7600mV - 33400mV
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_ADC_KL30_CPU                  TRUE         TRUE         the KL30_CPU voltage level exceeds or underruns its allowed limits    Measured battery voltage for the CPU in [mV]
 *                                                                   7600mV - 33400mV
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_OVER_TEMPERATURE              TRUE         TRUE         the board temperature exceeds its allowed range                       Board temperature in tenth degrees
 *                                                                   (the board temperature is 133 degree Celcius or higher)               Celcius.\n <i>Remark:</i> Faulty value is
 *                                                                                                                                         signed (two's complement)
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_USER_STACK                TRUE         FALSE        a memory block of the user stack is corrupted                         The faulty value which is written to the
 *                                                                                                                                         protected space of the stack memory
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_REGISTER                  TRUE         FALSE        internal register values are corrupted                                not available
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_DSRAM                     TRUE         FALSE        a memory block of the data S-RAM is corrupted                         The diverging value that has been detected
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_PSRAM                     TRUE         FALSE        a memory block of the program S-RAM is corrupted                      The diverging value that has been detected
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_DPRAM                     TRUE         FALSE        a memory block of the dual-port RAM is corrupted                      The diverging value that has been detected
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_ZeroFlag                  TRUE         FALSE        the zero flag was mistakenly raised during a math operation           Register PSW of the XC2000 CPU
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_CarryFlag                 TRUE         FALSE        the carry flag was mistakenly raised during a math operation          Register PSW of the XC2000 CPU
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_NegativeFlag              TRUE         FALSE        the negative flag was mistakenly raised during a math operation       Register PSW of the XC2000 CPU
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_OverflowFlag              TRUE         FALSE        the overflow flag was mistakenly raised during a math operation       Register PSW of the XC2000 CPU
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_SYS_STACK_OF              TRUE         FALSE        An overflow of the system stack occurred                              System stack pointer of the XC2000 CPU
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_SYS_STACK_UF              TRUE         FALSE        An underflow of the system stack occurred                             System stack pointer of the XC2000 CPU
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_SR0_TRAP                  TRUE         FALSE        A SR0 trap occurred and all interrupts have been disabled.            Register SCU.TRAPSTAT of the XC2000 CPU
 *                                                                   For further information about System-Request-0 traps consult
 *                                                                   the XC2000 user manual.
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_MEM_CLASS_B_TRAP              TRUE         FALSE        A class B trap occurred and all interrupts have been disabled.        Register TFR of the XC2000 CPU
 *                                                                   <i>Examples for class B traps are:</i>\n
 *                                                                   - Undefined Opcode\n
 *                                                                   - Memory Access Error\n
 *                                                                   - Illegal Word Operand Access\n\n
 *
 *                                                                   For further information about Class B traps consult the XC2000
 *                                                                   user manual.
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_FREQ_STARTUP                  TRUE         FALSE        Internal clock frequency drifts from its normal value                 Indication if the frequency is too high
 *                                                                                                                                         or too low
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_PWM_CURRENT_ZERO              FALSE        FALSE        Current measurement greater zero during startup phase                 Measured current in \n
 *                                                                                                                                         [digits * number_of_samples] or 0xFFFF\n
 *                                                                                                                                         if a synchronization error between driver\n
 *                                                                                                                                         and checker module has been detected.
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_PWM_CURRENT_OFFSET            FALSE        FALSE        Offset of current measurement circuitry on PWM output not within      Offset of the current measurement
 *                                                                   limits                                                                circuitry stored in the PDB in\n
 *                                                                                                                                         [digits * number_of_samples]. This value\n
 *                                                                                                                                         is scaled to the number of samples taken\n
 *                                                                                                                                         for one measurement.
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_PWM_LIMITS_RANGE              FALSE        TRUE         Pulse width not within range on PWM output (outside min/max pulse)    Read duty cycle in [us]
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_PWM_LIMITS_TOL                FALSE        TRUE         Pulse width not within tolerance window on PWM output                 Difference between set and read duty cycle
 *                                                                                                                                         in [us]
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_PWM_PERIOD_MISMATCH           FALSE        TRUE         Period mismatch on PWM output                                         Difference between set and read period
 *                                                                                                                                         in [us]
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_PWM_CURRENT                   FALSE        TRUE         Current not within limits on current controlled input                 Difference between set and read current
 *                                                                                                                                         in [mA]
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_PWM_CURRENT_DEAD_TIME         FALSE        FALSE        Set current not reached after dead time elapsed                       Difference between set and read current
 *                                                                                                                                         in [mA]
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_PWM_CURRENT_OFFS_DRIFT        FALSE        TRUE         Current offset too low (due to drift or HW defect)                    Measured raw current in\n
 *                                                                                                                                         [digits * number_of_samples]
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_PWD_LIMITS_FREQ               FALSE        TRUE         Frequency limit error on timer input or counter limit error in        Measured frequency value in [Hz] or
 *                                                                   case of incremental/counter input                                     counter value in [digits]
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_PWD_LIMITS_PULSE_WIDTH        FALSE        TRUE         Pulse width limit error on timer input                                Measured pulse-width in [us]. This faulty
 *                                                                                                                                         value is saturated to 0xFFFF
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_CYCLE_TIME                    FALSE        FALSE        Cycle time too high                                                   Measured cycle time in [us]. This faulty
 *                                                                                                                                         value is saturated to 0xFFFF
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_RPP                           FALSE        TRUE         Insufficient gate drive on reverse polarity protection.\n             Difference between Urpp and Ubat in [V].\n
 *                                                                   <i>Further Reasons for occurrence:</i>\n                               (Urpp .. Gate drive of reverse polarity
 *                                                                   - Clamp 30 voltage is not connected to ECU\n                          protection,\n Ubat .. Supply voltage for
 *                                                                   - Driver is used with external safety switch but no PWM is            output stages)
 *                                                                     configured safety critical
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_EXT_WD                        TRUE         FALSE        External WD has activated the safe state                              Content of flag ESR1T in register
 *                                                                                                                                         SCU.DMPMIT of the XC2000 CPU
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_LS_PROT                       FALSE        TRUE         Over-current condition on safety switch                               Error code (#IO_ErrorType) set by digital
 *                                                                                                                                         output driver
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_OVD_STARTUP                   TRUE         FALSE        Over voltage detector startup test has failed\n                       Internal state information (state in which
 *                                                                   <i>Further Reasons for occurrence:</i>\n                               an error occurred)
 *                                                                   - Hardware over-current protection is tripped due to a
 *                                                                     short battery condition on the low-side digital outputs
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_OVD                           TRUE         FALSE        Over voltage detection has activated the safe state                   Read status value from OVD circuitry
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_SAFETY_SW_INT                 TRUE         FALSE        Safety switch check error (internal switch)\n                         Internal state information (state in which
 *                                                                   <i>Further Reasons for occurrence:</i>\n                               an error occurred)
 *                                                                   - Improper wiring\n
 *                                                                   - Improper safety switch setting for safety-critical PWM output\n
 *                                                                   - Driver has been configured for use with an external safety
 *                                                                     switch but an internal one or no safety switch is wired
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_SAFETY_SW_EXT                 TRUE         FALSE        Safety switch check error (external switch)\n                         Internal state information (state in which
 *                                                                   <i>Further Reasons for occurrence:</i>\n                               an error occurred)
 *                                                                   - Improper wiring of external safety switch\n
 *                                                                   - Improper safety switch setting for safety-critical PWM output\n
 *                                                                   - Driver has been configured for internal safety switch
 *                                                                     but an external one is wired
 *
 *                                                                   \attention
 *                                                                   The startup test for the external Safety Switch runs in a \n
 *                                                                   blocking manner when being already in the cyclic execution phase \n
 *                                                                   (when the application software is already running). The startup \n
 *                                                                   test will violate the cycle time that is defined by the \n
 *                                                                   application software.
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_SAFETY_SW_SHUT_OFF            FALSE        FALSE        An error while checking the PWM feedback shut off path of             Internal state information (state in which
 *                                                                   the safety switches occurred                                          an error occurred)
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_INIT_ERROR                    TRUE         FALSE        Error during initialization of diagnostic state machine               IO-Driver error code
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_INVALID_DIAG_STATE            TRUE         FALSE        Invalid diagnostic state                                              Content of state variable which holds the
 *                                                                                                                                         diagnostic state
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_INVALID_STARTUP_STATE         TRUE         FALSE        Invalid diagnostic state in configuration state                       Content of state variable which holds the
 *                                                                                                                                         diagnostic state
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_INVALID_MAIN_STATE            TRUE         FALSE        Invalid diagnostic state in main state                                Content of state variable which holds the
 *                                                                                                                                         diagnostic state
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_WD_STARTUP                    TRUE         FALSE        Watchdog startup test has failed
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_SR_LowNibble                  TRUE         TRUE         The feedback value of the low nibble of the
 *                                                                   shift register is faulty
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_SR_HighNibble                 TRUE         TRUE         The feedback value of the high nibble of the
 *                                                                   shift register is faulty
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_TIMEOUT                       TRUE         FALSE        A timeout between Diagnostic Module and IO-Driver has occurred\n       not available
 *                                                                   (e.g. driver functions of safety critical IO not called)
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_APPL_SAFE_STATE               TRUE         FALSE        Application requested to activate the safe state                       not available
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_PLL_VCO_NOT_LOCKED            TRUE         FALSE        The PLL/VCO lost its lock to the oscillator frequency.\n              Content of CPU register PLLSTAT
 *                                                                   (e.g. if the oscillator is damaged or not connected to the CPU
 *                                                                   anymore)
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_SW_INTERNAL                   TRUE         FALSE        Internal SW error detected. May be SW or HW related.                   not available
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_INIT_ERROR                    TRUE         FALSE        Error during initialization of diagnostic state machine                Error code #IO_E_SW_INTERNAL
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 *   #DIAG_E_INT_WATCHDOG                  TRUE         FALSE        Internal Watchdog has not been serviced in time - WD reset             Content of CPU register RSTSTAT1
 *                                                                   occurred.
 * ------------------------------------ ---------- --------------- --------------------------------------------------------------------- ----------------------------------------------
 * \endverbatim
 **************************************************************************/


/*
 * D I A G N O S T I C   S T A T E   M A C H I N E   E R R O R   V A L U E S
 */

/**
 * \name Diagnostic State Machine Error Values
 * \anchor diag_err_values_doc
 *
 * Errors codes that the function DIAG_Status returns in parameter
 * diag_error.error_code. They are also used for the error callback function.
 *
 * \remark <b>For a detailed description see </b>
 * \ref diag_err_values_table "Listing of diagnostic errors"
 */

/*
 * notice: when changing this type definition please make sure to
 * also check the table diag_err_type_table in DIAG_INT_ErrorHandler.c
 */
/**
 * <b>For a detailed description see </b>
 * \ref diag_err_values_table "Listing of diagnostic errors"
 */
typedef enum _diag_errortype
{
      DIAG_E_NOERROR                = 0     /**< no error */
    , DIAG_E_ADC_LIMITS             = 1     /**< an ADC limit was exceeded or underrun */
    , DIAG_E_ADC_5V2_SUPPLY         = 2     /**< the 5.2V supply voltage deviates from its normal level */
    , DIAG_E_ADC_SENSOR_SUPPLY      = 3     /**< the sensor supply voltage deviates from its normal level */
    , DIAG_E_ADC_KL30_MAIN          = 4     /**< the KL30 voltage level exceeds or underrun its allowed limits */
    , DIAG_E_ADC_KL30_CPU           = 5     /**< the KL30_CPU voltage level exceeds or underrun its allowed limits */
    , DIAG_E_OVER_TEMPERATURE       = 6     /**< the board temperature exceeds its allowed range */
    , DIAG_E_MEM_USER_STACK         = 7     /**< a memory block of the user stack is corrupted */
    , DIAG_E_MEM_REGISTER           = 8     /**< internal register values are corrupted */
    , DIAG_E_MEM_DSRAM              = 9     /**< a memory block of the data S-RAM is corrupted */
    , DIAG_E_MEM_PSRAM              = 10    /**< a memory block of the program S-RAM is corrupted */
    , DIAG_E_MEM_DPRAM              = 11    /**< a memory block of the dual-port RAM is corrupted */
    , DIAG_E_MEM_ZeroFlag           = 12    /**< the zero flag was mistakenly raised during a math operation */
    , DIAG_E_MEM_CarryFlag          = 13    /**< the carry flag was mistakenly raised during a math operation */
    , DIAG_E_MEM_NegativeFlag       = 14    /**< the negative flag was mistakenly raised during a math operation */
    , DIAG_E_MEM_OverflowFlag       = 15    /**< the overflow flag was mistakenly raised during a math operation */
    , DIAG_E_MEM_SYS_STACK_OF       = 16    /**< an overflow of the system stack occurred */
    , DIAG_E_MEM_SYS_STACK_UF       = 17    /**< an underflow of the system stack occurred */
    , DIAG_E_MEM_SR0_TRAP           = 18    /**< a SR0 trap occurred and all interrupts have been disabled */
    , DIAG_E_MEM_CLASS_B_TRAP       = 19    /**< a class B trap occurred and all interrupts have been disabled */
    , DIAG_E_PWM_CURRENT_ZERO       = 20    /**< Current measurement greater zero during startup phase */
    , DIAG_E_PWM_CURRENT_OFFSET     = 21    /**< Offset of current measurement circuitry on PWM output not within limits */
    , DIAG_E_PWM_LIMITS_RANGE       = 22    /**< Pulse width not within range on PWM output (outside min/max pulse) */
    , DIAG_E_PWM_LIMITS_TOL         = 23    /**< Pulse width not within tolerance window on PWM output */
    , DIAG_E_PWM_PERIOD_MISMATCH    = 24    /**< Period mismatch on PWM output */
    , DIAG_E_PWM_CURRENT            = 25    /**< Current not within limits on current controlled input */
    , DIAG_E_PWM_CURRENT_DEAD_TIME  = 26    /**< Set current not reached after dead time elapsed */
    , DIAG_E_PWM_CURRENT_OFFS_DRIFT = 27    /**< Current offset too low (due to drift or HW defect) */
    , DIAG_E_PWD_LIMITS_FREQ        = 28    /**< Frequency limit error on timer input or counter error on incremental/counter input */
    , DIAG_E_PWD_LIMITS_PULSE_WIDTH = 29    /**< Pulse width limit error on timer input */
    , DIAG_E_CYCLE_TIME             = 30    /**< Cycle time too high */
    , DIAG_E_RPP                    = 31    /**< Insufficient gate drive on reverse polarity protection. */
    , DIAG_E_EXT_WD                 = 32    /**< External WD has activated the safe state */
    , DIAG_E_LS_PROT                = 33    /**< Over-current condition on safety switch */
    , DIAG_E_OVD_STARTUP            = 34    /**< Over voltage startup test has failed */
    , DIAG_E_OVD                    = 35    /**< Over voltage detection has activated the safe state */
    , DIAG_E_SAFETY_SW_INT          = 36    /**< Safety switch check error (internal switch) */
    , DIAG_E_SAFETY_SW_EXT          = 37    /**< Safety switch check error (external switch) */
    , DIAG_E_SAFETY_SW_SHUT_OFF     = 38    /**< Safety switch check failed during start-up */
    , DIAG_E_INVALID_DIAG_STATE     = 39    /**< Invalid diagnostic state */
    , DIAG_E_INVALID_STARTUP_STATE  = 40    /**< Invalid diagnostic state in startup state */
    , DIAG_E_INVALID_MAIN_STATE     = 41    /**< Invalid diagnostic state in main state */
    , DIAG_E_WD_STARTUP             = 42    /**< Watchdog startup test has failed */
    , DIAG_E_SR_LowNibble           = 43    /**< The feedback value of the low nibble of the shift register is faulty */
    , DIAG_E_SR_HighNibble          = 44    /**< The feedback value of the high nibble of the shift register is faulty */
    , DIAG_E_FREQ_STARTUP           = 45    /**< Internal clock frequency drifts from its normal value */
    , DIAG_E_TIMEOUT                = 46    /**< A timeout between Diagnostic Module and IO-Driver has occurred (f.ex driver functions of safety critical IO not called) */
    , DIAG_E_APPL_SAFE_STATE        = 47    /**< Application requested to activate the safe state */
    , DIAG_E_PLL_VCO_NOT_LOCKED     = 48    /**< The PLL/VCO lost its lock to the oscillator frequency. */
    , DIAG_E_SW_INTERNAL            = 49    /**< Internal SW error detected. Maybe SW or HW related. */
    , DIAG_E_INIT_ERROR             = 50    /**< Error during initialization of diagnostic state machine */
    , DIAG_E_INT_WATCHDOG           = 51    /**< Internal Watchdog has not been serviced in time - WD reset occurred */
    , DIAG_E_MEM_SOFTBREAK_TRAP     = 52    /**< a SOFTBREAK trap occurred (software triggered) */
    , DIAG_E_MAX_ERRORCODES         = 53
}DIAG_ErrorType;


/*
 * D I A G N O S T I C   S T A T E   M A C H I N E   S T A T E S
 */

/**
 * \name States of the diagnostic state machine
 * \anchor diag_states
 *
 * State information returned by the function DIAG_Status
 */
/*\{*/
#define DIAG_STATE_INIT                     0x03    /**< Diagnostic state machine is in its initial state (safety-outputs not operational) */
#define DIAG_STATE_STARTUP                  0x05    /**< Diagnostic state machine is in startup state performing startup tests (safety-outputs not operational) */
#define DIAG_STATE_MAIN                     0x06    /**< Diagnostic state machine is in main state performing runtime tests (safety-outputs operational) */
#define DIAG_STATE_SAFE_STATE               0x09    /**< Diagnostic state machine is in safe state due to errors that have been detected (safety-outputs not operational) */
#define DIAG_STATE_DISABLED                 0x00    /**< Diagnostic state machine is disabled. This means the IO-Driver has been configured NON-SAFETY */
/*\}*/


/**
 * \name error callback reaction
 * \anchor diag_err_cb_reaction
 *
 * allowed return values of error callback function
 */
/*\{*/
#define DIAG_ERR_NOACTION       0x1            /**< take no action (ignore the error)               */
#define DIAG_ERR_SAFESTATE      0x9            /**< enter the safe state (switch off all outputs)   */
/*\}*/

/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/**
 *
 * \brief Diagnostic Error code structure
 *
 *  Stores all relevant error parameters returned from the diagnostic
 *  state machine or returned from the WD
 *
 */
typedef struct _diag_errorcode
{
    DIAG_ErrorType  error_code;          /**< error code: for a detailed description of the diagnostic error codes see 
                                           *  \ref diag_err_values_table "Listing of diagnostic errors"                     */
    IO_PIN          device_num;          /**< device number which caused error   */
    ubyte2          faulty_value;        /**< value which caused the error       */
} DIAG_ERRORCODE;

/**
 * \brief Error callback to be used by application software
 */
typedef ubyte1 (* DIAG_ERR_CALLBACK)( ubyte1 diagnostic_state, DIAG_ERRORCODE * error_code );

#endif /* _DIAG_CONSTANTS_H */
