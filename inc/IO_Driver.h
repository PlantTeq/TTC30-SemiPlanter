/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_Driver.h
 *
 * \brief High level interface to IO Driver
 *
 * The IO Driver high level interface provides a general
 * initialization function, a version API and general task functions
 * which shall wrap the whole user application.
 *
 * \section appl_structure Basic structure of an application
 *
 * The IO Driver API provides two different types of functions:
 * - Initialization functions:
 *         These functions are designed to be called once at the
 *         beginning of an application.
 * - Task functions:
 *         These functions are designed to be called periodically at
 *         runtime.
 *
 * The function IO_Driver_Init() needs to be the first function, called
 * during the initialization.
 *
 * All safety critical IO initialization functions need to be called before
 * the first call of IO_Driver_TaskBegin().
 *
 * All task functions need to be enclosed by the functions
 * IO_Driver_TaskBegin() and IO_Driver_TaskEnd()
 *
 * Example of an application:
 *
 * \code
 * void task (void)
 * {
 *     IO_Driver_TaskBegin();
 *
 *     // User Application
 *     // and calls to driver task functions.
 *
 *     IO_Driver_TaskEnd();
 * }
 *
 *
 * void main (void)
 * {
 *     ubyte4 timestamp;
 *
 *     //--------------------------------//
 *     // start of driver initialization //
 *     //--------------------------------//
 *
 *     // IO_Driver_Init() is the first function:
 *     IO_Driver_Init( IO_DRIVER_MODE_SERVICE_WD, NULL );     // configure automatic WD servicing
 *
 *     //------------------------------//
 *     // end of driver initialization //
 *     //------------------------------//
 *
 *     //--------------------------------------------//
 *     // start of safety critical IO initialization //
 *     //--------------------------------------------//
 *
 *     // initialize all the safety critical IO functions
 *
 *     //------------------------------------------//
 *     // end of safety critical IO initialization //
 *     //------------------------------------------//
 *
 *     //--------------------------------------------//
 *     // from now on only task functions are called //
 *     //--------------------------------------------//
 *     while (1){
 *         IO_RTC_StartTime(&timestamp);
 *
 *         task();
 *
 *         while (IO_RTC_GetTimeUS(timestamp) < 5000);
 *     }
 * }
 *
 * \endcode
 *
 * The \c task function is called every 5000us = 5ms. Please
 * refer to the \ref IO_RTC.h "Real Time Clock" documentation for
 * details on how to use the RTC functions.
 *
 **************************************************************************/

/**********************************************************************//**
 * \page ecu_map ECU Map
 * \brief Description and properties of the ECU
 * \section ecu_properties_section ECU properties
 *
 * The HY-TTC 30 ECU is equipped with an Infineon XC2267 microcontroller which
 * is driven at a CPU frequency of 80MHz.
 *
 * \section memory_map_section Memory Map
 *
 * The HY-TTC 30 provides different memory types. The following table
 * shows which memories and sizes are available:
 *
 * \verbatim[table]
 * ==================================== =============
 *  Memory                               Size
 * ------------------------------------ -------------
 *  EEPROM                               8128 Byte
 * ------------------------------------ -------------
 *  internal Flash                       768 kByte
 * ------------------------------------ -------------
 *  internal RAM                         82 kByte
 * ------------------------------------ -------------
 * \endverbatim
 *
 **************************************************************************/

