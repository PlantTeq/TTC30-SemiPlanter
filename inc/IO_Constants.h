/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_Constants.h
 *
 * \brief Global defines for IO Driver
 *
 * This header file defines the Error Codes for the IO-Driver.
 *
 **************************************************************************/

#ifndef _IO_CONSTANTS_H
#define _IO_CONSTANTS_H

/**********************************************************************//**
 * \page pin_features_30x HY-TTC30 Family pin features
 *
 * \brief Listing of all IO driver pins and their configuration options
 *
 * \section pin_feat_30x_adc Analog Inputs
 * \subsection pin_feat_30x_adc_1m 1-Mode ADC inputs (primary function)
 *
 * These pins can be used as ADC inputs:
 * - \c #IO_ADC_20
 * - \c #IO_ADC_21
 *
 * They can be configured by software for:
 *    - <i> absolute (0 .. 32V)</i>
 *
 * For details on the usage refer to the \ref IO_ADC.h "driver functions for analog inputs".
 *
 * As a secondary function they can be used as:
 * - \ref pin_feat_30x_adc_1m_di "Digital Inputs"
 *
 * \subsection pin_feat_30x_adc_3mode 3-Mode ADC inputs (primary function)
 * These pins can be used as ADC inputs:
 * - \c #IO_ADC_10
 * - \c #IO_ADC_11
 * - \c #IO_ADC_12
 * - \c #IO_ADC_13
 * - \c #IO_ADC_14 (for HY-TTC32 variants pin supports also resistive mode)
 * - \c #IO_ADC_15 (for HY-TTC32 variants pin supports also resistive mode)
 *
 * They can be configured by software for:
 *    - <i> absolute (0 .. 5V or 0 .. 10V)</i>
 *    - <i> ratiometric (0 .. 5V)</i>
 *    - <i> current (0 .. 24mA)</i>
 *
 * For details on the usage refer to the \ref IO_ADC.h "driver functions for analog inputs".
 *
 * As a secondary function they can be used as:
 * - \ref pin_feat_30x_adc_3mode_di "Digital Inputs"
 *
 * \subsection pin_feat_30x_adc_4mode 4-Mode ADC inputs (primary function)
 * These pins can be used as ADC inputs:
 * - \c #IO_ADC_00
 * - \c #IO_ADC_01
 *
 * They can be configured by software for:
 *    - <i> absolute (0 .. 5V or 0 .. 10V)</i>
 *    - <i> ratiometric (0 .. 5V)</i>
 *    - <i> current (0 .. 24mA)</i>
 *    - <i> resistive (0 .. 65kOhm)</i>
 *
 * For details on the usage refer to the \ref IO_ADC.h "driver functions for analog inputs".
 *
 * As a secondary function they can be used as:
 * - \ref pin_feat_30x_adc_4mode_di "Digital Inputs"
 *
 * \subsection pin_feat_30x_adc_other Normal ADC inputs
 * These pins have a fixed assignment to internal board voltages:
 * - \c #IO_ADC_UBAT
 * - \c #IO_ADC_UBAT_CPU
 * - \c #IO_ADC_SENSOR_SUPPLY
 * - \c #IO_K15
 * - \c #IO_ADC_NODE_ID_0
 * - \c #IO_ADC_NODE_ID_1
 * - \c #IO_ADC_BOARD_TEMP
 *
 * For details on the usage refer to the \ref IO_ADC.h "driver functions for analog inputs".
 *
 * \subsection pin_feat_30x_pwm_adc 32V Analog Inputs (secondary function of PWM Outputs)
 * As secondary function, these pins can be used as Analog Inputs:
 * - \c #IO_ADC_34
 * - \c #IO_ADC_35
 * - \c #IO_ADC_36
 * - \c #IO_ADC_37
 * - \c #IO_ADC_38
 * - \c #IO_ADC_39
 * - \c #IO_ADC_40
 * - \c #IO_ADC_41
 *
 * They can be configured by software for:
 *    - <i> absolute (0 .. 32V)</i>
 *
 * \subsection pin_feat_30x_led LED Outputs
 * One of the following: 
 * - \c #IO_LED_00
 * - \c #IO_LED_01
 * - \c #IO_LED_02
 * - \c #IO_LED_03
 * - \c #IO_LED_04
 * - \c #IO_LED_05
 * - \c #IO_LED_06
 * - \c #IO_LED_07
 * 
 *
 *
 *
 *
 *
 * \subsection pin_feat_30x_do_ls_adc 32V Analog Inputs (secondary function of Low-Side Digital Outputs)
 * As secondary function, these pins can be used as Analog Inputs:
 * - \c #IO_ADC_28
 * - \c #IO_ADC_29
 *
 * They can be configured by software for:
 *    - <i> absolute (0 .. 32V)</i>
 *
 * \subsection pin_feat_30x_pwd_adc 32V Analog Inputs (secondary function of PWD Inputs)
 * As secondary function, these pins can be used as Analog Inputs:
 * - \c #IO_ADC_30
 * - \c #IO_ADC_31
 * - \c #IO_ADC_32
 * - \c #IO_ADC_33
 *
 * They can be configured by software for:
 *    - <i> absolute (0 .. 32V)</i>
 *
 * \subsection pin_feat_30x_pvg_adc 32V Analog Inputs (secondary function of PVG Outputs)
 * As secondary function, these pins can be used as Analog Inputs:
 * - \c #IO_ADC_22
 * - \c #IO_ADC_23
 * - \c #IO_ADC_24
 * - \c #IO_ADC_25
 * - \c #IO_ADC_26
 * - \c #IO_ADC_27
 *
 * They can be configured by software for:
 *    - <i> absolute (0 .. 32V)</i>
 *
 * \section pin_feat_30x_di Digital inputs
 *
 * \subsection pin_feat_30x_adc_1m_di Digital inputs with configurable pull-up/down resistor (secondary function of 1-Mode ADC Inputs)
 * As secondary function, these pins can be used as digital inputs with a voltage range of 0-32V
 * and have a configurable pull-up/down resistor:
 * - \c #IO_DI_02
 * - \c #IO_DI_03
 *
 * For details on the usage refer to the
 * \ref IO_DIO.h "driver functions for digital inputs and outputs".
 *
 * \subsection pin_feat_30x_adc_3mode_di Digital inputs (secondary function of 3-Mode ADC Inputs)
 * As secondary function, these pins can be used as digital inputs with a voltage range of 0-10V:
 * - \c #IO_DI_10
 * - \c #IO_DI_11
 * - \c #IO_DI_12
 * - \c #IO_DI_13
 * - \c #IO_DI_14
 * - \c #IO_DI_15
 *
 * For details on the usage refer to the
 * \ref IO_DIO.h "driver functions for digital inputs and outputs".
 *
 * \subsection pin_feat_30x_adc_4mode_di Digital inputs (secondary function of 4-Mode ADC Inputs)
 * As secondary function, these pins can be used as digital inputs with a voltage range of 0-10V:
 * - \c #IO_DI_00
 * - \c #IO_DI_01
 *
 * For details on the usage refer to the
 * \ref IO_DIO.h "driver functions for digital inputs and outputs".
 *
 * \subsection pin_feat_30x_pwd_di Digital inputs with configurable pull-up/down resistor (secondary function of PWD Inputs)
 * As secondary function, these pins can be used as digital inputs with a voltage range of 0-32V
 * and have a configurable pull-up/down resistor:
 * - \c #IO_DI_04
 * - \c #IO_DI_05
 * - \c #IO_DI_06
 * - \c #IO_DI_07
 *
 * For details on the usage refer to the
 * \ref IO_DIO.h "driver functions for digital inputs and outputs".
 *
 * \subsection pin_feat_30x_pwm_di Digital inputs (secondary function of PWM Outputs)
 * As secondary function, these pins can be used as digital inputs with a voltage range of 0-32V:
 * - \c #IO_DI_24
 * - \c #IO_DI_25
 * - \c #IO_DI_26
 * - \c #IO_DI_27
 * - \c #IO_DI_28
 * - \c #IO_DI_29
 * - \c #IO_DI_30
 * - \c #IO_DI_31
 *
 * For details on the usage refer to the
 * \ref IO_DIO.h "driver functions for digital inputs and outputs".
 *
 * \subsection pin_feat_30x_do_ls_di Digital inputs (secondary function of Low-Side Digital Outputs)
 * As secondary function, these pins can be used as digital inputs with a voltage range of 0-32V:
 * - \c #IO_DI_22
 * - \c #IO_DI_23
 *
 * For details on the usage refer to the
 * \ref IO_DIO.h "driver functions for digital inputs and outputs".
 *
 * \subsection pin_feat_30x_pvg_di Digital inputs (secondary function of PVG Outputs)
 * As secondary function, these pins can be used as digital inputs with a voltage range of 0-32V:
 * - \c #IO_DI_16
 * - \c #IO_DI_17
 * - \c #IO_DI_18
 * - \c #IO_DI_19
 * - \c #IO_DI_20
 * - \c #IO_DI_21

 * For details on the usage refer to the
 * \ref IO_DIO.h "driver functions for digital inputs and outputs".

 * \section pin_feat_30x_pwd PWD Inputs
 * \subsection pin_feat_30x_pwd_comp_inc Complex Digital Timer Inputs with configurable pull-up/down resistor and incremental decoder (primary function)
 * These pins can be used as digital timer inputs with incremental decoding:
 * - \c #IO_PWD_00
 * - \c #IO_PWD_01
 *
 * They can decode
 * - <i>signals from incremental encoders</i>,
 * - <i>PWM signals</i> or
 * - <i>signals from ABS sensors</i>.
 *
 * Furthermore they provide analog (ADC) feedback.
 * As PWM decoder the frequency and pulse width can be measured at the same time.
 *
 * As a secondary function they can be used as:
 * - \ref pin_feat_30x_pwd_adc "Analog Inputs"
 * - \ref pin_feat_30x_pwd_di "Digital Inputs"
 *
 * \subsection pin_feat_30x_pwd_comp Complex Digital Timer Inputs with configurable pull-up/down resistor (primary function)
 * These pins can be used as digital timer inputs:
 * - \c #IO_PWD_02
 * - \c #IO_PWD_03
 *
 * They can decode
 * - <i>PWM signals</i> or
 * - <i>signals from ABS sensors</i>.
 *
 * Furthermore they provide analog (ADC) feedback.
 * As PWM decoder the frequency and pulse width can be measured at the same time.
 *
 * As a secondary function they can be used as:
 * - \ref pin_feat_30x_pwd_adc "Analog Inputs"
 * - \ref pin_feat_30x_pwd_di "Digital Inputs"
 *
 * \subsection pin_feat_30x_pwm_ocm_pwd Complex Digital Timer Inputs (secondary function of PWM Outputs)
 * As secondary function these pins can be used as digital timer inputs:
 * - \c #IO_PWD_22
 * - \c #IO_PWD_23
 *
 * They can decode
 * - <i>PWM signals</i> or
 * - <i>signals from ABS sensors</i>.
 *
 * Furthermore they provide analog (ADC) feedback.
 * As PWM decoder the frequency and pulse width can be measured at the same time.
 *
 * For details on the usage refer to the \ref IO_PWD.h "driver functions for digital timer inputs".
 *
 * \subsection pin_feat_30x_pwm_cm_pwd Digital Timer Inputs (secondary function of PWM Outputs with Current Measurement)
 * As secondary function these pins can be used as digital timer inputs to decode PWM signals.
 * They can measure either the frequency or the pulse duration:
 * - \c #IO_PWD_10
 * - \c #IO_PWD_11
 * - \c #IO_PWD_12
 * - \c #IO_PWD_13
 * - \c #IO_PWD_20
 * - \c #IO_PWD_21
 *
 * Furthermore they provide analog (ADC) feedback.
 *
 * For details on the usage refer to the \ref IO_PWD.h "driver functions for digital timer inputs".
 *
 * \section pin_feat_30x_pwm High-Side PWM outputs
 * \subsection pin_feat_30x_pwm_cm High-Side PWM Outputs with Current Measurement (primary function)
 * These pins can be used to generate a pulse width modulated (PWM) output:
 * - \c #IO_PWM_00
 * - \c #IO_PWM_01
 * - \c #IO_PWM_02
 * - \c #IO_PWM_03
 * - \c #IO_PWM_04
 * - \c #IO_PWM_05
 *
 * Furthermore they provide current measurement (CM), digital timer (PWD) and
 * analog (ADC) feedback.
 *
 * For details on the usage refer to the \ref IO_PWM.h "driver functions for PWM outputs".
 *
 * As a secondary function they can be used as:
 * - \ref pin_feat_30x_pwm_adc "Analog Inputs"
 * - \ref pin_feat_30x_pwm_di "Digital Inputs"
 * - \ref pin_feat_30x_pwm_cm_pwd "PWD Inputs"
 * - \ref pin_feat_30x_pwm_cm_do "Digital Outputs"
 *
 * \subsection pin_feat_30x_pwm_ocm High-Side PWM Outputs (primary function)
 * These pins can be used to generate a pulse width modulated (PWM) output:
 * - \c #IO_PWM_10
 * - \c #IO_PWM_11
 *
 * Furthermore they provide a over-current monitoring to detect overload situations,
 * digital timer (PWD) and analog (ADC) feedback.
 *
 * For details on the usage refer to the \ref IO_PWM.h "driver functions for PWM outputs".
 *
 * As a secondary function they can be used as:
 * - \ref pin_feat_30x_pwm_adc "Analog Inputs"
 * - \ref pin_feat_30x_pwm_di "Digital Inputs"
 * - \ref pin_feat_30x_pwm_ocm_pwd "PWD Inputs"
 * - \ref pin_feat_30x_pwm_ocm_do "Digital Outputs"
 *
 * \section pin_feat_30x_do Digital Outputs
 *
 * \subsection pin_feat_30x_do_ls Low-Side Digital Outputs (primary function)
 * These pins can be used as digital low-side outputs:
 * - \c #IO_DO_10
 * - \c #IO_DO_11
 *
 * For details on the usage refer to the
 * \ref IO_DIO.h "driver functions for digital inputs and outputs".
 *
 * As a secondary function they can be used as:
 * - \ref pin_feat_30x_do_ls_adc "Analog Inputs"
 * - \ref pin_feat_30x_do_ls_di "Digital Inputs"
 *
 * \subsection pin_feat_30x_pwm_cm_do High-Side Digital Outputs with current measurement (secondary function of PWM Outputs)
 * These pins can be used as digital high-side switches:
 * - \c #IO_DO_20
 * - \c #IO_DO_21
 * - \c #IO_DO_22
 * - \c #IO_DO_23
 * - \c #IO_DO_24
 * - \c #IO_DO_25
 *
 * Furthermore they provide current measurement (CM) and analog (ADC) feedback.
 *
 * For details on the usage refer to the
 * \ref IO_DIO.h "driver functions for digital inputs and outputs".
 *
 * \subsection pin_feat_30x_pwm_ocm_do High-Side Digital Outputs (secondary function of PWM Outputs)
 * These pins can be used as digital high-side switches:
 *  - \c #IO_DO_00
 *  - \c #IO_DO_01
 *  - \c #IO_DO_02
 *  - \c #IO_DO_03
 *  - \c #IO_DO_04
 *  - \c #IO_DO_05
 *  - \c #IO_DO_06
 *  - \c #IO_DO_07
 *
 * Furthermore they provide an over-current monitoring to detect overload
 * situations and analog (ADC) feedback.
 *
 * For details on the usage refer to the
 * \ref IO_DIO.h "driver functions for digital inputs and outputs".
 *
 * \subsection pin_feat_30x_pvg_do_pp Push-Pull Digital Outputs (secondary function of PVG Outputs)
 * These pins can be used as push-pull digital output switches:
 *  - \c #IO_DO_30
 *  - \c #IO_DO_31
 *  - \c #IO_DO_32
 *  - \c #IO_DO_33
 *  - \c #IO_DO_34
 *  - \c #IO_DO_35
 *
 * Furthermore they provide an over-current monitoring to detect overload
 * situations and analog (ADC) feedback.
 *
 * For details on the usage refer to the
 * \ref IO_DIO.h "driver functions for digital inputs and outputs".
 *
 * \section pin_feat_30x_pvg_vout PVG/Voltage Outputs
 *
 * \subsection pin_feat_30x_pvg PVG Outputs (primary function)
 * These pins can be used as digital low-side switches:
 * - \c #IO_PVG_00
 * - \c #IO_PVG_01
 * - \c #IO_PVG_02
 * - \c #IO_PVG_03
 * - \c #IO_PVG_04
 * - \c #IO_PVG_05
 *
 * For details on the usage refer to the
 * \ref IO_PVG.h "driver functions for PVG Outputs".
 *
 * As a secondary function they can be used as:
 * - \ref pin_feat_30x_pvg_adc "Analog Inputs"
 * - \ref pin_feat_30x_pvg_di "Digital Inputs"
 * - \ref pin_feat_30x_pvg_do_pp "Digital Outputs"
 *
 * \subsection pin_feat_30x_vout Voltage Outputs (secondary function of PVG Outputs)
 * These pins can be used as voltage outputs:
 * - \c #IO_VOUT_00
 * - \c #IO_VOUT_01
 * - \c #IO_VOUT_02
 * - \c #IO_VOUT_03
 * - \c #IO_VOUT_04
 * - \c #IO_VOUT_05
 *
 * Furthermore they analog (ADC) feedback.\n
 * These outputs are driven by a configurable PID controller.
 *
 * For details on the usage refer to the
 * \ref IO_Vout.h "driver functions for voltage outputs".
 *
 **************************************************************************/

