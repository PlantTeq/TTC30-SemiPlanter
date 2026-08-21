
/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietory
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 **************************************************************************/
 /**********************************************************************//**
 * \file IO_Pins.h
 *
 * \brief Global IO Pin defines for IO Driver
 *
 *      This header file defines the IO Pins as well as the
 *      aliases for the IO Pins.
 *
 **************************************************************************/
/* GENERATED CODE (DO NOT MODIFY) */
/**********************************/
#ifndef _IO_PINS_H
#define _IO_PINS_H

 /**********************************************************************//**
 * 
 * \brief Enumeration of all (internal and external) pins. 
 *
 * Lists all used and available (virtual) I/O pins
 * for operating functionality as well as for diagnostic purposes. 
 *
 **************************************************************************/ 
typedef enum {
    IO_PIN_L2 = 0,    /**<Pin L2 \n
                *   <i>main function:</i> High-Side battery supply input. See \ref pin_feat_30x_adc_other "ADC input" (for battery measurement)\n
                *   <i>alternate functions:</i> none */  
    IO_PIN_L1 = 1,    /**<Pin L1 \n
                *   <i>main function:</i> CPU battery supply input. See \ref pin_feat_30x_adc_other "ADC input" (for battery measurement)\n
                *   <i>alternate functions:</i> none */  
    IO_PIN_K4 = 2,    /**<Pin K4 \n
                *   <i>main function:</i> Terminal 15 input. See \ref IO_POWER.h "Driver for ECU Power functions" for details\n
                *   <i>alternate functions:</i> none */  
    IO_PIN_H3 = 3,    /**<Pin H3 \n
                *   <i>main function:</i> Sensor supply output. See \ref pin_feat_30x_adc_other "ADC input" (for feedback measurement)\n
                *   <i>alternate functions:</i> none */  
    IO_PIN_K3 = 4,    /**<Pin K3 (function NOT available for HY-TTC32 and HY-TTC32S) \n
                *   <i>main function:</i> Node ID 0 input. See \ref pin_feat_30x_adc_other "ADC input" (for feedback measurement)\n
                *   <i>alternate functions:</i> none */  
    IO_PIN_J3 = 5,    /**<Pin J3 (function NOT available for HY-TTC32 and HY-TTC32S) \n
                *   <i>main function:</i> Node ID 1 input. See \ref pin_feat_30x_adc_other "ADC input" (for feedback measurement)\n
                *   <i>alternate functions:</i> none */  
    IO_PIN_G4 = 6,    /**<Pin G4 \n
                *       <i>main function:</i> \ref pin_feat_30x_adc_1m "1-Mode ADC Input"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_adc_1m_di "Digital Input"\n */  
    IO_PIN_F4 = 7,    /**<Pin F4 \n
                *       <i>main function:</i> \ref pin_feat_30x_adc_1m "1-Mode ADC Input"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_adc_1m_di "Digital Input"\n */  
    IO_PIN_E3 = 8,    /**<Pin E3 \n
                *       <i>main function:</i> \ref pin_feat_30x_pwd_comp_inc "Complex Timer Input (with incremental decoder)"\n
                *       <i>alternate functions:</i> \ref  pin_feat_30x_pwd_adc "Analog Input", \ref  pin_feat_30x_pwd_di "Digital Input", \ref pin_feat_30x_pwd_comp_inc "Complex Timer Input"\n */  
    IO_PIN_D3 = 9,    /**<Pin D3 \n
                *       <i>main function:</i> \ref pin_feat_30x_pwd_comp_inc "Complex Timer Input (with incremental decoder)"\n
                *       <i>alternate functions:</i> \ref  pin_feat_30x_pwd_adc "Analog Input", \ref  pin_feat_30x_pwd_di "Digital Input", \ref pin_feat_30x_pwd_comp_inc "Complex Timer Input"\n */  
    IO_PIN_C3 = 10,    /**<Pin C3 \n
                *       <i>main function:</i> \ref pin_feat_30x_pwd_comp "Complex Timer Input"\n
                *       <i>alternate functions:</i> \ref  pin_feat_30x_pwd_adc "Analog Input", \ref  pin_feat_30x_pwd_di "Digital Input"\n */  
    IO_PIN_B3 = 11,    /**<Pin B3 \n
                *       <i>main function:</i> \ref pin_feat_30x_pwd_comp "Complex Timer Input"\n
                *       <i>alternate functions:</i> \ref  pin_feat_30x_pwd_adc "Analog Input", \ref  pin_feat_30x_pwd_di "Digital Input"\n */  
    IO_PIN_J4 = 12,    /**<Pin J4 \n
                *       <i>main function:</i> \ref pin_feat_30x_adc_4mode "4-Mode ADC Input"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_adc_4mode_di "Digital Input", \ref pin_feat_30x_led "LED Output"\n */  
    IO_PIN_H4 = 13,    /**<Pin H4 \n
                *       <i>main function:</i> \ref pin_feat_30x_adc_4mode "4-Mode ADC Input"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_adc_4mode_di "Digital Input", \ref pin_feat_30x_led "LED Output"\n */  
    IO_PIN_E4 = 14,    /**<Pin E4 \n
                *       <i>main function:</i> \ref pin_feat_30x_adc_3mode "3-Mode ADC Input"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_adc_3mode_di "Digital Input", \ref pin_feat_30x_led "LED Output"\n */  
    IO_PIN_D4 = 15,    /**<Pin D4 \n
                *       <i>main function:</i> \ref pin_feat_30x_adc_3mode "3-Mode ADC Input"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_adc_3mode_di "Digital Input", \ref pin_feat_30x_led "LED Output"\n */  
    IO_PIN_C4 = 16,    /**<Pin C4 \n
                *       <i>main function:</i> \ref pin_feat_30x_adc_3mode "3-Mode ADC Input"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_adc_3mode_di "Digital Input", \ref pin_feat_30x_led "LED Output"\n */  
    IO_PIN_B4 = 17,    /**<Pin B4 \n
                *       <i>main function:</i> \ref pin_feat_30x_adc_3mode "3-Mode ADC Input"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_adc_3mode_di "Digital Input", \ref pin_feat_30x_led "LED Output"\n */  
    IO_PIN_A4 = 18,    /**<Pin A4 \n
                *       <i>main function:</i> \ref pin_feat_30x_adc_3mode "3-Mode ADC Input"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_adc_3mode_di "Digital Input", \ref pin_feat_30x_led "LED Output"\n */  
    IO_PIN_A3 = 19,    /**<Pin A3 \n
                *       <i>main function:</i> \ref pin_feat_30x_adc_3mode "3-Mode ADC Input"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_adc_3mode_di "Digital Input", \ref pin_feat_30x_led "LED Output"\n */  
    IO_PIN_H1 = 20,    /**<Pin H1 \n
                *       <i>main function:</i> \ref pin_feat_30x_pwm_cm "PWM Output with current measurement"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_pwm_cm_pwd "Timer Input", \ref pin_feat_30x_pwm_cm_do "High-Side Digital Output (with current measurement)", \ref pin_feat_30x_pwm_ocm_do "High-Side Digital Output", \ref pin_feat_30x_pwm_adc "Analog input", \ref pin_feat_30x_pwm_di "Digital input"\n */  
    IO_PIN_G1 = 21,    /**<Pin G1 \n
                *       <i>main function:</i> \ref pin_feat_30x_pwm_cm "PWM Output with current measurement"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_pwm_cm_pwd "Timer Input", \ref pin_feat_30x_pwm_cm_do "High-Side Digital Output (with current measurement)", \ref pin_feat_30x_pwm_ocm_do "High-Side Digital Output", \ref pin_feat_30x_pwm_adc "Analog input", \ref pin_feat_30x_pwm_di "Digital input"\n */  
    IO_PIN_F1 = 22,    /**<Pin F1 \n
                *       <i>main function:</i> \ref pin_feat_30x_pwm_cm "PWM Output with current measurement"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_pwm_cm_pwd "Timer Input", \ref pin_feat_30x_pwm_cm_do "High-Side Digital Output (with current measurement)", \ref pin_feat_30x_pwm_ocm_do "High-Side Digital Output", \ref pin_feat_30x_pwm_adc "Analog input", \ref pin_feat_30x_pwm_di "Digital input"\n */  
    IO_PIN_E1 = 23,    /**<Pin E1 \n
                *       <i>main function:</i> \ref pin_feat_30x_pwm_cm "PWM Output with current measurement"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_pwm_cm_pwd "Timer Input", \ref pin_feat_30x_pwm_cm_do "High-Side Digital Output (with current measurement)", \ref pin_feat_30x_pwm_ocm_do "High-Side Digital Output", \ref pin_feat_30x_pwm_adc "Analog input", \ref pin_feat_30x_pwm_di "Digital input"\n */  
    IO_PIN_D1 = 24,    /**<Pin D1 \n
                *       <i>main function:</i> \ref pin_feat_30x_pwm_cm "PWM Output with current measurement"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_pwm_cm_pwd "Timer Input", \ref pin_feat_30x_pwm_cm_do "High-Side Digital Output (with current measurement)", \ref pin_feat_30x_pwm_ocm_do "High-Side Digital Output", \ref pin_feat_30x_pwm_adc "Analog input", \ref pin_feat_30x_pwm_di "Digital input"\n */  
    IO_PIN_C1 = 25,    /**<Pin C1 \n
                *       <i>main function:</i> \ref pin_feat_30x_pwm_cm "PWM Output with current measurement"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_pwm_cm_pwd "Timer Input", \ref pin_feat_30x_pwm_cm_do "High-Side Digital Output (with current measurement)", \ref pin_feat_30x_pwm_ocm_do "High-Side Digital Output", \ref pin_feat_30x_pwm_adc "Analog input", \ref pin_feat_30x_pwm_di "Digital input"\n */  
    IO_PIN_K1 = 26,    /**<Pin K1 \n
                *       <i>main function:</i> \ref pin_feat_30x_pwm_ocm "PWM Output"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_pwm_ocm_pwd "Complex Timer Input", \ref pin_feat_30x_pwm_ocm_do "High-Side Digital Output", \ref pin_feat_30x_pwm_adc "Analog input", \ref pin_feat_30x_pwm_di "Digital input"\n */  
    IO_PIN_J1 = 27,    /**<Pin J1 \n
                *       <i>main function:</i> \ref pin_feat_30x_pwm_ocm "PWM Output"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_pwm_ocm_pwd "Complex Timer Input", \ref pin_feat_30x_pwm_ocm_do "High-Side Digital Output", \ref pin_feat_30x_pwm_adc "Analog input", \ref pin_feat_30x_pwm_di "Digital input"\n */  
    IO_PIN_B1 = 28,    /**<Pin B1 \n
                *       <i>main function:</i> \ref pin_feat_30x_do_ls "Low-Side Digital Output"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_do_ls_adc "Analog Input", \ref pin_feat_30x_do_ls_di "Digital Input"\n */  
    IO_PIN_A1 = 29,    /**<Pin A1 \n
                *       <i>main function:</i> \ref pin_feat_30x_do_ls "Low-Side Digital Output"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_do_ls_adc "Analog Input", \ref pin_feat_30x_do_ls_di "Digital Input"\n */  
    IO_PIN_K2 = 30,    /**<Pin K2 \n
                *       <i>main function:</i> \ref pin_feat_30x_pvg "PVG Output"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_vout "Voltage Output", \ref pin_feat_30x_pvg_adc "Analog Input", \ref pin_feat_30x_pvg_di "Digital Input", \ref pin_feat_30x_pvg_do_pp "Push-Pull Digital Outputs"\n */  
    IO_PIN_J2 = 31,    /**<Pin J2 \n
                *       <i>main function:</i> \ref pin_feat_30x_pvg "PVG Output"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_vout "Voltage Output", \ref pin_feat_30x_pvg_adc "Analog Input", \ref pin_feat_30x_pvg_di "Digital Input", \ref pin_feat_30x_pvg_do_pp "Push-Pull Digital Outputs"\n */  
    IO_PIN_H2 = 32,    /**<Pin H2 \n
                *       <i>main function:</i> \ref pin_feat_30x_pvg "PVG Output"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_vout "Voltage Output", \ref pin_feat_30x_pvg_adc "Analog Input", \ref pin_feat_30x_pvg_di "Digital Input", \ref pin_feat_30x_pvg_do_pp "Push-Pull Digital Outputs"\n */  
    IO_PIN_G2 = 33,    /**<Pin G2 \n
                *       <i>main function:</i> \ref pin_feat_30x_pvg "PVG Output"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_vout "Voltage Output", \ref pin_feat_30x_pvg_adc "Analog Input", \ref pin_feat_30x_pvg_di "Digital Input", \ref pin_feat_30x_pvg_do_pp "Push-Pull Digital Outputs"\n */  
    IO_PIN_F2 = 34,    /**<Pin F2 \n
                *       <i>main function:</i> \ref pin_feat_30x_pvg "PVG Output"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_vout "Voltage Output", \ref pin_feat_30x_pvg_adc "Analog Input", \ref pin_feat_30x_pvg_di "Digital Input", \ref pin_feat_30x_pvg_do_pp "Push-Pull Digital Outputs"\n */  
    IO_PIN_E2 = 35,    /**<Pin E2 \n
                *       <i>main function:</i> \ref pin_feat_30x_pvg "PVG Output"\n
                *       <i>alternate functions:</i> \ref pin_feat_30x_vout "Voltage Output", \ref pin_feat_30x_pvg_adc "Analog Input", \ref pin_feat_30x_pvg_di "Digital Input", \ref pin_feat_30x_pvg_do_pp "Push-Pull Digital Outputs"\n */  
    IO_INT_PIN_UART_CH0 = 36,    /**<Internal Pin for UART module, see \ref IO_POWER.h "Driver for ECU Power functions" for details\n */  
    IO_INT_PIN_DRIVER = 37,    /**<Internal Pin for general Driver, see \ref IO_Driver.h "General Driver items" for details\n */  
    IO_INT_PIN_RTC = 38,    /**<Internal Pin for RTC, see \ref IO_RTC.h "RTC Driver" for details\n */  
    IO_INT_PIN_PERIODIC = 39,    /**<Internal Pin for Periodic Timer, see \ref IO_RTC.h "RTC Driver" for details\n */  
    IO_INT_PIN_POWER = 40,    /**<Internal Pin for Power Driver, see \ref IO_POWER.h "Driver for ECU Power functions" for details\n */  
    IO_INT_PIN_EEPROM = 41,    /**<Internal Pin for EEPROM Driver, see \ref IO_EEPROM.h "EEPROM Driver" for details\n */  
    IO_INT_DEV_CPU = 42,    /**<General device 'pin' for internal CPU diagnosis */  
    IO_INT_PIN_EXT_WD = 43,    /**<Internal device pin for diagnosis of external watchdog\n */  
    IO_INT_PIN_TEMP = 44,    /**<Internal Pin for temperature measurement, see \ref IO_ADC.h "ADC Driver" for details\n */  
    IO_INT_PIN_POWERSTAGE_ENABLE = 45,    /**<Internal Pin for Powerstage enable signal, see \ref IO_POWER.h "Driver for ECU Power functions" for details\n */  
    IO_INT_PIN_PVG_VOUT_0_ENABLE = 46,    /**<Internal Pin for PVG group 0 Output enable signal, see \ref IO_POWER.h "Driver for ECU Power functions" for details\n */  
    IO_INT_PIN_PVG_VOUT_1_ENABLE = 47,    /**<Internal Pin for PVG group 1 Output enable signal, see \ref IO_POWER.h "Driver for ECU Power functions" for details\n */  
    IO_INT_PIN_CAN_CH0 = 48,    /**<Internal Pin for CAN channel 0, see \ref IO_CAN.h "CAN Driver" for details\n */  
    IO_INT_PIN_CAN_CH1 = 49,    /**<Internal Pin for CAN channel 1, see \ref IO_CAN.h "CAN Driver" for details\n */  
    IO_INT_PIN_5V2 = 50,    /**<Internal Pin for measurement of +5V2 voltage, see \ref IO_ADC.h "ADC Driver" for details\n */  
    IO_INT_PIN_SHIFT_LB_HI = 51,    /**<Internal Pin for measurement shift register 0 loopback voltage (high nibble) */  
    IO_INT_PIN_SHIFT_LB_LO = 52,    /**<Internal Pin for measurement shift register 0 loopback voltage (low nibble) */  
    IO_INT_PIN_SHIFT1_LB_HI = 53,    /**<Internal Pin for measurement shift register 1 loopback voltage (high nibble) */  
    IO_INT_PIN_SHIFT1_LB_LO = 54,    /**<Internal Pin for measurement shift register 1 loopback voltage (low nibble) */  
    IO_PIN_NONE = 55,
    IO_PIN_MAX = 56
} IO_PIN;