/**********************************************************************//**
 * \page safety_examples Implementation Examples for Safety Functions
 * \brief Example for using the IO-Driver in a safety critical environment
 * \section safety_cb_example Example implementation for Safety-Callback
 *
 * See #DIAG_ERR_CALLBACK for details on the safety callback function.
 *
 * For further details on error types and how they are treated by the
 * diagnostic state machine, refer to
 *  \ref diag_err_values_table "Diagnostic state machine error codes".
 *
 * \code
 * static ubyte1 APPL_SafetyCb( ubyte1 diag_state
 *                            , DIAG_ERRORCODE * const error )
 * {
 *     ubyte1 action;
 *
 *     // Error codes can for example be saved to error memory of application software
 *     // (APPL_WriteErrMem is a hypothetical function of the application software).
 *     APPL_WriteErrMem( diag_state, error );
 *
 *     // Just a simple example which does
 *     // not take the type of error into account
 *     switch (error->device_num)
 *     {
 *         case IO_ADC_00:
 *         case IO_ADC_01:
 *             // ...
 *             // Do something sophisticated
 *             // ...
 *             action = DIAG_ERR_NOACTION;
 *             break;
 *         case IO_PWM_00:
 *         case IO_PWM_01:
 *         case IO_PWM_02:
 *         case IO_PWM_03:
 *             // ...
 *             // Do something sophisticated
 *             // ...
 *             action = DIAG_ERR_SAFESTATE;
 *             break;
 *         case IO_PWM_04:
 *         case IO_PWM_05:
 *         case IO_PWM_10:
 *         case IO_PWM_11:
 *             // ...
 *             // Do something sophisticated
 *             // ...
 *             action = DIAG_ERR_NOACTION;
 *             break;
 *         default:
 *             action = DIAG_ERR_SAFESTATE;
 *             break;
 *     }
 *
 *     return action;
 * }
 *
 * \endcode
 *
 * \section safety_step_example Example for calling PWM step functions for safety critical PWM outputs to explain how to react on certain error codes of the step functions:
 * \code
 * void APPL_PWM_ControlOutput( void )
 * {
 *     bool pwm_current_new_00;
 *     ubyte2 pwm_current_00;
 *     ubyte2 duty;
 *     IO_ErrorType rc_pwm_00;
 *     IO_ErrorType rc_pwm_current_00;
 *
 *     rc_pwm_current_00 = IO_PWM_GetCur( IO_PWM_00, &pwm_current_00, &pwm_current_new_00 );
 *     // Application controller for PWM (hypothetical user function)
 *     duty = APPL_PWM_PidCtrl( IO_PWM_00, pwm_current_00, pwm_current_new_00, rc_pwm_current_00 );
 *
 *     // set duty cycle
 *     rc_pwm_00 = IO_PWM_SetDuty( IO_PWM_00, duty, NULL );
 *
 *     switch (rc_pwm_00)
 *     {
 *         case IO_E_OK:
 *             // Everything fine
 *             break;
 *         case IO_E_PWM_SHORT_CIRCUIT:
 *             // Application code
 *             break;
 *         case IO_E_PWM_OPEN_LOAD_OR_SHORT_BATTERY:
 *              // Application code
 *             break;
 *         case IO_E_PWM_SHORT_BATTERY:
 *              // Application code
 *              break;
 *         case IO_E_PWM_OPEN_LOAD:
 *              // Application code
 *             break;
 *         case ...
 *             // Application code
 *             break;
 *         default:
 *              // Unexpected error code
 *             break;
 *     }
 * }
 *
 * \endcode
 *
 * \section safety_driver_example Example for safety critical Driver configuration
 * \code
 *
 * // Safety configuration for IO-Driver
 * static const  IO_DRIVER_SAFETY_CONF c_driver_safety_conf =
 * {
 *           .safety_switch_type = IO_DRIVER_SAFETY_SWITCH_INT
 *         , .glitch_filter_time = 40    // [ms]
 *         , .cycle_time = 11000         // [us] -> task cycle time plus tolerance
 *         , .error_callback = &APPL_SafetyCb // (Set to NULL if IO-Driver should decide what to do on errors)
 * };
 *
 * // Safety configuration for PWM Output
 * static const IO_PWM_SAFETY_CONF c_pwm_safety_conf =
 * {
 *       IO_SAFETY_SWITCH_0
 *     , 80
 *     , 100
 *     , 200
 * };
 *
 *
 * void task (void)
 * {
 *     IO_ErrorType rc_driv_begin;
 *     IO_ErrorType rc_driv_end;
 *
 *     rc_driv_begin = IO_Driver_TaskBegin();
 *     if (rc_driv_begin != IO_E_OK)
 *     {
 *         // User code
 *     }
 *
 *     // User Application
 *     // and calls to driver task functions.
 *     APPL_PWM_ControlOutput();
 *
 *     rc_driv_end = IO_Driver_TaskEnd();
 *     if (rc_driv_end != IO_E_OK)
 *     {
 *         // User code
 *     }
 * }
 *
 *
 * void main (void)
 * {
 *     ubyte4 timestamp;
 *     IO_ErrorType rc_driv_init;
 *     IO_ErrorType rc_pwm_init;
 *
 *     //--------------------------------//
 *     // start of driver initialization //
 *     //--------------------------------//
 *
 *     // IO_Driver_Init() is the first function:
 *     rc_driv_init = IO_Driver_Init( IO_DRIVER_MODE_SERVICE_WD, &c_driver_safety_conf );  // safety critical application
 *     if (rc_driv_init != IO_E_OK)
 *     {
 *         // User code
 *     }
 *
 *     // Initialization of PWM Output, 200Hz
 *     rc_pwm_init = IO_PWM_Init( IO_PWM_00, 200, TRUE, TRUE, 0, &c_pwm_safety_conf);
 *     if (rc_pwm_init != IO_E_OK)
 *     {
 *         // User code
 *     }
 *
 *     //------------------------------//
 *     // end of driver initialization //
 *     //------------------------------//
 *
 *
 *     //--------------------------------------------//
 *     // from now on only task functions are called //
 *     //--------------------------------------------//
 *     while (1){
 *         IO_RTC_StartTime(&timestamp);
 *
 *         task();
 *
 *         while (IO_RTC_GetTimeUS(timestamp) < 10000);
 *     }
 * }
 * \endcode
 *
 * \section safety_switch_example Example for manual safety switch test
 * This example shows how to perform a manual safety switch test (for both internal and external safety switch configuration), which is necessary in case safety critical configured PWM outputs are applied with loads <b>after</b> the <b>ECU startup</b>.
 * \code
 *
 * // number of safety configured PWM outputs //
 * #define SAFETY_PWM_NUM  2
 * // number of software cycle to elapse before the return value from IO_PWM_SetDuty is plausible //
 * #define SSW_CYCLES      5
 *
 * // status for safety critical PWM outputs //
 * typedef struct pwm_status
 * {
 *     IO_PIN pwm_pin;
 *     IO_ErrorType pwm_set_rc;
 *     ubyte2 duty_cycle;
 *
 *     // flag that indicates if the respective PMW output has to be tested with a manual safety switch test again //
 *     bool pwm_man_ssw_test;
 *
 * } PWM_STATUS;
 *
 * // states of manual safety switch test //
 * typedef enum { SSW_START,
 *                SSW_DISABLED,
 *                SSW_ENABLED,
 *                SSW_WAIT,
 *                SSW_DONE
 * } SSW_STATE;
 *
 * void Man_SSW_Test (IO_PIN safety_switch);
 *
 * static IO_ErrorType driver_init_rc;
 * static IO_ErrorType driver_task_begin_rc;
 * static IO_ErrorType driver_task_end_rc;
 * static IO_ErrorType pwm_init_rc;
 * static IO_ErrorType power_rc;
 * static IO_ErrorType diag_rc;
 * static DIAG_ERRORCODE diag_error;
 * static ubyte1 diag_state;
 *
 *
 * // insert ALL safety configured PWM outputs in this structure! //
 * // set duty cycle only via: (pwm_status[i].duty_cycle = value)! //
 * static PWM_STATUS pwm_status[] = {{IO_PIN_H1, IO_E_OK, 0x8000, FALSE },
 *                                   {IO_PIN_G1, IO_E_OK, 0x8000, FALSE }};
 *                                 // ...
 *
 * // global variables for manual safety switch test //
 * static SSW_STATE ssw_test_next_state = SSW_START;
 * static SSW_STATE ssw_test_prev_state = SSW_START;
 * static ubyte1 ssw_cnt = 0;
 *
 * static ubyte1 APPL_SafetyCb( ubyte1 diag_state, DIAG_ERRORCODE * const error )
 * {
 *     ubyte1 i;
 *
 *     switch (error->device_num )
 *     {
 *         // add case for ALL safety configured PWM output pins! //
 *         case IO_PIN_H1:
 *         case IO_PIN_G1:
 *         // ...
 *             switch ( error->error_code )
 *             {
 *                 // ignore error case and label the affected PWM output for later manual ssw testing (pwm_man_ssw_test) //
 *                 case DIAG_E_SAFETY_SW_SHUT_OFF:
 *                     for (i = 0; i < SAFETY_PWM_NUM; i++)
 *                     {
 *                         if (pwm_status[i].pwm_pin == error->device_num)
 *                         {
 *                             pwm_status[i].pwm_man_ssw_test = TRUE;
 *                         }
 *                     }
 *                     return DIAG_ERR_NOACTION;
 *                 default:
 *                     return DIAG_ERR_NOACTION;
 *             }
 *         default:
 *             return DIAG_ERR_SAFESTATE;
 *     }
 * }
 *
 * // safety configuration for IO-Driver //
 * static const IO_DRIVER_SAFETY_CONF driver_safety_conf =
 * {
 *     IO_DRIVER_SAFETY_SWITCH_INT, // or IO_DRIVER_SAFETY_SWITCH_EXT,
 *     180,
 *     11000,
 *     APPL_SafetyCb
 * };
 *
 * // safety configuration for PWM output //
 * static const IO_PWM_SAFETY_CONF pwm_safety_conf =
 * {
 *     IO_SAFETY_SWITCH_0, // or IO_SAFETY_SWITCH_1
 *     50,
 *     150,
 *     200
 * };
 *
 *
 * void main (void)
 * {
 *     ubyte1 i;
 *     ubyte4 timestamp;
 *
 *     driver_init_rc = IO_Driver_Init ( IO_DRIVER_MODE_DEFAULT, &driver_safety_conf );
 *
 *     // safety configured PWM outputs
 *     pwm_init_rc = IO_PWM_Init( IO_PIN_H1, 200, TRUE, TRUE, 5000, &pwm_safety_conf );
 *     pwm_init_rc = IO_PWM_Init( IO_PIN_G1, 200, TRUE, TRUE, 5000, &pwm_safety_conf );
 *     // ...
 *
 *     power_rc = IO_POWER_Set( IO_INT_POWERSTAGE_ENABLE, IO_POWER_ON);
 *
 *     while (1)
 *     {
 *         (void) IO_RTC_StartTime (&timestamp);
 *         driver_task_begin_rc = IO_Driver_TaskBegin();
 *
 *
 *
 *         // User code //
 *
 *
 *
 *         // ========================= SAFETY PWM OUTPUTS ======================= //
 *
 *         // ensure that the duty cycle of not yet tested safety PWM outputs is 0%!//
 *         for (i = 0; i < SAFETY_PWM_NUM; i++)
 *         {
 *             if (pwm_status[i].pwm_man_ssw_test != FALSE)
 *             {
 *                 pwm_status[i].pwm_set_rc = IO_PWM_SetDuty(pwm_status[i].pwm_pin,
 *                                                           0,
 *                                                           NULL );
 *             }
 *             else
 *             {
 *                 pwm_status[i].pwm_set_rc = IO_PWM_SetDuty(pwm_status[i].pwm_pin,
 *                                                           pwm_status[i].duty_cycle,
 *                                                           NULL );
 *             }
 *         }
 *
 *         // trigger manual safety switch test (only during diagnostic main state!) //
 *         // ---------------------------------------------------------------------- //
 *         // Note: As the function Man_SSW_Test test has to be called several times //
 *         // in order to perform a complete manual safety switch test, it must be   //
 *         // ensured that the user defined trigger condition does not inhibit the   //
 *         // function call, once the manual safety switch test has been started!    //
 *         // ---------------------------------------------------------------------- //
 *         if ( (diag_state == DIAG_STATE_MAIN) &&
 *              (( #INSERT TRIGGER CONDITION HERE# ) || ( ssw_test_next_state != SSW_START )) )
 *         {
 *             Man_SSW_Test( pwm_safety_conf.safety_switch );
 *         }
 *
 *         // ==================================================================== //
 *
 *
 *
 *         // User code //
 *
 *
 *
 *         // retrieve diagnostic state and error code //
 *         diag_rc = DIAG_Status(&diag_state, &diag_error);
 *
 *         driver_task_end_rc = IO_Driver_TaskEnd ();
 *         while (IO_RTC_GetTimeUS (timestamp) < 10000);
 *     }
 * }
 *
 * // ====================== MANUAL SAFETY SWITCH TEST ====================== //
 * void Man_SSW_Test (IO_PIN safety_switch)
 * {
 *     ubyte1 i;
 *
 *     switch (ssw_test_next_state)
 *     {
 *         // [1]: start manual safety switch test //
 *         case SSW_START:
 *             (void) IO_POWER_Set( safety_switch, IO_POWER_OFF);
 *             ssw_test_next_state = SSW_WAIT;
 *             ssw_test_prev_state = SSW_START;
 *             break;
 *
 *         // [3]: check PWM set error only for not yet tested PWM outputs with load after SSW is disabled //
 *         case SSW_DISABLED:
 *             for (i = 0; i < SAFETY_PWM_NUM; i++)
 *             {
 *                 if ((pwm_status[i].pwm_man_ssw_test != FALSE) &&
 *                     (pwm_status[i].pwm_set_rc != IO_E_PWM_OPEN_LOAD))
 *                 {
 *                     // Error: PWM output is not correctly disabled! //
 *                     if (pwm_status[i].pwm_set_rc == IO_E_OK)
 *                     {
 *                         (void) DIAG_EnterSafestate();
 *                     }
 *                 }
 *             }
 *             (void) IO_POWER_Set( safety_switch, IO_POWER_ON);
 *             ssw_test_next_state = SSW_WAIT;
 *             ssw_test_prev_state = SSW_DISABLED;
 *             break;
 *
 *         // [5]: check PWM set error only for not yet tested PWM outputs with load after SSW is enabled //
 *         case SSW_ENABLED:
 *             for (i = 0; i < SAFETY_PWM_NUM; i++)
 *             {
 *                 if ((pwm_status[i].pwm_man_ssw_test != FALSE) &&
 *                     (pwm_status[i].pwm_set_rc != IO_E_PWM_OPEN_LOAD))
 *                 {
 *                     // Error: PWM output is not correctly enabled! //
 *                     if (pwm_status[i].pwm_set_rc != IO_E_OK)
 *                     {
 *                         (void) DIAG_EnterSafestate();
 *                     }
 *                 }
 *             }
 *             (void) IO_POWER_Set( safety_switch, IO_POWER_ON);
 *             ssw_test_next_state = SSW_DONE;
 *             break;
 *
 *         // [6]: set the tested flag for affected PWM outputs with load //
 *         case SSW_DONE:
 *             for (i = 0; i < SAFETY_PWM_NUM; i++)
 *             {
 *                 if ((pwm_status[i].pwm_man_ssw_test != FALSE) &&
 *                     (pwm_status[i].pwm_set_rc != IO_E_PWM_OPEN_LOAD))
 *                 {
 *                     pwm_status[i].pwm_man_ssw_test = FALSE;
 *                 }
 *             }
 *             ssw_test_next_state = SSW_START;
 *             break;
 *
 *         // [2, 4]: wait some software cycles, in order to receive plausible return values from IO_PWM_SetDuty() //
 *         case SSW_WAIT:
 *             if (ssw_cnt >= SSW_CYCLES)
 *             {
 *                 if (ssw_test_prev_state == SSW_START)
 *                 {
 *                     ssw_test_next_state = SSW_DISABLED;
 *                 }
 *                 else
 *                 {
 *                     ssw_test_next_state = SSW_ENABLED;
 *                 }
 *                 ssw_cnt = 0;
 *             }
 *             else
 *             {
 *                 ssw_cnt++;
 *             }
 *             break;
 *     }
 * }
 * \endcode
 *
 **************************************************************************/