/**********************************************************************//**
 * \page pin_diag_features Pin and diagnostic features
 *
 * \brief Explicit overview of the diagnostic functions of the ECU pins.
 *
 * \section pin_diag_ttc30x Diagnostic features of HY-TTC 30 Family
 *
 * \verbatim[table]
 * ======================================= ================ ================= ========================================== ==================================================================================== =========================================
 *   PIN GROUP                                                                  FEATURES                                   DIAGNOSTIC FUNCTIONS
 * ========================================================================== ========================================== ==============================================================================================================================
 *   Group                                   Pin Numbers      Defines           I/O Features                               Diagnostics                                                                          Error Codes
 * --------------------------------------- ---------------- ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *   <b>PWM output groups</b>
 * ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   \ref IO_PWM.h "PWM Output" \n           #IO_PIN_H1\n     #IO_PWM_00 \n      \ref IO_PWM.h "PWM Output"                  open load detection                 \n                                             #IO_E_PWM_OPEN_LOAD
 *   with timer feedback,       \n           #IO_PIN_G1\n     #IO_PWM_01 \n
 * + analog feedback            \n           #IO_PIN_F1\n     #IO_PWM_02 \n                                              ------------------------------------------------------------------------------------ -----------------------------------------
 *   and current feedback                    #IO_PIN_E1\n     #IO_PWM_03 \n                                                  short circuit to GND detection                                                     #IO_E_PWM_SHORT_CIRCUIT
 * +                                         #IO_PIN_D1\n     #IO_PWM_04 \n                                              ------------------------------------------------------------------------------------ -----------------------------------------
 *                                           #IO_PIN_C1\n     #IO_PWM_05 \n                                                  short circuit to UBAT detection                                                    #IO_E_PWM_SHORT_BATTERY
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           the PWM output is disabled  \n                                                     #IO_E_PWM_OUTPUT_DISABLED
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           measured signal period is too small                                                #IO_E_PWM_CAPTURE_ERROR
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           Current monitoring and over current protection.\n                                  #IO_E_PROT_USER_OVERLOAD\n
 *                                                                                                                           Be reminded that the output protection will return these errors not only if        #IO_E_PROT_TEMP_OVERLOAD\n
 *                                                                                                                           the output current is too high but if one of the following conditions are met: \n  #IO_E_PROT_ACTIVE\n
 *                                                                                                                           - over temperature \n                                                              #IO_E_PROT_FATAL\n
 *                                                                                                                           - Supply for power stages disconnected                                             #IO_E_PROT_REENABLE
 *                                                                                                                             (due to loss of gate drive for reverse polarity protection)\n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DO_00 \n       \ref IO_DIO.h "Digital Output"              open load detection \n                                                             #IO_E_DO_OPEN_LOAD
 * +                                                          #IO_DO_01 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DO_02 \n                                                   short circuit to GND detection \n                                                  #IO_E_DO_SHORT_CIRCUIT
 * +                                                          #IO_DO_03 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DO_04 \n                                                   short circuit to UBAT detection \n                                                 #IO_E_DO_SHORT_BATTERY
 * +                                                          #IO_DO_05 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           open load detection / short circuit to UBAT detection \n                           #IO_E_DO_OPEN_LOAD_OR_SHORT_BATTERY
 *                                                                                                                           (if a pull up resistor is configured) \n
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           low pass of digital outputs with analog feedback is being tuned in. \n             #IO_E_DO_DIAG_TRANSIENT_OSC
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           Current monitoring and current protection. \n                                      #IO_E_PROT_USER_OVERLOAD\n
 *                                                                                                                           Be reminded that the output protection will return these errors not only if        #IO_E_PROT_TEMP_OVERLOAD\n
 *                                                                                                                           the output current is too high but if one of the following conditions are met: \n  #IO_E_PROT_ACTIVE\n
 *                                                                                                                           - over temperature \n                                                              #IO_E_PROT_FATAL\n
 *                                                                                                                           - Supply for power stages disconnected                                             #IO_E_PROT_REENABLE
 *                                                                                                                             (due to loss of gate drive for reverse polarity protection)\n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DO_20 \n       \ref IO_DIO.h "Digital Output"              open load detection \n                                                             #IO_E_DO_OPEN_LOAD
 * +                                                          #IO_DO_21 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DO_22 \n                                                   short circuit to GND detection \n                                                  #IO_E_DO_SHORT_CIRCUIT
 * +                                                          #IO_DO_23 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DO_24 \n                                                   short circuit to UBAT detection \n                                                 #IO_E_DO_SHORT_BATTERY
 * +                                                          #IO_DO_25 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           open load detection / short circuit to UBAT detection \n                           #IO_E_DO_OPEN_LOAD_OR_SHORT_BATTERY
 *                                                                                                                           (if a pull up resistor is configured) \n
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           low pass of digital outputs with analog feedback is being tuned in. \n             #IO_E_DO_DIAG_TRANSIENT_OSC
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           Current monitoring and current protection. \n                                      #IO_E_PROT_USER_OVERLOAD\n
 *                                                                                                                           Be reminded that the output protection will return these errors not only if        #IO_E_PROT_TEMP_OVERLOAD\n
 *                                                                                                                           the output current is too high but if one of the following conditions are met: \n  #IO_E_PROT_ACTIVE\n
 *                                                                                                                           - over temperature \n                                                              #IO_E_PROT_FATAL\n
 *                                                                                                                           - Supply for power stages disconnected                                             #IO_E_PROT_REENABLE
 *                                                                                                                             (due to loss of gate drive for reverse polarity protection)\n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_ADC_34 \n      \ref IO_ADC.h "Analog Input"                no built-in diagnostic functions.
 *                                                            #IO_ADC_35 \n
 *                                                            #IO_ADC_36 \n
 *                                                            #IO_ADC_37 \n
 *                                                            #IO_ADC_38 \n
 *                                                            #IO_ADC_39 \n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_24 \n       \ref IO_DIO.h "Digital Input"\n             open load detection / short circuit to GND detection \n                            #IO_E_DI_OPEN_LOAD_OR_SHORT_CIRCUIT
 *                                                            #IO_DI_25 \n       (with proper limits configured)             (if a pull down resistor is configured) \n
 * +                                                          #IO_DI_26 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_27 \n                                                   short circuit to UBAT detection \n                                                 #IO_E_DI_SHORT_BATTERY
 * +                                                          #IO_DI_28 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_29 \n                                                   invalid voltage level detection (i.e. outside of specified limits) \n              #IO_E_DI_INVALID_VOLTAGE
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short to GND detection \n                                                          #IO_E_DI_SHORT_CIRCUIT
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           open load detection \n                                                             #IO_E_DI_OPEN_LOAD
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_PWD_20 \n      \ref IO_PWD.h "Digital Timer Input"         measured signal period is too small                                                #IO_E_PWD_CAPTURE_ERROR
 * +                                                          #IO_PWD_21 \n                                              ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_PWD_10 \n                                                  time measurement not finished yet                                                  #IO_E_PWD_NOT_FINISHED
 * +                                                          #IO_PWD_11 \n                                              ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_PWD_12 \n                                                  only a constant high level is detected                                             #IO_E_PWD_HIGH_LEVEL
 * +                                                          #IO_PWD_13 \n                                              ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           only a constant low level is detected                                              #IO_E_PWD_LOW_LEVEL
 * --------------------------------------- ---------------- ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *   \ref IO_PWM.h "PWM Output" \n           #IO_PIN_K1\n     #IO_PWM_10 \n      \ref IO_PWM.h "PWM Output"                  open load detection                 \n                                             #IO_E_PWM_OPEN_LOAD
 *   with timer feedback,       \n           #IO_PIN_J1\n     #IO_PWM_11 \n
 * + analog feedback                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short circuit to GND detection                                                     #IO_E_PWM_SHORT_CIRCUIT
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short circuit to UBAT detection                                                    #IO_E_PWM_SHORT_BATTERY
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           the PWM output is disabled  \n                                                     #IO_E_PWM_OUTPUT_DISABLED
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           measured signal period is too small                                                #IO_E_PWM_CAPTURE_ERROR
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           Over current monitoring and over current protection. \n                            #IO_E_PROT_USER_OVERLOAD\n
 *                                                                                                                           Be reminded that the output protection will return these errors not only if        #IO_E_PROT_TEMP_OVERLOAD\n
 *                                                                                                                           the output current is too high but if one of the following conditions are met: \n  #IO_E_PROT_ACTIVE\n
 *                                                                                                                           - over temperature \n                                                              #IO_E_PROT_FATAL\n
 *                                                                                                                           - Supply for power stages disconnected                                             #IO_E_PROT_REENABLE
 *                                                                                                                             (due to loss of gate drive for reverse polarity protection)\n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DO_06 \n       \ref IO_DIO.h "Digital Output"              open load detection \n                                                             #IO_E_DO_OPEN_LOAD
 * +                                                          #IO_DO_07 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short circuit to GND detection \n                                                  #IO_E_DO_SHORT_CIRCUIT
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short circuit to UBAT detection \n                                                 #IO_E_DO_SHORT_BATTERY
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           open load detection / short circuit to UBAT detection \n                           #IO_E_DO_OPEN_LOAD_OR_SHORT_BATTERY
 *                                                                                                                           (if a pull up resistor is configured) \n
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           low pass of digital outputs with analog feedback is being tuned in. \n             #IO_E_DO_DIAG_TRANSIENT_OSC
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           Over current protection. \n                                                        #IO_E_PROT_USER_OVERLOAD\n
 *                                                                                                                           Be reminded that the output protection will return these errors not only if        #IO_E_PROT_TEMP_OVERLOAD\n
 *                                                                                                                           the output current is too high but if one of the following conditions are met: \n  #IO_E_PROT_ACTIVE\n
 *                                                                                                                           - over temperature \n                                                              #IO_E_PROT_FATAL\n
 *                                                                                                                           - Supply for power stages disconnected                                             #IO_E_PROT_REENABLE
 *                                                                                                                             (due to loss of gate drive for reverse polarity protection)\n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_ADC_40 \n      \ref IO_ADC.h "Analog Input"                no built-in diagnostic functions.
 *                                                            #IO_ADC_41 \n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_30 \n       \ref IO_DIO.h "Digital Input"\n             open load detection / short circuit to GND detection \n                            #IO_E_DI_OPEN_LOAD_OR_SHORT_CIRCUIT
 *                                                            #IO_DI_31 \n       (with proper limits configured)             (if a pull down resistor is configured) \n
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short circuit to UBAT detection \n                                                 #IO_E_DI_SHORT_BATTERY
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           invalid voltage level detection (i.e. outside of specified limits) \n              #IO_E_DI_INVALID_VOLTAGE
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short to GND detection \n                                                          #IO_E_DI_SHORT_CIRCUIT
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           open load detection \n                                                             #IO_E_DI_OPEN_LOAD
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_PWD_22 \n      \ref IO_PWD.h "Digital Timer Input"         measured signal period is too small                                                #IO_E_PWD_CAPTURE_ERROR
 * +                                                          #IO_PWD_23 \n                                              ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           time measurement not finished yet                                                  #IO_E_PWD_NOT_FINISHED
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           timer overflow occurred                                                            #IO_E_PWD_TIMER_OVERFLOW
 * --------------------------------------- ---------------- ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *   <b>Digital output groups</b>
 * ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   \ref IO_DIO.h "Digital Output" \n       #IO_PIN_B1 \n     #IO_DO_10 \n      \ref IO_DIO.h "Digital Output"              open load detection \n                                                             #IO_E_DO_OPEN_LOAD
 * + low-side with analog feedback           #IO_PIN_A1 \n     #IO_DO_11 \n                                              ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short circuit to GND detection \n                                                  #IO_E_DO_SHORT_CIRCUIT
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short circuit to UBAT detection \n                                                 #IO_E_DO_SHORT_BATTERY
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           open load detection / short circuit to UBAT detection \n                           #IO_E_DO_OPEN_LOAD_OR_SHORT_BATTERY
 *                                                                                                                           (if a pull up resistor is configured) \n
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           low pass of digital outputs with analog feedback is being tuned in. \n             #IO_E_DO_DIAG_TRANSIENT_OSC
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           Over current protection. Be reminded that the output protection will \n            #IO_E_PROT_USER_OVERLOAD\n
 *                                                                                                                           return these errors not only if the output current is too high but if one of       #IO_E_PROT_TEMP_OVERLOAD\n
 *                                                                                                                           the following conditions are met: \n                                               #IO_E_PROT_ACTIVE\n
 *                                                                                                                           - Over temperature \n                                                              #IO_E_PROT_FATAL\n
 *                                                                                                                           - Supply for power stages disconnected                                             #IO_E_PROT_REENABLE
 *                                                                                                                             (due to loss of gate drive for reverse polarity protection)\n
 *                                                                                                                           - OVP circuitry has been tripped \n
 *                                                                                                                           - External watchdog has disabled the output stages
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_ADC_28 \n      \ref IO_ADC.h "Analog Input"                no built-in diagnostic functions.
 *                                                            #IO_ADC_29 \n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_22 \n       \ref IO_DIO.h "Digital Input"\n             open load detection \n                                                             #IO_E_DI_OPEN_LOAD
 * +                                                          #IO_DI_23 \n       (with proper limits configured)         ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short circuit to UBAT detection \n                                                 #IO_E_DI_SHORT_BATTERY
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short circuit to GND detection \n                                                  #IO_E_DI_SHORT_CIRCUIT
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           invalid voltage level detection (i.e. outside of specified limits) \n              #IO_E_DI_INVALID_VOLTAGE
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           open load / short circuit to GND detection \n                                      #IO_E_DI_OPEN_LOAD_OR_SHORT_CIRCUIT
 *                                                                                                                           (if a pull down resistor is configured) \n
 * --------------------------------------- ---------------- ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *   <b>Analog input groups</b>
 * ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   \ref IO_ADC.h "4-Mode ADC" \n           #IO_PIN_J4\n     #IO_ADC_00 \n      \ref IO_ADC.h "Analog Input"                configuration switch protection                                                    #IO_E_FET_PROTECTION
 *                                           #IO_PIN_H4\n     #IO_ADC_01 \n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_LED_00 \n      \ref IO_LED.h "LED Switch"                  an over current was detected                                                       #IO_E_FET_PROTECTION
 *                                                            #IO_LED_01 \n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_00 \n       \ref IO_DIO.h "Digital Input"\n             open load detection / short circuit to GND detection \n                            #IO_E_DI_OPEN_LOAD_OR_SHORT_CIRCUIT
 *                                                            #IO_DI_01 \n       (with proper limits configured)             (if a pull down resistor is configured) \n
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short circuit to UBAT detection \n                                                 #IO_E_DI_SHORT_BATTERY
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           invalid voltage level detection (i.e. outside of specified limits) \n              #IO_E_DI_INVALID_VOLTAGE
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short to GND detection \n                                                          #IO_E_DI_SHORT_CIRCUIT
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           open load detection \n                                                             #IO_E_DI_OPEN_LOAD
 * --------------------------------------- ---------------- ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *   \ref IO_ADC.h "3-Mode ADC" \n           #IO_PIN_E4\n     #IO_ADC_10 \n      \ref IO_ADC.h "Analog Input"                configuration switch protection                                                    #IO_E_FET_PROTECTION
 *                                           #IO_PIN_D4\n     #IO_ADC_11 \n
 *                                           #IO_PIN_C4\n     #IO_ADC_12 \n
 *                                           #IO_PIN_B4\n     #IO_ADC_13 \n
 *                                           #IO_PIN_A4\n     #IO_ADC_14 \n
 *                                           #IO_PIN_A3\n     #IO_ADC_15 \n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_LED_02 \n      \ref IO_LED.h "LED Switch"                  an over current was detected                                                       #IO_E_FET_PROTECTION
 *                                                            #IO_LED_03 \n
 *                                                            #IO_LED_04 \n
 *                                                            #IO_LED_05 \n
 *                                                            #IO_LED_06 \n
 *                                                            #IO_LED_07 \n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_10 \n       \ref IO_DIO.h "Digital Input"\n             open load detection / short circuit to GND detection \n                            #IO_E_DI_OPEN_LOAD_OR_SHORT_CIRCUIT
 *                                                            #IO_DI_11 \n       (with proper limits configured)             (if a pull down resistor is configured) \n
 * +                                                          #IO_DI_12 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_13 \n                                                   short circuit to UBAT detection \n                                                 #IO_E_DI_SHORT_BATTERY
 * +                                                          #IO_DI_14 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_15 \n                                                   invalid voltage level detection (i.e. outside of specified limits) \n              #IO_E_DI_INVALID_VOLTAGE
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short to GND detection \n                                                          #IO_E_DI_SHORT_CIRCUIT
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           open load detection \n                                                             #IO_E_DI_OPEN_LOAD
 * --------------------------------------- ---------------- ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *   \ref IO_ADC.h "1-Mode ADC" \n           #IO_PIN_G4\n     #IO_ADC_20 \n      \ref IO_ADC.h "Analog Input"                no built-in diagnostic functions.
 *                                           #IO_PIN_G4\n     #IO_ADC_21 \n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_02 \n       \ref IO_DIO.h "Digital Input"\n             open load detection / short circuit to GND detection \n                            #IO_E_DI_OPEN_LOAD_OR_SHORT_CIRCUIT
 *                                                            #IO_DI_03 \n       (with proper limits configured)             (if a pull down resistor is configured) \n
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short circuit to UBAT detection \n                                                 #IO_E_DI_SHORT_BATTERY
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           invalid voltage level detection (i.e. outside of specified limits) \n              #IO_E_DI_INVALID_VOLTAGE
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short to GND detection \n                                                          #IO_E_DI_SHORT_CIRCUIT
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           open load detection \n                                                             #IO_E_DI_OPEN_LOAD
 * --------------------------------------- ---------------- ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *   <b>Digital timer input groups</b>
 * ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   \ref IO_PWD.h "Digital Timer Input"     #IO_PIN_E3\n     #IO_PWD_00 \n      \ref IO_PWD.h "Digital Timer Input"         measured signal period is too small                                                #IO_E_PWD_CAPTURE_ERROR
 * +                                         #IO_PIN_D3\n     #IO_PWD_01 \n                                              ------------------------------------------------------------------------------------ -----------------------------------------
 *                                           #IO_PIN_C3\n     #IO_PWD_02 \n                                                  time measurement not finished yet                                                  #IO_E_PWD_NOT_FINISHED
 * +                                         #IO_PIN_B3\n     #IO_PWD_03 \n                                              ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           timer overflow occurred                                                            #IO_E_PWD_TIMER_OVERFLOW
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           only a constant high level is detected                                             #IO_E_PWD_HIGH_LEVEL
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           only a constant low level is detected                                              #IO_E_PWD_LOW_LEVEL
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_ADC_30 \n      \ref IO_ADC.h "Analog Input"                no built-in diagnostic functions.
 *                                                            #IO_ADC_31 \n
 *                                                            #IO_ADC_32 \n
 *                                                            #IO_ADC_33 \n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_04 \n       \ref IO_DIO.h "Digital Input"\n             open load detection / short circuit to GND detection \n                            #IO_E_DI_OPEN_LOAD_OR_SHORT_CIRCUIT
 *                                                            #IO_DI_05 \n       (with proper limits configured)             (if a pull down resistor is configured) \n
 * +                                                          #IO_DI_06 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_07 \n                                                   short circuit to UBAT detection \n                                                 #IO_E_DI_SHORT_BATTERY
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           invalid voltage level detection (i.e. outside of specified limits) \n              #IO_E_DI_INVALID_VOLTAGE
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short to GND detection \n                                                          #IO_E_DI_SHORT_CIRCUIT
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           open load detection \n                                                             #IO_E_DI_OPEN_LOAD
 * --------------------------------------- ---------------- ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *   <b>PVG output groups</b>
 * ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   \ref IO_PVG.h "PVG Output"              #IO_PIN_K2\n     #IO_PVG_00 \n      \ref IO_PVG.h "PVG Output"                  short circuit to UBAT detection \n                                                 #IO_E_PVG_SHORT_BATTERY
 * +                                         #IO_PIN_J2\n     #IO_PVG_01 \n                                              ------------------------------------------------------------------------------------ -----------------------------------------
 *                                           #IO_PIN_H2\n     #IO_PVG_02 \n                                                  short circuit to GND detection \n                                                  #IO_E_PVG_SHORT_CIRCUIT
 * +                                         #IO_PIN_G2\n     #IO_PVG_03 \n                                              ------------------------------------------------------------------------------------ -----------------------------------------
 *                                           #IO_PIN_F2\n     #IO_PVG_04 \n                                                  the PVG output is disabled \n                                                      #IO_E_PVG_OUTPUT_DISABLED
 * +                                         #IO_PIN_E2\n     #IO_PVG_05 \n                                              ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           over current protection \n                                                         #IO_E_PROT_ACTIVE
 *                                                                                                                                                                                                              #IO_E_PROT_REENABLE
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_VOUT_00 \n     \ref IO_VOut.h "Voltage Output"             short circuit to UBAT detection                                                    #IO_E_VOUT_SHORT_BATTERY
 * +                                                          #IO_VOUT_01 \n                                             ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_VOUT_02 \n                                                 short circuit to GND detection                                                     #IO_E_VOUT_SHORT_CIRCUIT
 * +                                                          #IO_VOUT_03 \n                                             ------------------------------------------------------------------------------------ -----------------------------------------
**                                                            #IO_VOUT_04 \n                                                 the configured output voltage was not reached within settling time                 #IO_E_VOUT_PRECISION
 * +                                                          #IO_VOUT_05 \n                                             ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           the voltage output is disabled                                                     #IO_E_VOUT_OUTPUT_DISABLED
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           over current protection \n                                                         #IO_E_PROT_ACTIVE
 *                                                                                                                                                                                                              #IO_E_PROT_REENABLE
 *
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_ADC_22 \n      \ref IO_ADC.h "Analog Input"                no built-in diagnostic functions.
 *                                                            #IO_ADC_23 \n
 *                                                            #IO_ADC_24 \n
 *                                                            #IO_ADC_25 \n
 *                                                            #IO_ADC_26 \n
 *                                                            #IO_ADC_27 \n
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_16 \n       \ref IO_DIO.h "Digital Input"\n             open load detection / short circuit to GND detection \n                            #IO_E_DI_OPEN_LOAD_OR_SHORT_CIRCUIT
 *                                                            #IO_DI_17 \n       (with proper limits configured)             (if a pull down resistor is configured) \n
 * +                                                          #IO_DI_18 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_18 \n                                                   short circuit to UBAT detection \n                                                 #IO_E_DI_SHORT_BATTERY
 * +                                                          #IO_DI_19 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DI_20 \n                                                   invalid voltage level detection (i.e. outside of specified limits) \n              #IO_E_DI_INVALID_VOLTAGE
 * +                                                          #IO_DI_21 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           short to GND detection \n                                                          #IO_E_DI_SHORT_CIRCUIT
 * +                                                                                                                     ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           open load detection \n                                                             #IO_E_DI_OPEN_LOAD
 * +                                                        ----------------- ------------------------------------------ ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DO_30 \n       \ref IO_DIO.h "Digital Output"              short circuit to UBAT detection \n                                                 #IO_E_DO_SHORT_BATTERY
 * +                                                          #IO_DO_31 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DO_32 \n                                                   short circuit to GND detection \n                                                  #IO_E_DO_SHORT_CIRCUIT
 * +                                                          #IO_DO_33 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                            #IO_DO_34 \n                                                   the digital output is disabled \n                                                  #IO_E_DO_OUTPUT_DISABLED
 * +                                                          #IO_DO_35 \n                                               ------------------------------------------------------------------------------------ -----------------------------------------
 *                                                                                                                           over current protection \n                                                         #IO_E_PROT_ACTIVE
 *                                                                                                                                                                                                              #IO_E_PROT_REENABLE
 * ======================================= ================ ================= ========================================== ==================================================================================== =========================================
 * \endverbatim
 *
 **************************************************************************/