#define IO_ADC_UBAT    IO_PIN_L2    /**<Pin L2, for details and features see #IO_PIN_L2 */
#define IO_ADC_UBAT_CPU    IO_PIN_L1    /**<Pin L1, for details and features see #IO_PIN_L1 */
#define IO_K15    IO_PIN_K4    /**<Pin K4, for details and features see #IO_PIN_K4 */
#define IO_ADC_SENSOR_SUPPLY    IO_PIN_H3    /**<Pin H3, for details and features see #IO_PIN_H3 */
#define IO_ADC_NODE_ID_0    IO_PIN_K3    /**<Pin K3, for details and features see #IO_PIN_K3 */
#define IO_ADC_NODE_ID_1    IO_PIN_J3    /**<Pin J3, for details and features see #IO_PIN_J3 */
#define IO_ADC_20    IO_PIN_G4    /**<Pin G4, for details and features see #IO_PIN_G4 */
#define IO_DI_02    IO_PIN_G4    /**<Pin G4, for details and features see #IO_PIN_G4 */
#define IO_ADC_21    IO_PIN_F4    /**<Pin F4, for details and features see #IO_PIN_F4 */
#define IO_DI_03    IO_PIN_F4    /**<Pin F4, for details and features see #IO_PIN_F4 */
#define IO_PWD_00    IO_PIN_E3    /**<Pin E3, for details and features see #IO_PIN_E3 */
#define IO_ADC_30    IO_PIN_E3    /**<Pin E3, for details and features see #IO_PIN_E3 */
#define IO_DI_04    IO_PIN_E3    /**<Pin E3, for details and features see #IO_PIN_E3 */
#define IO_PWD_01    IO_PIN_D3    /**<Pin D3, for details and features see #IO_PIN_D3 */
#define IO_ADC_31    IO_PIN_D3    /**<Pin D3, for details and features see #IO_PIN_D3 */
#define IO_DI_05    IO_PIN_D3    /**<Pin D3, for details and features see #IO_PIN_D3 */
#define IO_PWD_02    IO_PIN_C3    /**<Pin C3, for details and features see #IO_PIN_C3 */
#define IO_ADC_32    IO_PIN_C3    /**<Pin C3, for details and features see #IO_PIN_C3 */
#define IO_DI_06    IO_PIN_C3    /**<Pin C3, for details and features see #IO_PIN_C3 */
#define IO_PWD_03    IO_PIN_B3    /**<Pin B3, for details and features see #IO_PIN_B3 */
#define IO_ADC_33    IO_PIN_B3    /**<Pin B3, for details and features see #IO_PIN_B3 */
#define IO_DI_07    IO_PIN_B3    /**<Pin B3, for details and features see #IO_PIN_B3 */
#define IO_ADC_00    IO_PIN_J4    /**<Pin J4, for details and features see #IO_PIN_J4 */
#define IO_LED_00    IO_PIN_J4    /**<Pin J4, for details and features see #IO_PIN_J4 */
#define IO_DI_00    IO_PIN_J4    /**<Pin J4, for details and features see #IO_PIN_J4 */
#define IO_ADC_01    IO_PIN_H4    /**<Pin H4, for details and features see #IO_PIN_H4 */
#define IO_LED_01    IO_PIN_H4    /**<Pin H4, for details and features see #IO_PIN_H4 */
#define IO_DI_01    IO_PIN_H4    /**<Pin H4, for details and features see #IO_PIN_H4 */
#define IO_ADC_10    IO_PIN_E4    /**<Pin E4, for details and features see #IO_PIN_E4 */
#define IO_LED_02    IO_PIN_E4    /**<Pin E4, for details and features see #IO_PIN_E4 */
#define IO_DI_10    IO_PIN_E4    /**<Pin E4, for details and features see #IO_PIN_E4 */
#define IO_ADC_11    IO_PIN_D4    /**<Pin D4, for details and features see #IO_PIN_D4 */
#define IO_LED_03    IO_PIN_D4    /**<Pin D4, for details and features see #IO_PIN_D4 */
#define IO_DI_11    IO_PIN_D4    /**<Pin D4, for details and features see #IO_PIN_D4 */
#define IO_ADC_12    IO_PIN_C4    /**<Pin C4, for details and features see #IO_PIN_C4 */
#define IO_LED_04    IO_PIN_C4    /**<Pin C4, for details and features see #IO_PIN_C4 */
#define IO_DI_12    IO_PIN_C4    /**<Pin C4, for details and features see #IO_PIN_C4 */
#define IO_ADC_13    IO_PIN_B4    /**<Pin B4, for details and features see #IO_PIN_B4 */
#define IO_LED_05    IO_PIN_B4    /**<Pin B4, for details and features see #IO_PIN_B4 */
#define IO_DI_13    IO_PIN_B4    /**<Pin B4, for details and features see #IO_PIN_B4 */
#define IO_ADC_14    IO_PIN_A4    /**<Pin A4, for details and features see #IO_PIN_A4 */
#define IO_LED_06    IO_PIN_A4    /**<Pin A4, for details and features see #IO_PIN_A4 */
#define IO_DI_14    IO_PIN_A4    /**<Pin A4, for details and features see #IO_PIN_A4 */
#define IO_ADC_15    IO_PIN_A3    /**<Pin A3, for details and features see #IO_PIN_A3 */
#define IO_LED_07    IO_PIN_A3    /**<Pin A3, for details and features see #IO_PIN_A3 */
#define IO_DI_15    IO_PIN_A3    /**<Pin A3, for details and features see #IO_PIN_A3 */
#define IO_PWM_00    IO_PIN_H1    /**<Pin H1, for details and features see #IO_PIN_H1 */
#define IO_DO_00    IO_PIN_H1    /**<Pin H1, for details and features see #IO_PIN_H1 */
#define IO_DO_20    IO_PIN_H1    /**<Pin H1, for details and features see #IO_PIN_H1 */
#define IO_ADC_34    IO_PIN_H1    /**<Pin H1, for details and features see #IO_PIN_H1 */
#define IO_DI_24    IO_PIN_H1    /**<Pin H1, for details and features see #IO_PIN_H1 */
#define IO_PWD_20    IO_PIN_H1    /**<Pin H1, for details and features see #IO_PIN_H1 */
#define IO_PWM_01    IO_PIN_G1    /**<Pin G1, for details and features see #IO_PIN_G1 */
#define IO_DO_01    IO_PIN_G1    /**<Pin G1, for details and features see #IO_PIN_G1 */
#define IO_DO_21    IO_PIN_G1    /**<Pin G1, for details and features see #IO_PIN_G1 */
#define IO_ADC_35    IO_PIN_G1    /**<Pin G1, for details and features see #IO_PIN_G1 */
#define IO_DI_25    IO_PIN_G1    /**<Pin G1, for details and features see #IO_PIN_G1 */
#define IO_PWD_21    IO_PIN_G1    /**<Pin G1, for details and features see #IO_PIN_G1 */
#define IO_PWM_02    IO_PIN_F1    /**<Pin F1, for details and features see #IO_PIN_F1 */
#define IO_DO_02    IO_PIN_F1    /**<Pin F1, for details and features see #IO_PIN_F1 */
#define IO_DO_22    IO_PIN_F1    /**<Pin F1, for details and features see #IO_PIN_F1 */
#define IO_ADC_36    IO_PIN_F1    /**<Pin F1, for details and features see #IO_PIN_F1 */
#define IO_DI_26    IO_PIN_F1    /**<Pin F1, for details and features see #IO_PIN_F1 */
#define IO_PWD_10    IO_PIN_F1    /**<Pin F1, for details and features see #IO_PIN_F1 */
#define IO_PWM_03    IO_PIN_E1    /**<Pin E1, for details and features see #IO_PIN_E1 */
#define IO_DO_03    IO_PIN_E1    /**<Pin E1, for details and features see #IO_PIN_E1 */
#define IO_DO_23    IO_PIN_E1    /**<Pin E1, for details and features see #IO_PIN_E1 */
#define IO_ADC_37    IO_PIN_E1    /**<Pin E1, for details and features see #IO_PIN_E1 */
#define IO_DI_27    IO_PIN_E1    /**<Pin E1, for details and features see #IO_PIN_E1 */
#define IO_PWD_11    IO_PIN_E1    /**<Pin E1, for details and features see #IO_PIN_E1 */
#define IO_PWM_04    IO_PIN_D1    /**<Pin D1, for details and features see #IO_PIN_D1 */
#define IO_DO_04    IO_PIN_D1    /**<Pin D1, for details and features see #IO_PIN_D1 */
#define IO_DO_24    IO_PIN_D1    /**<Pin D1, for details and features see #IO_PIN_D1 */
#define IO_ADC_38    IO_PIN_D1    /**<Pin D1, for details and features see #IO_PIN_D1 */
#define IO_DI_28    IO_PIN_D1    /**<Pin D1, for details and features see #IO_PIN_D1 */
#define IO_PWD_12    IO_PIN_D1    /**<Pin D1, for details and features see #IO_PIN_D1 */
#define IO_PWM_05    IO_PIN_C1    /**<Pin C1, for details and features see #IO_PIN_C1 */
#define IO_DO_05    IO_PIN_C1    /**<Pin C1, for details and features see #IO_PIN_C1 */
#define IO_DO_25    IO_PIN_C1    /**<Pin C1, for details and features see #IO_PIN_C1 */
#define IO_ADC_39    IO_PIN_C1    /**<Pin C1, for details and features see #IO_PIN_C1 */
#define IO_DI_29    IO_PIN_C1    /**<Pin C1, for details and features see #IO_PIN_C1 */
#define IO_PWD_13    IO_PIN_C1    /**<Pin C1, for details and features see #IO_PIN_C1 */
#define IO_PWM_10    IO_PIN_K1    /**<Pin K1, for details and features see #IO_PIN_K1 */
#define IO_DO_06    IO_PIN_K1    /**<Pin K1, for details and features see #IO_PIN_K1 */
#define IO_ADC_40    IO_PIN_K1    /**<Pin K1, for details and features see #IO_PIN_K1 */
#define IO_DI_30    IO_PIN_K1    /**<Pin K1, for details and features see #IO_PIN_K1 */
#define IO_PWD_22    IO_PIN_K1    /**<Pin K1, for details and features see #IO_PIN_K1 */
#define IO_PWM_11    IO_PIN_J1    /**<Pin J1, for details and features see #IO_PIN_J1 */
#define IO_DO_07    IO_PIN_J1    /**<Pin J1, for details and features see #IO_PIN_J1 */
#define IO_ADC_41    IO_PIN_J1    /**<Pin J1, for details and features see #IO_PIN_J1 */
#define IO_DI_31    IO_PIN_J1    /**<Pin J1, for details and features see #IO_PIN_J1 */
#define IO_PWD_23    IO_PIN_J1    /**<Pin J1, for details and features see #IO_PIN_J1 */
#define IO_SAFETY_SWITCH_0    IO_PIN_B1    /**<Pin B1, for details and features see #IO_PIN_B1 */
#define IO_DO_10    IO_PIN_B1    /**<Pin B1, for details and features see #IO_PIN_B1 */
#define IO_ADC_28    IO_PIN_B1    /**<Pin B1, for details and features see #IO_PIN_B1 */
#define IO_DI_22    IO_PIN_B1    /**<Pin B1, for details and features see #IO_PIN_B1 */
#define IO_SAFETY_SWITCH_1    IO_PIN_A1    /**<Pin A1, for details and features see #IO_PIN_A1 */
#define IO_DO_11    IO_PIN_A1    /**<Pin A1, for details and features see #IO_PIN_A1 */
#define IO_ADC_29    IO_PIN_A1    /**<Pin A1, for details and features see #IO_PIN_A1 */
#define IO_DI_23    IO_PIN_A1    /**<Pin A1, for details and features see #IO_PIN_A1 */
#define IO_PVG_00    IO_PIN_K2    /**<Pin K2, for details and features see #IO_PIN_K2 */
#define IO_VOUT_00    IO_PIN_K2    /**<Pin K2, for details and features see #IO_PIN_K2 */
#define IO_ADC_22    IO_PIN_K2    /**<Pin K2, for details and features see #IO_PIN_K2 */
#define IO_DI_16    IO_PIN_K2    /**<Pin K2, for details and features see #IO_PIN_K2 */
#define IO_DO_30    IO_PIN_K2    /**<Pin K2, for details and features see #IO_PIN_K2 */
#define IO_PVG_01    IO_PIN_J2    /**<Pin J2, for details and features see #IO_PIN_J2 */
#define IO_VOUT_01    IO_PIN_J2    /**<Pin J2, for details and features see #IO_PIN_J2 */
#define IO_ADC_23    IO_PIN_J2    /**<Pin J2, for details and features see #IO_PIN_J2 */
#define IO_DI_17    IO_PIN_J2    /**<Pin J2, for details and features see #IO_PIN_J2 */
#define IO_DO_31    IO_PIN_J2    /**<Pin J2, for details and features see #IO_PIN_J2 */
#define IO_PVG_02    IO_PIN_H2    /**<Pin H2, for details and features see #IO_PIN_H2 */
#define IO_VOUT_02    IO_PIN_H2    /**<Pin H2, for details and features see #IO_PIN_H2 */
#define IO_ADC_24    IO_PIN_H2    /**<Pin H2, for details and features see #IO_PIN_H2 */
#define IO_DI_18    IO_PIN_H2    /**<Pin H2, for details and features see #IO_PIN_H2 */
#define IO_DO_32    IO_PIN_H2    /**<Pin H2, for details and features see #IO_PIN_H2 */
#define IO_PVG_03    IO_PIN_G2    /**<Pin G2, for details and features see #IO_PIN_G2 */
#define IO_VOUT_03    IO_PIN_G2    /**<Pin G2, for details and features see #IO_PIN_G2 */
#define IO_ADC_25    IO_PIN_G2    /**<Pin G2, for details and features see #IO_PIN_G2 */
#define IO_DI_19    IO_PIN_G2    /**<Pin G2, for details and features see #IO_PIN_G2 */
#define IO_DO_33    IO_PIN_G2    /**<Pin G2, for details and features see #IO_PIN_G2 */
#define IO_PVG_04    IO_PIN_F2    /**<Pin F2, for details and features see #IO_PIN_F2 */
#define IO_VOUT_04    IO_PIN_F2    /**<Pin F2, for details and features see #IO_PIN_F2 */
#define IO_ADC_26    IO_PIN_F2    /**<Pin F2, for details and features see #IO_PIN_F2 */
#define IO_DI_20    IO_PIN_F2    /**<Pin F2, for details and features see #IO_PIN_F2 */
#define IO_DO_34    IO_PIN_F2    /**<Pin F2, for details and features see #IO_PIN_F2 */
#define IO_PVG_05    IO_PIN_E2    /**<Pin E2, for details and features see #IO_PIN_E2 */
#define IO_VOUT_05    IO_PIN_E2    /**<Pin E2, for details and features see #IO_PIN_E2 */
#define IO_ADC_27    IO_PIN_E2    /**<Pin E2, for details and features see #IO_PIN_E2 */
#define IO_DI_21    IO_PIN_E2    /**<Pin E2, for details and features see #IO_PIN_E2 */
#define IO_DO_35    IO_PIN_E2    /**<Pin E2, for details and features see #IO_PIN_E2 */
#define IO_UART    IO_INT_PIN_UART_CH0    /**<Internal Pin for UART channel on the Debug Adapter */
#define IO_ADC_BOARD_TEMP    IO_INT_PIN_TEMP    /**<internal pin for measuring the board temperature */
#define IO_INT_POWERSTAGE_ENABLE    IO_INT_PIN_POWERSTAGE_ENABLE    /**<Internal Pin for enabling powerstages */
#define IO_INT_PVG_VOUT_0_ENABLE    IO_INT_PIN_PVG_VOUT_0_ENABLE    /**<Internal Pin for enabling PVG group 0 outputs */
#define IO_INT_PVG_VOUT_1_ENABLE    IO_INT_PIN_PVG_VOUT_1_ENABLE    /**<Internal Pin for enabling PVG group 1 outputs */
#define IO_CAN_CHANNEL_0    IO_INT_PIN_CAN_CH0    /**<Internal Pin for CAN channel 0 */
#define IO_CAN_CHANNEL_1    IO_INT_PIN_CAN_CH1    /**<Internal Pin for CAN channel 1 */
#define IO_ADC_5V2    IO_INT_PIN_5V2    /**<internal pin for measuring the 5.2V Voltage */

#endif
    
/**********************************/
/*       GENERATED CODE END       */