/**********************************************************************//**
 * \page uds_examples Examples for using UDS support functions
 *
 * This chapter shows some examples how to use the support functions for
 * UDS reprogramming.
 *
 * \section read_brbl_did Read DataIdentifiers stored in the Branding Block
 *
 * This section describes how to read a DataIdentifier that has been stored
 * in the ECU's branding block.
 *
 * \code
 * IO_ErrorType rc_validate;
 * IO_ErrorType rc_get_did;
 * IO_BRBL_CUSTOM_DID did_item;
 *
 * // First the consistency of the BRBL has to be checked. The validity of
 * // the BRBL has to be checked only once per power cycle. It is best to
 * // call this function during the init phase (directly after calling \c IO_Driver_Init)
 * // because validating the BRBL will take approximately 1.2ms.
 * rc_validate = IO_BRBL_Validate();
 * if (rc_validate == IO_E_OK)
 * {
 *     rc_get_did = IO_BRBL_GetDid( IO_BRBL_CUSTOM_DID_IDX_0, &did_item );
 *     if (rc_get_did == IO_E_OK)
 *     {
 *         // The DID data can now be accessed as follows:
 *         // did_item.Did holds the DID (e.g. 0xFD00)
 *         // did_item.DidLength holds the length of the data that is attached to the DID
 *         // did_item.DidData is a pointer to the DID data (array with length did_item.DidLength)
 *         // Copying the DID data from the branding block can be done as follows:
 *         for (i = 0; i < did_item.DidLength; i++)
 *         {
 *             app_did_data_buffer[i] = did_item.DidData[i];
 *         }
 *     }
 * }
 * else
 * {
 *     // Reading from the BRBL is not possible because it could not be validated.
 * }
 * \endcode
 *
 * \section reprog_not_allowed Machine active - Reprogramming not allowed
 *
 * If the machine is currently active, reprogramming is not allowed in general.
 * In such a case none of the driver functions for UDS support may be called.
 * The following response shall be sent by the application:
 *
 * \code
 * //Tester: 02 10 02 55 55 55 55 55 (Request to switch to the programming sessions)
 * //App:    03 7F 10 22 55 55 55 55 (Request denied with code "conditions not correct")
 * \endcode
 *
 * \section rst_to_boot_mode_auth_bl Reset to boot mode with authentication in bootloader
 *
 * Example for giving control to the bootloader and letting the bootloader handle the
 * security access. In the example the length of the seed/key mechanism has been assumed
 * to be 4 bytes (setting in branding block). This method (giving control to the bootloader
 * triggered by the session control) is the one suggested within
 * ISO 14229-1:2013. It is also the easiest solution to implement for the application
 * software.
 *
 * \code
 * // Tester: 02 10 02 55 55 55 55 55 (App checks if machine is inactive ...)
 * // If reprogramming is allowed in the current operating state the application
 * // gives control to the bootloader. Please be aware that a response pending
 * // has to be sent in case the application does not switch to boot mode immediately
 * // but needs some time to shut down to a safe operating state!
 * (void)IO_Driver_ResetToBootMode( IO_DRIVER_RTBM_UDS_RPG_ATTEMPT_AUTH_BL
 *                                , IO_DRIVER_RTBM_UDS_RSP_SEND );
 * // The bootloader will now take over. Note: In this case the bootloader will send the positive response
 * // to the request to switch to the programming session (see function \c IO_Driver_ResetToBootMode).
 * // BL    : 06 50 02 00 32 01 F4 55
 * // Tester: 02 27 07 55 55 55 55 55
 * // BL    : 06 67 07 xx xx xx xx 55
 * // Tester: 06 27 08 yy yy yy yy 55
 * // BL    : 02 67 08 55 55 55 55 55
 * \endcode
 *
 * \section rst_to_boot_mode_auth_app Reset to boot mode with authentication in application
 *
 * Example for giving control to the bootloader after the application software handled the
 * security access. In the example the length of the seed/key mechanism has been assumed
 * to be 4 bytes (setting in branding block).
 * \attention
 *   With this approach the authentication within the bootloader is bypassed and has to be 
 *   handled by the application. 
 *   The responsibility for correct authentication lies solely with the application and therefore with the system integrator.
 *
 * \code
 * ubyte4 prn;
 * ubyte4 key;
 * ubyte4 secret_key[IO_BRBL_XTEA_PRIV_KEY_LEN];
 *
 * // Tester: 02 10 02 55 55 55 55 55 (App checks if machine is inactive ...)
 * // App   : 06 50 02 00 32 01 F4 55 (... if yes a positive response needs to be sent.)
 * // Tester: 02 27 07 55 55 55 55 55
 * // App   : Calls the driver function to generate a random seed.
 * (void)IO_Crypt_GetPseudoRandomNumber( &prn );
 * //Be aware that service 0x27 uses the Big Endian format but the XC2000 CPU uses the Little
 * //Endian format. Thus when copying prn to the UDS message, byte3 of prn must be on the LSB position
 * //in the response message and byte0 of prn on the MSB position.
 * // App   : 06 67 07 xx xx xx xx 55 (Random Seed "xx xx xx xx" from IO_Crypt_GetPseudoRandomNumber)
 * // Tester: 06 27 08 yy yy yy yy 55
 * // App   : Deciphers the key sent by the tester by passing "yy yy yy yy" to the XTEA decipher function.
 * // Be aware that the key is sent by the tester in Big Endian format. It must be stored in the variable
 * // key in Little Endian format.
 * (void)IO_BRBL_GetXteaKey( IO_BRBL_XTEA_PRIV_KEY_IDX_1
 *                         , secret_key //array of IO_BRBL_XTEA_PRIV_KEY_LEN elements
 *                         , IO_BRBL_XTEA_PRIV_KEY_LEN );
 * (void)IO_Crypt_XteaDecipher32( &key //yyyyyyyy
 *                              , secret_key
 *                              , TRUE );
 * // The key is now deciphered and must match the sent seed again (otherwise a negative response has to be sent!).
 * // If seed matches the deciphered key the control can be given to the bootloader.
 * IO_Driver_ResetToBootMode( IO_DRIVER_RTBM_UDS_RPG_ATTEMPT_AUTH_APP
 *                          , IO_DRIVER_RTBM_UDS_RSP_SEND );
 * // The bootloader will now take over. Note: In this case the bootloader will send the positive response
 * // to the previous service 0x27 message of the tester (see function \c IO_Driver_ResetToBootMode).
 * // BL    : 02 67 08 55 55 55 55 55
 * \endcode
 *
 **************************************************************************/