/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/

#include "ptypes_xe167.h"

/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/** Every driver function returns an error code of type IO_ErrorType.
  * Refer to \ref err_values_doc "Error Values" for a documentation of the
  * possible values
  */
typedef ubyte2  IO_ErrorType;

/**************************************************************************
 *
 * D E F I N I T I O N S
 *
 **************************************************************************/

/*
 * E R R O R   V A L U E S
 */

/**
 * \name Error Values
 * \anchor err_values_doc
 *
 * Errors codes that a function might return in IO_ErrorType.
 */
/*@{*/
/** everything is fine, no error has occurred. */
#define IO_E_OK                                 0

/** Module or function is busy. \n
 *  This error is reported if a function or module has not yet finished its
 *  task. For example the EEPROM write function will return this error code
 *  if a previous write command has not been finished yet. Or if a channel
 *  is still initializing (e.g. during startup, changing modes, ...) and
 *  therefore not ready so far.
 */
#define IO_E_BUSY                               2

/** General error. No further information can be provided. */
#define IO_E_UNKNOWN                            3

 /** Global safety configuration is missing. \n
  *  This error is reported if an IO is defined as safety critical although
  *  no safety configuration has been passed to the IO_Driver_Init()
  *  (parameter \c safety_conf) function. An IO pin is considered as safety
  *  critical if a valid safety configuration has been passed to the
  *  init function (see \c safety_conf parameter of the functions
  *  IO_ADC_ChannelInit(), IO_PWM_Init(), IO_PWD_IncInit() and IO_PWD_ComplexInit() ).
  */