#ifndef _IO_DRIVER_H
#define _IO_DRIVER_H

/**********************************************************************//**
 * \mainpage HY-TTC 30 Family I/O Driver Manual
 *
 * \version 1.30.0 (Software artefact reference: D-TTC-X-C-20-001)
 * \date    10.07.2020
 *
 *
 * This document describes the API of the XC2000 IO Driver for the HY-TTC 30 Family.
 *
 * The following driver functions are described:
 *      - \ref IO_Driver.h              "General driver items"
 *      - \ref IO_Constants.h           "Driver constants"
 *      - \ref IO_Pins.h                "Pin definitions, constants and aliases"
 *      - \ref pin_features_30x         "Pin features and overview"
 *      - \ref pin_diag_features        "Diagnostic features overview"
 *      - \ref Apdb.h                   "APDB for boot loader"
 *      - \ref DIAG_Constants.h         "Constants for ECU diagnostics"
 *      - \ref DIAG_Functions.h         "Functions for ECU diagnostics"
 *      - \ref IO_DIO.h                 "Driver for digital inputs and outputs (DIO)"
 *      - \ref IO_ADC.h                 "Analog to Digital Converter (ADC) driver"
 *      - \ref IO_PWM.h                 "Pulse Width Modulation (PWM) driver"
 *      - \ref IO_PWD.h                 "Pulse Width Decode (PWD) and digital timer input driver"
 *      - \ref IO_PVG.h                 "Proportional Valve Group (PVG) output driver"
 *      - \ref IO_VOut.h                "Voltage output driver (VOut)"
 *      - \ref IO_RTC.h                 "Real Time Clock (RTC) driver"
 *      - \ref IO_LED.h                 "LED driver"
 *      - \ref IO_CAN.h                 "Controller Area Network (CAN) driver"
 *      - \ref IO_UART.h                "Universal Asynchronous Receiver Transmitter (UART) driver"
 *      - \ref IO_EEPROM.h              "EEPROM driver"
 *      - \ref IO_EEPROM_Preload.h      "EEPROM preload functions"
 *      - \ref IO_POWER.h               "Driver for ECU power functions"
 *      - \ref IO_NodeID.h              "NodeID pins driver"
 *      - \ref IO_WD.h                  "Window Watchdog driver"
 *      - \ref IO_PID.h                 "PID controller"
 *      - \ref IO_WDTimer.h             "Internal Watchdog"
 *      - \ref IO_Crypt.h               "Cryptographic functions"
 *      - \ref IO_BRBL.h                "Functions for accessing parts of the branding block of the ECU"
 *      - \ref uds_examples             "Examples for using UDS support functions"
 *      - \ref safety_examples          "Examples for implementation of Safety Functions"
 *        - \ref safety_cb_example      "Example implementation for Safety-Callback-Function"
 *        - \ref safety_step_example    "Example for error code evaluation"
 *        - \ref safety_driver_example  "Example for IO-Driver safety configuration"
 *        - \ref safety_switch_example  "Example for manual safety switch test"
 *
 *
 **************************************************************************/

/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/

#include "ptypes_xe167.h"
#include "IO_Constants.h"
#include "DIAG_Constants.h"
#include "IO_Pins.h"

/**************************************************************************
 *
 * D E F I N I T I O N S
 *
 **************************************************************************/

/**
 * \name CPU clock setting
 *
 * \brief CPU frequency in MHz
 */
/*\{*/
#define IO_DRIVER_SYSTEM_CLOCK  80  /**< CPU frequency in MHz */
/*\}*/

/**
 * \name Driver operating modes
 *
 * \attention
 *   These defines are left in for compatibility reasons. Their
 *   purpose has gone with driver version 1.2.24.
 *   Setting these parameters has no effect. Non-safety ECUs do not have a
 *   external watchdog and for safety relevant ECUs the external watchdog
 *   is triggered by \c IO_Driver_TaskBegin. However, one of these values
 *   has to be used for the parameter \c mode of the function \c IO_Driver_Init
 *   because the parameter checks of this functions are still active (for
 *   compatibility reasons).
 *

 */
/*\{*/
#define IO_DRIVER_MODE_DEFAULT       0x00   /**< Default mode for the IO-Driver with:\n
                                                 - need to manually serve the Window Watchdog (\ref IO_WD.h) */
#define IO_DRIVER_MODE_SERVICE_WD    0x02   /**< Bit to set IO-Driver mode to service the Window Watchdog automatically.\n
                                                 The Watchdog will be served automatically within function #IO_Driver_TaskBegin.
                                                 Due to a Watchdog window of 2ms..45ms (see \ref IO_WD.h), in this mode
                                                 the application cycle time must not exceed 45ms. */