#define IO_E_DRV_SAFETY_CONF_NOT_CONFIG         20

 /** The safety configuration for the channel to be configured is invalid. \n
  *  This error is reported if a parameter in the safety configuration structure
  *  used for configuring a IO channel is wrong (see \c safety_conf parameter
  *  of the functions IO_ADC_ChannelInit(), IO_PWM_Init(),
  *  IO_PWD_IncInit() and IO_PWD_ComplexInit() ).
  */
#define IO_E_INVALID_SAFETY_CONFIG              21

 /** The given channel does not support to be configured safety critical. \n
  *  This error is reported if an IO channel is configured as safety critical
  *  but does not support this feature (reported by
  *  IO_ADC_ChannelInit(), IO_PWM_Init() ).
  */
#define IO_E_SAFETY_NOT_SUPPORTED               22

/** The ECU is already in safe state. \n
 *  This error code is reported by the function \c DIAG_EnterSafestate if
 *  the ECU is already in the safe state at the time the application requests
 *  to switch to the safe state.
 */
#define IO_E_ECU_ALREADY_IN_SAFE_STATE          23

/** CRC checksum wrong. \n
 *  This error code is reported if the CRC calculation of the
 *  production data block has failed.
 */
#define IO_E_INVALID_CRC                        24

/** No safety switch is configured. \n
 *  This error is reported when trying to access (e.g. en- or disabling) a safety switch
 *  but no internal or external safety switch is configured.
 */