/*\}*/

/**
 * \name CPU reset status
 */
/*\{*/
#define IO_DRIVER_RST_STAT_NA    0x00   /**< Reset status could not be determined */
#define IO_DRIVER_RST_STAT_PORST 0x01   /**< Reset status for a Poweron Reset */
#define IO_DRIVER_RST_STAT_WD    0x02   /**< Reset status for a Window Watchdog reset */
#define IO_DRIVER_RST_STAT_WDT   0x04   /**< Reset status for a Watchdog Timer reset */
#define IO_DRIVER_RST_STAT_SW    0x08   /**< Reset status for a Software reset */
/*\}*/


/**
 * \name Options regarding the UDS security access for function reset to boot mode
 */
/*\{*/
#define IO_DRIVER_RTBM_UDS_RPG_ATTEMPT_NONE     0x0000  /**< No reprogramming attempt. Bootloader will not stop during boot. */
#define IO_DRIVER_RTBM_UDS_RPG_ATTEMPT_AUTH_BL  0x0001  /**< Reset to boot mode. Authentication will be done in the bootloader. This is the recommended option */
#define IO_DRIVER_RTBM_UDS_RPG_ATTEMPT_AUTH_APP 0x0002  /**< Reset to boot mode. Authentication has been done already
                                                             by the application software. Not recommended! */
/*\}*/

/**
 * \name Options regarding the response behavior of the bootloader after it was triggered by the application
 *       due to an UDS reprogramming attempt.
 */
/*\{*/
#define IO_DRIVER_RTBM_UDS_RSP_NONE             0x0000  /**< The bootloader will not send any response without receiving
                                                             the request for reprogramming by itself. */
#define IO_DRIVER_RTBM_UDS_RSP_SEND             0x0001  /**< The bootloader will send the response to the request for
                                                             reprogramming that has been received by the application. */
/*\}*/

/**
 * \brief Reset reasons
 *
 * \remark
 * - Depending on the ECU not all or additional reasons are available.
 *
 * \remark
 * - Definitions may be used instead of enumerations.
 */
typedef enum _io_driver_reset_reason
{
    IO_DRIVER_RESET_REASON_PORST     /**< Power-On reset */
  , IO_DRIVER_RESET_REASON_WDT       /**< Watchdog-Timer reset */
  , IO_DRIVER_RESET_REASON_TRAP      /**< Reset due to exception */
  , IO_DRIVER_RESET_REASON_SW        /**< Reset due to call of IO_Driver_ResetToBootMode or IO_Driver_Reset */
  , IO_DRIVER_RESET_REASON_UNKNOWN   /**< Unknown reset */
} IO_DRIVER_RESET_REASON;

/**
 * \name Safety switch
 *
 * Type of used safety switch (internal or external)
 */
/*\{*/
#define IO_DRIVER_SAFETY_SWITCH_INT     0x00 /**< Internal safety switch */
#define IO_DRIVER_SAFETY_SWITCH_EXT     0x01 /**< External safety switch */
#define IO_DRIVER_SAFETY_SWITCH_NONE    0x02 /**< No safety switch */
/*\}*/

/**
 * \name No safety switch 
 *
 * Not use safety switch 0,1 for the second shut-off path
 */
/*\{*/
#define IO_SAFETY_SWITCH_NONE IO_PIN_NONE /**<Pin NONE, PWM safety switch none configuration*/
/*\}*/


/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/
/**********************************************************************//**
 *
 * \brief Driver Safety Configuration
 *
 * This structure is used to pass the configuration for
 * a safety critical application to the IO-Driver.
 *
 **************************************************************************/
typedef struct _io_driver_safety_conf
{
    ubyte1              safety_switch_type; /**< Declares whether an internal, external or no safety switch is used
                                                 ( \c #IO_DRIVER_SAFETY_SWITCH_INT, \c #IO_DRIVER_SAFETY_SWITCH_EXT or
                                                   \c #IO_DRIVER_SAFETY_SWITCH_NONE ) */
    ubyte1              glitch_filter_time; /**< Filter time for debouncing errors (10..180ms) */
    ubyte2              cycle_time;         /**< Maximum Cycle time for the IO-Driver task (500..25000us)   */
    DIAG_ERR_CALLBACK   error_callback;     /**< Callback function that will be called in case of non-fatal errors.
                                                 Can be set to NULL if no function shall be called */
}IO_DRIVER_SAFETY_CONF;
/**
 * \brief Contains information regarding traps/exceptions
 */
typedef struct _io_driver_trap_info
{
    ubyte2                 trap_id;         /**< Trap-id. Can be one of:
                                                 - 0: no trap occurred
                                                 - 1: Service Request 0 trap occurred
                                                 - 2: System stack overflow trap occurred
                                                 - 3: System stack underflow trap occurred
                                                 - 4: Software trap occurred
                                                 - 5: Class B trap occurred */
    ubyte2                 cpu_tfr;         /**< Content of CPU register TFR before reset */
    ubyte2                 cpu_trapstat;    /**< Content of CPU register TRAPSTAT before reset */
    ubyte2                 cpu_eccstat;     /**< Content of CPU register ECCSTAT before reset */
    ubyte2                 cpu_pecon;       /**< Content of CPU register PECON before reset */
    ubyte4                 fault_location;  /**< Fault location. In most cases the address given
                                                 in this variable points to the location following
                                                 the one which caused the exception/trap. This fault
                                                 location is only a hint where to search. It does
                                                 not point to the exact location of the problem.
                                                 The fault location is only available for system
                                                 stack over-/under-flow and class B traps. */
} IO_DRIVER_TRAP_INFO;

/**
 * \brief Reset information
 */
typedef struct _io_driver_rst_info
{
    IO_DRIVER_RESET_REASON  reset_reason;   /**< Reset reason */
    ubyte2                  reset_counter;  /**< Reset counter, 0 means it was a power-on reset */
    IO_DRIVER_TRAP_INFO     trap_info;      /**< Trap/Exception information */
} IO_DRIVER_RESET_INFO;

/**************************************************************************
 *
 * F U N C T I O N S
 *
 **************************************************************************/

/**********************************************************************//**
 *
 * \brief Global initialization of IO driver.
 * This function shall be called before any other driver function
 * (except function #IO_Driver_GetVersionOfDriver() and
 * #IO_Driver_GetResetStatus())
 *  - Initializes SPI Devices
 *  - Switches off all power outputs
 *  - Initializes the RTC
 *  - Switches on the interrupts of the CPU
 *  - Initializes the measurement of UBAT
 *  - Initializes and configures the Window Watchdog
 *
 * \param  mode            A bitfield that allows to configure various settings
 *                         of the IO-Driver. One or more of:
 *                         - #IO_DRIVER_MODE_DEFAULT: Default mode with
 *                           manual triggering.
 *                         - #IO_DRIVER_MODE_SERVICE_WD: The Watchdog is
 *                           serviced automatically by the IO-Driver.
 *
 * \param safety_conf      Safety Configuration if the hardware is supporting
 *                         and used as safety device. Otherwise set this
 *                         parameter to NULL.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_WRONG_HW_TYPE          wrong HW-Type detected
 * \retval #IO_E_BUSY                   driver is just initialized
 * \retval #IO_E_SAFETY_NOT_SUPPORTED   the HW does not support safety features
 * \retval #IO_E_INVALID_SAFETY_CONFIG  the safety configuration is faulty
 * \retval #IO_E_INVALID_CRC            the CRC from the APDB is invalid
 * \retval #IO_E_TASK_NO_FREE_SLOTS     No more free slots to setup task function
 *
 * \remarks
 *         Initializing the driver with a valid safety configuration might take
 *         about 65ms.
 *
 **************************************************************************
 *
 * \attention
 *   The parameter mode is left in for compatibility reasons. Its
 *   functionality has been removed with driver version 1.2.24.
 *   Setting this parameter has no effect. Non-safety ECU do not have a
 *   external watchdog and for safety relevant ECUs the external watchdog
 *   is triggered by \c IO_Driver_TaskBegin.
 *
 **************************************************************************/
IO_ErrorType IO_Driver_Init( ubyte1 mode
                           , const IO_DRIVER_SAFETY_CONF * const safety_conf );

/**********************************************************************//**
 *
 * \brief Returns the mode configured with #IO_Driver_Init
 *
 * This function returns the mode passed when initializing the IO-Driver
 * with #IO_Driver_Init.
 *
 * \param  mode            Returned mode (see #IO_Driver_Init for details)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has
 *                                      not been called before
 *
 **************************************************************************
 *
 * \attention
 *   This function has been left in for compatibility reasons. Its
 *   functionality has been removed with driver version 1.2.24.
 *   The returned value for \c mode is always \c #IO_DRIVER_MODE_DEFAULT
 *
 **************************************************************************/
IO_ErrorType IO_Driver_GetMode ( ubyte1 * const mode );

/**********************************************************************//**
 *
 * \brief Returns the version number of the driver
 *
 * \param major          Major version
 * \param minor          Minor version
 * \param patchlevel     Patchlevel
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                    everything fine
 * \retval #IO_E_NULL_POINTER          null pointer has been passed
 *
 **************************************************************************/
IO_ErrorType IO_Driver_GetVersionOfDriver( ubyte1 * const major
                                         , ubyte1 * const minor
                                         , ubyte2 * const patchlevel);

 /**********************************************************************//**
  *
  * \brief Returns the version number of the bootloader
  *
  * \param major        Major version
  * \param minor        Minor version
  * \param patchlevel   Patchlevel
  *
  * \return #IO_ErrorType
  * \retval #IO_E_OK                    everything fine
  * \retval #IO_E_NULL_POINTER          null pointer has been passed
  *
  **************************************************************************/
IO_ErrorType IO_Driver_GetVersionOfBootloader( ubyte1 *const major
                                             , ubyte1 *const minor
                                             , ubyte4 *const patchlevel );

/**********************************************************************//**
 *
 * \brief Task function for IO Driver.
 * This function shall be called at the beginning of the task.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                         everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED     Driver was not initialized correctly
 *
 * \retval #IO_E_UART_PARITY                parity check failed
 * \retval #IO_E_UART_OVERFLOW              HW receive buffer overrun
 * \retval #IO_E_UART_BUFFER_FULL           SW receive queue is full and data has been lost
 *
 **************************************************************************/
IO_ErrorType IO_Driver_TaskBegin(void);

/**********************************************************************//**
 *
 * \brief Task function for IO Driver.
 * This function shall be called at the end of the task.
 *
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED Driver was not initialized correctly
 *
 * \retval #IO_E_UART_PARITY            parity check failed
 * \retval #IO_E_UART_OVERFLOW          HW receive buffer overrun
 * \retval #IO_E_UART_BUFFER_FULL       SW receive queue is full and data has been lost
 *
 **************************************************************************/
IO_ErrorType IO_Driver_TaskEnd(void);

/**********************************************************************//**
 *
 * \brief Returns the result of the autobaudrate detection.
 *
 * This function returns the result of the autobaudrtate detection which
 * was executed during startup (within the bootloader).
 *
 * \return ubyte2   autoBaudrate
 *
 * \retval 0        the autoBaudrate detection could not verify a valid baudrate
 * \retval 125      a baudrate of 125kbps was detected
 * \retval 250      a baudrate of 250kbps was detected
 * \retval 500      a baudrate of 500kbps was detected
 *
 **************************************************************************/
ubyte2 IO_Driver_GetAutoBaudrate(void);