#define IO_E_NO_SAFETY_SWITCH_CONFIGURED        25

/**  The common driver init function \c IO_Driver_Init() has been already called. \n
 *   This error code is reported by a function if it is called after the common
 *   driver init function \c IO_Driver_Init() has been called.
 */
#define IO_E_DRIVER_INITIALIZED                 26

/** The instruction is not permitted in the current diagnostic state. \n
 *  This error code is reported if the instruction is not permitted in the current state of
 *  the diagnostic state machine (e.g. en-/disabling an IO Pin during \c DIAG_STATE_STARTUP).
 */
#define IO_E_INVALID_DIAG_STATE                 27

/** A NULL pointer has been passed to the function. \n
 *  This error is reported if a non-optional pointer parameter of the function has been set to NULL.
 */
#define IO_E_NULL_POINTER                       30

/** An invalid parameter has been passed to the function. \n
 *  This error is reported if at least one of the parameters which have been passed to the function is outside the allowed range.
 */
#define IO_E_INVALID_PARAMETER                  31

/** The IO channel (IO pin) is busy. \n
 *  This error is reported if an IO Pin has been initialized before.
 *  To change the configuration of the channel during runtime the according De-Init function needs
 *  to be called before the channel can be again initialized with a new configuration.
 */
#define IO_E_CHANNEL_BUSY                       32

/** The IO channel (IO pin) has not been initialized. \n
 *  This error is reported by an IO driver task function if the channel has not been initialized.
 *  To initialize the channel, the according Init function needs to be called.
 */
#define IO_E_CHANNEL_NOT_CONFIGURED             33

/** The IO channel (IO pin) does not exist. \n
 *  This error is reported if a non-existent channel ID has been passed to the function.
 */
#define IO_E_INVALID_CHANNEL_ID                 34

/** An internal switch (FET) has been disabled to protect the hardware from damages. \n
 *  If the current on an internal FET is too high, the FET will be switched off by software to protect it from destruction.
 *  After 1s timeout the driver tries to re-enable the FET. \n
 *  When a FET has been switched off by the protection mechanism, this error code will be returned by the respective
 *  task function. The measured values are therefore invalid and should not be used for further calculations.
 */
#define IO_E_FET_PROTECTION                     35

/** The periodic interrupt timer has not been initialized. \n
 *  This error code is reported if trying to disable the periodic interrupt timer although it has not been setup.
 */
#define IO_E_PERIODIC_NOT_CONFIGURED            36

/** The IO channel (IO pin) does not support the requested feature. \n
 *  Two conditions can lead to this error code:
 *  - For example when trying to initialize or use an ADC pin as PWM output.
 *  - When trying to initialize an IO for a pin function which is not available on the ECU variant.
 *    (For example when trying to initialize a PWD input but it is not physically mounted on the
 *    used ECU variant)
 */
#define IO_E_CH_CAPABILITY                      37

/** The common driver init function \c IO_Driver_Init() has not been called. \n
 *  This error code is reported by the IO-Driver init functions if the common
 *  driver init function \c IO_Driver_Init() has not been called.
 */
#define IO_E_DRIVER_NOT_INITIALIZED             38

 /** The IO channel (IO pin) cannot be configured due to conflicts with other IO channels. \n
  *  This error is reported by initialization functions if the configuration cannot be performed
  *  due to conflicts with other already configured IO pins.
  */
#define IO_E_GROUP_CONFLICT                     39

/** Message object or FIFO buffer overflow. \n
 *  This error is reported if CAN messages have been lost due to a full buffer.
 *  To avoid this error FIFO buffers can be used. If FIFO buffers are already used, try to increase the buffer size.
 */
#define IO_E_CAN_OVERFLOW                       40

/** A wrong or invalid handle has been used. \n
 *  This error is reported if:
 *   - a non-existent handle has been used.
 *   - if a write handle has been passed to a read function or vice versa
 *   - if a message object handle has been passed to a FIFO function or vice versa
 */
#define IO_E_CAN_WRONG_HANDLE                   41

/** No more message objects are available. \n
 *  The maximum number of available message objects has been reached.
 *  A single message object is needed to setting up a single message object with the function IO_CAN_ConfigMsg().
 *  When configuring a FIFO buffer with the function IO_CAN_ConfigFIFO() the number of needed message objects equals the size of the FIFO buffer.
 *  (single message objects are joined together to a FIFO buffer)
 */
#define IO_E_CAN_MAX_MO_REACHED                 42

/** No more message handles are available. \n
 *  The maximum number of message handles has been reached.
 *  A message handle is generated every time the function IO_CAN_ConfigMsg() or IO_CAN_ConfigFIFO() is called without returning an error.
 */
#define IO_E_CAN_MAX_HANDLES_REACHED            43

/** The FIFO buffer is full. \n
 *  When reading: The respective FIFO buffer is full, but no data has been lost. \n
 *  When writing: The data has not been accepted by the driver. The application needs to transmit it again when there is enough space in the buffer.
 */
#define IO_E_CAN_FIFO_FULL                      44

/** No new data is available. \n
 *  This error is returned if no CAN frame has been received since the last successful read.
 */
#define IO_E_CAN_OLD_DATA                       45

/** The CAN node is in error passive state. */
#define IO_E_CAN_ERROR_PASSIVE                  46

/** The CAN node is in bus off state. \n
 *  The bus-off recovery sequence is triggered by calling the function
 *  IO_CAN_MsgStatus() or IO_CAN_FIFOStatus().
 */
#define IO_E_CAN_BUS_OFF                        47

/** Received data is invalid. \n
 *  Read again to get the valid data.
 */
#define IO_E_CAN_INVALID_DATA                   48

/** The trigger mechanism for the external watchdog has been disabled.\n
 *  This error code is returned when the diagnostic part of the IO-Driver
 *  has activated the Safe-State!
 */
#define IO_E_WD_TRIGGER_DISABLED                50

 /** The trigger mechanism for the external watchdog has been temporary
  *  disabled for diagnosis of the watchdog.
  */
 #define IO_E_WD_TRIGGER_TEMPORARY_DISABLED     51

 /** Configuring/triggering/disabling of internal watchdog via API functions
  * is only possible if IO Driver is configured as non-safety relevant.
  * If IO Driver is configured as safety relevant configuration and triggering/disabling
  * is performed via functions IO_Driver_Init() / IO_Driver_TaskBegin() automatically
  */
 #define IO_E_WD_INT_ONLY_NON_SAFETY            52


/** Invalid address range. \n
 *  This error is reported if read or write operations are requested for
 *  non-existent EEPROM addresses.
 */
#define IO_E_EEPROM_RANGE                       60

/** EEPROM buffer overrun. \n
 *  An internal SPI buffer has reported an overrun, data was lost.
 */
#define IO_E_EEPROM_BUFFER_FULL                 61

 /** Error on CRC calculation. \n
  *  The checksum stored in the EEPROM and the calculated one do not match.\n
  *  The read data contains errors.
  */
#define IO_E_EEPROM_CRC_MISMATCH                62

/** The UART software buffer is full. \n
 *  When receiving: Too much data has been received since the last successful read operation - data has been lost. \n
 *  When transmitting: The given data does not fit into the buffer, data has been rejected. Try again when there is more space in the buffer.
 */
#define IO_E_UART_BUFFER_FULL                   70

/** A UART software buffer is empty. \n
 *  This error code is used internally by the UART driver. It is not reported by any API function.
 */
#define IO_E_UART_BUFFER_EMPTY                  71

/** Overflow in the UART hardware buffer. \n
 *  The hardware buffer reported an overflow. This happens if to much data has been received between two consecutive SW cycles. \n
 *  To avoid this problem the application can call the function IO_UART_Task() at any time.
 *  This function copies the data from the hardware buffer to the software buffer.
 */
#define IO_E_UART_OVERFLOW                      72

/** UART parity error. \n
 *  The received parity bit doesn't match the calculated one.
 */
#define IO_E_UART_PARITY                        73

/** Maximum number of SPI devices reached. \n
 *  This error code is used internally by the SPI driver. It is not reported by any API function.
 */
#define IO_E_SPI_MAX_DEV_REACHED                80

/** SPI hardware buffer is full. \n
 *  This error code is used internally by the SPI driver. It is not reported by any API function.
 */
#define IO_E_SPI_BUFFER_FULL                    81


/** The reported ADC value is invalid or not available. \n
 *  This error is reported in two cases. First, if an ADC value will be read
 *  out immediately after initializing the ADC channels and the ADC conversion
 *  of the respective channel has not been started yet (after startup). \n
 *  Secondly, this error is reported during runtime if a conversion error has
 *  been detected by the IO-Driver which means that the ADC did not convert
 *  the channel in the last conversion cycle.
 */
#define IO_E_ADC_INVALID                        92

 /** The given ADC channel is in its initialization phase and the low-pass
  *  filter of the analog input is still during tune-in. \n
  *  The initialization phase takes about 20ms. The ADC value which is
  *  returned is not valid.
  */
#define IO_E_ADC_CHANNEL_STARTUP                93

/** A capture error occurred on a PWM loop-back channel. \n
 *  This error code can be the result of a stuck measurement timer or if two
 *  edges of the measured signal are too close to each other, and the internal
 *  timer cannot measure the time difference anymore.
 *
 *  \par PWM output with timer feedback in PWM output mode:
 *  This error is reported if two edges of the measured signal are too close
 *  to each other, and the internal timer cannot measure the time difference
 *  anymore (for example spikes caused by the switching of inductive loads
 *  in electric motors).
 */
#define IO_E_PWM_CAPTURE_ERROR                  100

/** The timer feedback channel of a PWM output has not yet finished the
 *  pulse-width and period measurement.\n
 *  This error code is currently being suppressed and only used internally.
 *  Currently it will not be returned by any API function
 */
#define IO_E_PWM_NOT_FINISHED                   101

/** An open load condition has been detected. \n
 *  This error is reported if the output signal cannot be measured via
 *  the timer feedback and the level on the analog feedback channel
 *  corresponds to the device-internal pull-up resistor.
 *
 *  \remarks
 *  No open load detection is available if the power stages have been disabled.
 */
#define IO_E_PWM_OPEN_LOAD                      102

/** A short circuit condition has been detected. \n
 *  This error is reported if the output signal cannot be measured via the
 *  timer feedback and the level on the feedback channel is low (ground level)
 *  This means that no edges could be captured on the timer feedback channel
 *  within a timeout of 4 times of the PWM period time.
 *
 *  The error condition is reset as soon as the timer was able to capture
 *  a complete PWM period (3 edges).
 *
 *  \remarks
 *  No short circuit detection is available if the power stages have been disabled.
 *  Refer to the \ref IO_POWER.h "ECU power functions" for further details.
 */
#define IO_E_PWM_SHORT_CIRCUIT                  103

/** A short circuit to battery condition has been detected. \n
 *  This error is reported if the output signal cannot be measured via
 *  the timer feedback and the level on the analog feedback channel is larger
 *  than 0.8 times UBAT.
 *
 *  \remarks
 *  No short circuit detection is available if the power stages have been disabled.
 *  Refer to the \ref IO_POWER.h "ECU power functions" for further details.
 */
#define IO_E_PWM_SHORT_BATTERY                  104

/** An open load or short to battery condition has been detected. \n
 *  At the point of time when this error is returned, it was not yet possible
 *  to distinguish between open load or short to battery. After 50ms -- if the
 *  error is still present -- depending on the voltage level at the output pin
 *  the following errors will be returned:
 *  - #IO_E_PWM_SHORT_BATTERY if voltage is near battery voltage
 *  - #IO_E_PWM_OPEN_LOAD if voltage is in open load range
 *  - Still #IO_E_PWM_OPEN_LOAD_OR_SHORT_BATTERY if the IO driver was not
 *    able to precisely determine which error occurred.
 */
#define IO_E_PWM_OPEN_LOAD_OR_SHORT_BATTERY     105

/** The PWM output is in its startup phase. \n
 *  For PWM channels with current measurement the current
 *  measurement is being calibrated during this phase.
 */
#define IO_E_PWM_CHANNEL_STARTUP                107

/** The PWM output has been switched off because a error occurred during it's startup phase. \n
 *  For PWM channels with current measurement this means that the offset
 *  of the current measurement is out of range. For reasons to protect
 *  the current measurement hardware (electric shunt) this
 *  output will stay disabled for the remaining driving cycle.
 */
#define IO_E_PWM_OUTPUT_STARTUP_ERROR           108

/** PWM outputs are disabled. \n
 *  This error is reported if the high-side powerstages are disabled via #IO_POWER_Set
 *  or not yet enabled after ECU startup. No diagnosis is possible during this stage.
 *
 */
#define IO_E_PWM_OUTPUT_DISABLED                109

/** A low level has been detected on a PWM channel. \n
 *  This error is returned, if the diagnostic margin of a PWM channel has been
 *  deactivated and the duty cycle exceeds the lower diagnostic margin of 100us.
 *  In this state -- due to the low duty cycle -- a short to ground situation
 *  cannot be distinguished from a faultless situation.
 *
 *  \remarks
 *  This is no error, but an information for the application software that due to
 *  the low duty cycle only reduced diagnosis is available.
 */
#define IO_E_PWM_OUTPUT_LOW                     110

 /** A high level has been detected on a PWM channel. \n
  *  This error is returned, if the diagnostic margin of a PWM channel has
  *  been deactivated and the duty cycle exceeds the higher diagnostic margin
  *  of 250us. In this state -- due to the high duty cycle --  a short to
  *  battery situation cannot be distinguished from a faultless situation.
  *
  *  \remarks
  *  This is no error, but an information for the application software that due to
  *  the high duty cycle only reduced diagnosis is available.
  */
#define IO_E_PWM_OUTPUT_HIGH                    111

/** The diagnostic functions on a PWM output with analog feedback have been
 *  temporarily disabled. \n
 *  This error code is reported if a error is detected on a PWM output with
 *  disabled diagnostic margin. Due to the low pass filter in the feedback
 *  path, the diagnostic functions are disabled for 50ms after a change of the
 *  output level to avoid wrongly reported errors during the settling time
 *  of the filter. \n
 *
 *  \remarks
 *  This is no error but a information for the application software that
 *  currently no output diagnostic is available on this channel. If this code
 *  is returned the application software should wait until the error #IO_E_OK
 *  is returned before changing the output value again. Otherwise no diagnostic
 *  information can be provided.
 */