/**********************************************************************//**
 * \brief Resets the ECU to boot mode and instructs the bootloader to
 *        proceed with an UDS download procedure.
 *
 * \param bm_option   Option for Bootloader
 *                    - #IO_DRIVER_RTBM_UDS_RPG_ATTEMPT_AUTH_BL:
 *                    Reset to boot mode. Authentication will be handled by
 *                    the bootloader. This is considered the standard compliant
 *                    way and thus recommended.
 *                    - #IO_DRIVER_RTBM_UDS_RPG_ATTEMPT_AUTH_APP:
 *                    Reset to boot mode. Authentication will be handled by
 *                    the application software. It is the software integrator's
 *                    responsibility to secure the ECU from unauthorized access
 *                    in this case. This option is considered to be not compliant to
 *                    the standard and thus NOT RECOMMENDED! It is only possible to
 *                    unlock security access level 1 via this function (reprogramming
 *                    of application).
 *
 * \param bm_option_rsp Defines if the response to either the UDS request to switch
 *                      to the programming session or the security access has to be
 *                      sent by the bootloader or not.
 *                      - #IO_DRIVER_RTBM_UDS_RSP_NONE: The bootloader will not send
 *                      any response without receiving the request for reprogramming
 *                      by itself.
 *                      - #IO_DRIVER_RTBM_UDS_RSP_SEND: Instruct the bootloader to
 *                      send a response to the request for reprogramming that has been
 *                      received by the application. The response depends on the
 *                      parameter \c bm_option: if called with
 *                      IO_DRIVER_RTBM_UDS_RPG_ATTEMPT_AUTH_BL, the application
 *                      responds to the request to switch to the programming session
 *                      (UDS service 0x10 response message <06 50 02 00 32 01 F4 55>);
 *                      if called with IO_DRIVER_RTBM_UDS_RPG_ATTEMPT_AUTH_APP, the
 *                      application responds to "send key" (UDS service 0x27 response
 *                      message <02 67 xx 55 55 55 55 55>, where "xx" is the respective
 *                      sub-function selected in the branding block, or the default 0x62).
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                     Function executed Successfully (This
 *                                      error will never be shown because if
 *                                      the function executed successfully a
 *                                      CPU reset has been done and the function
 *                                      will never return).
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The I/O driver init function has not
 *                                      been called before.
 * \retval #IO_E_INVALID_PARAMETER      An invalid parameter has been passed.
 *
 **************************************************************************
 *
 * \remarks
 *  - Prior to calling this function the pre-programming steps according to
 *  ISO 14229-1:2013 have to be handled by the application software.
 *  - After receiving a request to switch to the programming session the
 *  application software shall call this function to switch to the programming
 *  session if the machine conditions allow it.
 *  - It is recommended that after the application received a request to switch to the
 *  programming session (CAN message <02 10 02>) the function \c
 *  IO_Driver_ResetToBootMode( IO_DRIVER_RTBM_UDS_RPG_ATTEMPT_AUTH_BL ) is be called.
 *  If the application needs time to shut down then a RCRRP (service 0x78) shall be sent
 *  to the diagnose tester.
 *  - If this function is called with parameter #IO_DRIVER_RTBM_UDS_RPG_ATTEMPT_AUTH_BL
 *  the bootloader will send the positive response to the request to switch to
 *  the programming session (CAN message <06 50 02 00 32 01 F4>).
 *  - If this function is called with parameter #IO_DRIVER_RTBM_UDS_RPG_ATTEMPT_AUTH_APP
 *  the application software must add the bootloaders P2 timing parameters into the
 *  response of request to switch to the programming session.
 *  - Be aware that the option #IO_DRIVER_RTBM_UDS_RSP_NONE for parameter \c bm_option_rsp
 *  is not recommended. If the application sends the response to the request to switch to
 *  the programming session before it calls the function \c IO_Driver_ResetToBootMode,
 *  then there will be a gap where the ECU can not answer any requests from the tester
 *  until the bootloader including its UDS server is started. For the TTC30X type of ECUs
 *  this time can be up to 30ms (time from calling the function IO_Driver_ResetToBootMode
 *  until the UDS server of the bootloader is ready to accept commands).
 *
 * \attention
 *   Be aware that resetting to boot mode during normal operation is
 *   dangerous and in general NOT ALLOWED. The application software is only
 *   authorized to call this function if:
 *   - The vehicle is inactive/not moving
 *   - The engine is not running
 *   - the risk of any other hazard by stopping the application software at
 *     the time of the reprogramming request can be ruled out
 *
 **************************************************************************/
IO_ErrorType IO_Driver_ResetToBootMode( ubyte2 bm_option
                                      , ubyte2 bm_option_rsp );

/**********************************************************************//**
 *
 * \brief Returns the reset status.
 *
 * This function returns the status/reason for the last CPU-reset.
 *
 * \param  status            Returned status. One of:\n
 *                             - #IO_DRIVER_RST_STAT_NA
 *                             - #IO_DRIVER_RST_STAT_PORST
 *                             - #IO_DRIVER_RST_STAT_WD
 *                             - #IO_DRIVER_RST_STAT_WDT
 *                             - #IO_DRIVER_RST_STAT_SW
 *
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                 everything fine
 * \retval #IO_E_NULL_POINTER       null pointer has been passed
 *
 **************************************************************************/
IO_ErrorType IO_Driver_GetResetStatus(ubyte2 * const status);

/**********************************************************************//**
 * \brief Returns the reset status in more detail
 *
 * \param[out]  reset_info    Information about reset
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                        Everything ok.
 * \retval #IO_E_NULL_POINTER              A null pointer has been passed
 * \retval #IO_E_RESET_COUNTER_INVALID     The reset counter is invalid. No reset or trap info is
 *                                         available (This indicates a SW or HW malfunction).
 * \retval #IO_E_SBRAM_CONTENT_INVALID     The trap information stored in the SBRAM is invalid.
 *                                         No trap info is available (This indicates a SW or HW
 *                                         malfunction).
 *
 *
 **************************************************************************/
IO_ErrorType IO_Driver_GetResetStatus_ex( IO_DRIVER_RESET_INFO * const reset_info );


#endif /* _IO_DRIVER_H */