#define IO_E_PWM_DIAG_TRANSIENT_OSC             112

 /** Current measurement inaccurate.\n
  *  This error is reported if the values for zero current compensation
  *  which are stored in the PDB are out of range or do not match the values
  *  which are measured during startup.
  */
 #define IO_E_PWM_CURRENT_INACCURATE            113

/** A timer overflow occurred. \n
 *  This error code appears when the 24bit timer overflows.
 *  The time depends on the timer resolution which can be configured with the respective initialization function.
 *
 *  \remarks
 *  timer_res = 0.2us -> max. period of input signal to measure =  3.34s \n
 *  timer_res = 0.4us -> max. period of input signal to measure =  6.68s \n
 *  timer_res = 0.8us -> max. period of input signal to measure = 13.36s \n
 *  timer_res = 1.6us -> max. period of input signal to measure = 26.73s \n
 *  timer_res = 3.2us -> max. period of input signal to measure = 53.47s
 */
#define IO_E_PWD_TIMER_OVERFLOW                 120

/** A constant high level has been detected on a timer channel.\n
 *
 *  \par PWM output with timer feedback in digital timer input mode:
 *  This error is reported if no edges are captured for 100ms and a high level is detected on the input pin.
 */
#define IO_E_PWD_HIGH_LEVEL                     121

/** A constant low level has been detected on a timer channel.\n
 *
 *  \par PWM output with timer feedback in digital timer input mode:
 *  This error is reported if no edges are captured for 100ms and a low level is detected on the input pin.
 */
#define IO_E_PWD_LOW_LEVEL                      122

/** A capture error occurred on a timer channel \n
 *
 *  \par PWM output with timer feedback in digital timer input mode:
 *  This error is reported if two edges of the measured signal are too close to each other,
 *  and the internal timer cannot measure the time difference anymore (for example spikes caused
 *  by the switching of inductive loads in electric motors).
 *
 *  \par Digital Timer input:
 *  This error is reported if two edges of the measured signal are too close to each other,
 *  and the internal timer cannot measure the time difference anymore (for example spikes caused
 *  by the switching of inductive loads in electric motors).
 */
#define IO_E_PWD_CAPTURE_ERROR                  123

/** The timer channel has not yet finished the measurement. \n
 *
 *  \par Digital timer input in digital timer input mode:
 *  Depending on the configuration a certain number of signal edges are required for a timing measurement.
 *  This error is reported if not all edges have been captured.
 *  If the task function is called multiple times during a period of the signal which shall be measured,
 *  the function will return #IO_E_OK only if valid data is available, otherwise it returns this error code.\n
 *  This error is also reported if no edges have been captured at all.
 */
#define IO_E_PWD_NOT_FINISHED                   124

/** The digital output is in it's startup phase.\n
 *  For digital output channels with current measurement the current
 *  measurement is being calibrated during this phase.
 */
#define IO_E_DO_CHANNEL_STARTUP                 130

/** The digital output could not be started up. \n
 *  This error code is related to digital output channels with current measurement
 *  and describes that the offset of the current measurement is out of range.
 *  To protect the current measurement hardware (electric shunt) this output will
 *  stay disabled for the remaining driving cycle.
 */
#define IO_E_DO_OUTPUT_STARTUP_ERROR            131

/** An open load condition has been detected
 *  on a digital output.
 */
#define IO_E_DO_OPEN_LOAD                       132

/** A short circuit to ground condition has been detected
 *  on a digital output.
 */
#define IO_E_DO_SHORT_CIRCUIT                   133

/** A short circuit to battery voltage condition has been detected
 *  on a digital output.
 */
#define IO_E_DO_SHORT_BATTERY                   134

/** An open load condition or a short circuit to battery voltage
 *  has been detected on a digital output.
 */
#define IO_E_DO_OPEN_LOAD_OR_SHORT_BATTERY      135

/** The diagnostic functions on a digital output with analog feedback have been
 *  temporarily disabled. \n
 *
 *  \par PWM output in digital output mode:
 *  This error code is reported if the output value of a digital output with
 *  analog feedback has been changed. Due to the low pass filter in the feedback
 *  path, the diagnostic functions are disabled for 100 ms after a change of the
 *  output level to avoid wrongly reported errors during the settling time
 *  of the filter. \n
 *
 *  \remarks
 *  This is no error but a information for the application software that
 *  currently no output diagnostic is available on this channel. If this code
 *  is returned the application software should wait until the error #IO_E_OK
 *  is returned before changing the output value again. Otherwise no diagnostic
 *  information can be provided.
 */
#define IO_E_DO_DIAG_TRANSIENT_OSC              136

/**
 * Digital outputs are disabled.\n
 * This error is reported if the high-side powerstages are disabled via #IO_POWER_Set
 * or not yet enabled after ECU startup. No diagnosis is possible during this stage.
 *
 */
#define IO_E_DO_OUTPUT_DISABLED                 137

/**
 * Current measurement inaccurate.\n
 * This error is reported if the values for zero current compensation which are stored
 * in the PDB are out of range or do not match the values which are measured during startup.
 */
#define IO_E_DO_CURRENT_INACCURATE              138

/** An output has detected a situation that was specified as overload by the user.
 *
 *  \par PWM Outputs:
 *  If the measured current rises above the \c overload_limit specified upon initialization
 *  with IO_PWM_Init, the driver task-function will return this error code.
 *
 *  \par Digital Outputs:
 *  If the measured current rises above the \c overload_limit specified upon initialization
 *  with IO_DO_Init, the driver task-function will return this error code.
 *
 *  \par PVG/Voltage Outputs:
 *  For PVG/Voltage outputs there is no user configurable overload situation available.
 */
#define IO_E_PROT_USER_OVERLOAD                 140

/** An output has detected a temporary overload situation.
 *
 * \par High Side Outputs with Current Measurement:
 *  These channels can take 3.0A continuous current and up to 4A peak current
 *  for 1 second. If the current is above 3.0A the IO-Driver signals to the
 *  application with this error code that after 1 second the output stages will
 *  be switched off unless the current is decreasing to or below 3.0A. \n
 *  If the power output has been switched off, the error code #IO_E_PROT_ACTIVE
 *  will be reported.
 *
 * \par High Side Outputs with Overcurrent Protection:
 *  This error code is not returned by channels with slow overcurrent protection,
 *  as these outputs do allow a high current for 1 second.
 *  If the current is above 3.75A after 1 second the output stages will be
 *  switched off. \n
 *  If the power output has been switched off, the error code #IO_E_PROT_ACTIVE
 *  will be reported.
 *
 * \par Low-Side Digital Outputs:
 * \if TTC32_LAGE
 *  These channels can take 3.5A continuous current and up to 5.5A peak current for
 *  15 second. If the current of one low-side output is above 3.5A the IO-Driver
 *  signals to the application with this error code that after 15 second the
 *  affected low-side output will be switched off unless the current is decreasing
 *  to or below 3.5A. However, if the load current of the other (currently not affected)
 *  low-side output exceeds 2.5A, or the measured board temperature exceeds 85 degree
 *  celsius, the affected low-side output will be switched off immediately. \n
 *  If the low-side output has been switched off, the error code #IO_E_PROT_ACTIVE
 *  will be reported.
 * \else
 *  These channels can take 3.5A continuous current and up to 5.5A peak current for
 *  1 second. If the current is above 3.5A the IO-Driver signals to the application
 *  with this error code that after 1 second the affected low-side output will be
 *  switched off unless the current is decreasing to or below 3.5A. \n
 *  If the low-side output has been switched off, the error code #IO_E_PROT_ACTIVE
 *  will be reported.
 * \endif
 *
 * \par PVG/Voltage Outputs:
 *  There is no temporary overload situation for PVG/Voltage outputs.
 */
#define IO_E_PROT_TEMP_OVERLOAD                 141


/** An output has detected a overload situation and has been switched off
 *  to protect the hardware from damage.
 *
 * \par High Side Outputs with Current Measurement:
 *  If the current on a power output is too high, the output will be switched
 *  off by software to protect the system from thermal overload. The current
 *  limit for these outputs is 3.0A. The driver will switch off the output if
 *  the current is
 *  - between 3.0A and 4.0A for more than 1s
 *  - above 4.0A
 *  - above "6*duty_cycle"A (value duty_cycle from 0..1, 50% duty_cycle means
 *    that the maximum current can be 3A, 25% duty cycle means 1.5A) \n
 *  After a timeout of 1 second the driver tries to re-enable the output.
 *  When a output has been switched off by the protection mechanism, this error
 *  code will be returned by the respective task function. \n
 *
 * \par High Side Outputs with Overcurrent Protection:
 *  If the current on a power output is higher than
 *  - 3.75A for longer than 1 second,
 *  - 5A for longer than 250ms
 *  - 7.1A for longer than 128ms \n
 *  the output will be switched off by software to protect the system from
 *  thermal overload. After a timeout of 1 second the driver tries to re-enable
 *  the output (for the 7.1A limit the timeout is 10s).
 *  When a output has been switched off by the protection mechanism,
 *  this error code will be returned by the respective task function. \n
 *
 * \par Low-Side Digital Outputs:
 *  If the current on a low-side power output is too high, the output will be
 *  switched off by software to protect the system from thermal overload. The
 *  current limit for these outputs is 3.5A. The driver will switch off the affected
 *  low-side output if the current is:
 * \if TTC32_LAGE
 *  - between 3.5A and 5.5A for more than 15s or the load current of the other
 *    (currently not affected) low-side output exceeds 2.5A or the measured board
 *    temperature exceeds 85 degree celsius (see also #IO_E_PROT_TEMP_OVERLOAD).
 *  - above 5.5A \n
 *  After a timeout of 60 seconds the driver tries to re-enable the output.
 *  When an output has been switched off by the protection mechanism, this error
 *  code will be returned by the respective task function. \n
 * \else
 *  - between 3.5A and 5.5A for more than 1s (see also #IO_E_PROT_TEMP_OVERLOAD).
 *  - above 5.5A \n
 *  After a timeout of 1 second the driver tries to re-enable the output.
 *  When an output has been switched off by the protection mechanism, this error
 *  code will be returned by the respective task function. \n
 * \endif
 *
 *
 *  \par Common for all power outputs
 *  If the driver detects over temperature or loss of gate-drive of the reverse
 *  polarity protection the power outputs (Highside and Lowside) are switched
 *  off. During the time the outputs are switched off, this error code gets
 *  returned.
 *
 * \par PVG/Voltage Outputs:
 *  If the absolute value of the difference (Udiff) between the configured
 *  output voltage and the voltage measured with the analog feedback channel
 *  and is greater than 9.5V for more than 100ms, the output enters the
 *  protection state. \n
 *  Udiff = (Us - Ufb), where Us is the set output voltage and Ufb is the
 *  measured feedback voltage. \n
 *  If Udiff > 9.5V (e.g. short circuit to ground) the output is reduced to 25% for 1s.\n
 *  If Udiff < -9.5V (e.g. short circuit to battery) in the output is increased to 75% for 1s.\n
 */
#define IO_E_PROT_ACTIVE                        142

/**
 *  An output has been switched off due to a fatal overload condition
 *  to protect the hardware from damage.
 *
 * \par PWM outputs:
 *  If the current on these outputs rises above 5.4A, the output is switched
 *  off automatically.
 *  After a timeout of 1 second the driver tries to re-enable that output.
 *  When an output has been switched off by the protection-circuit, this error
 *  code will be returned by the respective task function. \n
 *
 * \par High Side Outputs with Overcurrent Protection:
 *  If the current on these output rises above 7.1A, output is switched
 *  off automatically.
 *  After a timeout of 10 seconds the driver tries to re-enable that output.
 *  When an output has been switched off by the protection-circuit, this error
 *  code will be returned by the respective task function. \n
 *
 * \par Low-Side Digital Outputs:
 *  If the current on these output rises above 7A, the protection-circuit gets
 *  activated and the affected output is switched off automatically.
 * \if TTC32_LAGE
 *  After a timeout of 60 second the driver tries to re-enable that output.
 * \else
 *  After a timeout of one second the driver tries to re-enable that output.
 * \endif
 *  When an output has been switched off by the protection-circuit, this error
 *  code will be returned by the respective task function. \n
 *
 * \par PVG/Voltage Outputs:
 *  These channels do not report fatal protection errors.
 */
#define IO_E_PROT_FATAL                         143

/** An output which has been switched off has been re-enabled.
 *
 * \par High Side Outputs with Current Measurement:
 *  If these outputs are switched off due to an over-current condition, the
 *  IO-Driver will try to re-enable the output after 1 second. During the
 *  re-enable phase the IO-Driver returns this error code. \n
 *  If the current is higher than the limits specified for the output,
 *  the power output will be switched off again during the re-enable phase.
 *
 * \par High-Side Digital Outputs:
 *  If these outputs are switched off due to an over-current condition,
 *  the IO-Driver will try to re-enable the output after 1 second. During the
 *  re-enable phase the IO-Driver returns this error code. \n
 *  If the current is higher than the limits specified for the output,
 *  the power output will be switched off again during the re-enable phase.
 *
 * \par Low-Side Digital Outputs:
 *  If these outputs are switched off due to an over-current condition,
 *  the IO-Driver will try to re-enable the output after 1 second. During the
 *  re-enable phase the IO-Driver returns this error code. \n
 *  If the current is higher than the limits specified for the output,
 *  the power output will be switched off again during the re-enable phase.
 *
 * \par PVG/Voltage Outputs:
 * If these outputs are switched off due to a large difference between
 * the measured and configured voltage, the IO-Driver will try to re-enable
 * the output after 1s.
 */
#define IO_E_PROT_REENABLE                      144

/** An output has been switched off permanently.
 *
 *  Reasons could be:
 *  - Diagnostic module entered safe state
 *  - ECU is being switched off via KL15-Hold by
 *    the application software
 */
#define IO_E_PROT_PERMANENT_OFF                 145

/** An open load condition has been detected on a digital input.
 */
#define IO_E_DI_OPEN_LOAD                       160

/** An open load or short circuit to ground has has been detected
 *  on a digital input.
 */
#define IO_E_DI_OPEN_LOAD_OR_SHORT_CIRCUIT      161

/** A short circuit to ground has has been detected on a digital input.
 */
#define IO_E_DI_SHORT_CIRCUIT                   162

/** An open load or short circuit to battery has has been detected
 *  on a digital input.
 */
#define IO_E_DI_SHORT_BATTERY                   163

/**
 *  A voltage level outside of the specified thresholds has been detected
 *  on a digital input.
 */
#define IO_E_DI_INVALID_VOLTAGE                 164

/** The voltage thresholds passed to #IO_DI_Init are not valid.
 */
#define IO_E_DI_INVALID_LIMITS                  165

/** The voltage levels on pins #IO_PIN_K3 (#IO_ADC_NODE_ID_0)
 *  and #IO_PIN_J3 (#IO_ADC_NODE_ID_1) represent no valid Node ID.
 */
#define IO_E_NODEID_PINS_INVALID                170

/**
 *  The Node ID stored in the EEPROM and the voltage levels on pins #IO_PIN_K3
 *  and #IO_PIN_J3 do not match.
 */
#define IO_E_NODEID_EEPROM_MISMATCH             171

 /**
  * The Node ID stored in the EEPROM was invalid on startup and overwritten
  * with the Node ID determined via pins #IO_PIN_K3 and #IO_PIN_J3.
  */
#define IO_E_NODEID_EEPROM_INVALID              172

 /** The bootloader used the (valid) values stored in the EEPROM to calculate
  *  the modifier as the voltage level on the pins #IO_PIN_K3 (#IO_ADC_NODE_ID_0)
  *  and #IO_PIN_J3 (#IO_ADC_NODE_ID_1) did not represent a valid modifier.
  */
 #define IO_E_NODEID_EEPROM_FALLBACK            173


/** If the measured voltage on the analog feedback of a PVG output is below
 *  5% of UBat, this error is reported by the step function.
 */
#define IO_E_PVG_SHORT_CIRCUIT                  180

/** If the measured voltage on the analog feedback of a PVG output is above
 *  95% of UBat, this error is reported by the step function.
 */
#define IO_E_PVG_SHORT_BATTERY                  181

/** PVG/Voltage outputs are disabled. \n
 *  This error is reported if the PVG/Voltage Outputs are disabled via #IO_POWER_Set
 *  or not yet enabled after ECU startup. No diagnosis is possible during this stage.
 */
#define IO_E_PVG_OUTPUT_DISABLED                182

/** A short circuit to ground condition has been detected on a voltage output. \n
 *  This error is reported if after the settling time of 150ms the configured
 *  voltage is not reached and the the voltage measured on the analog feedback
 *  is less than 1000mV.
 */
#define IO_E_VOUT_SHORT_CIRCUIT                 190

/** A short circuit to battery condition has been detected on a voltage output. \n
 *  This error is reported if after the settling time of 150ms the configured
 *  voltage is not reached and the the voltage measured on the analog feedback
 *  is greater than (UBat - 1000mV).
 */
#define IO_E_VOUT_SHORT_BATTERY                 191

/** PVG/Voltage outputs are disabled. \n
 *  This error is reported if the PVG/Voltage Outputs are disabled
 *  via #IO_POWER_Set or not yet enabled after ECU startup. No diagnosis
 *  can be applied while the output stage is disabled.
 */
#define IO_E_VOUT_OUTPUT_DISABLED               192

/** Voltage on the output is outside the allowed tolerance.\n
 *  If the configured output voltage is not reached after 150ms within a
 *  tolerance of (+/- 200mV), this error is returned by the step-function.
 */
#define IO_E_VOUT_PRECISION                     193

/** No unused PID controllers available. \n
 *  This error is reported when a PID controller is configured, but all
 *  available PID controller (#IO_PID_MAX_HANDLES in total) are already used
 */
#define IO_E_PID_NO_FREE_HANDLES                200

/**
 * PID controller is currently in use. \n
 * This error is reported if a PID controller is de-initialized while it is in use.
 * At present, this error code is not returned by any IO-Driver function.
 */
#define IO_E_PID_USED                           201

/** Internal error has occurred.\n
 *  This error can be caused by a SW or processor malfunction.
 */
#define IO_E_SW_INTERNAL                        220

/** An internal error within the output protection has occurred. \n
 *  An error within the state machine which handles the output protection for
 *  power outputs with current measurement occurred. This is an error
 *  caused by SW malfunction.
 */
#define IO_E_SW_OUTPROT_SM                      221

 /** Hardware type does not match.\n
  *  This error is caused by using an IO-Driver for the wrong device
  *  (e.g. TTC-30XH driver for TTC-30XI).\n
  *
  *  \attention The IO-Driver remains uninitialized and no out- or inputs will work.
  */
#define IO_E_WRONG_HW_TYPE                      230

/** Maximum number of tasks reached. \n
 *  Certain IOs use a task running in the background for managing protection
 *  mechanisms or executing PID controllers. If the number of free task slots
 *  reaches its maximum, this error code gets returned. The root cause of such a
 *  error is most probably a malfunction within the IO driver software.
 */
#define IO_E_TASK_NO_FREE_SLOTS                 240
 /** Discharging of capacitor failed. \n
  *  Discharge circuit(new feature for ECU HW V5.00) was not able to discharge capacitor in time.
  *  Probably HW set-up regarding external safety switch is wrong (see also HW user manual).
  */

#define IO_E_DISCHARGE_FAILED                   241
 /** Reset counter is invalid
  *  The content of the reset save CPU registers that are used to store the reset counter
  *  is inconsistent.
  */
 #define IO_E_RESET_COUNTER_INVALID             250


 /** Content of standby memory invalid
  *  The content of a section of the standby memory is inconsistent and can't be read.
  */
 #define IO_E_SBRAM_CONTENT_INVALID             251


/*@}*/


#endif /* _IO_CONSTANTS_H */
