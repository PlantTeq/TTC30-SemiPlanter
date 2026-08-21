/**************************************************************************
 *  HY-TTC 30
 *  ZAP control software
 *  PlantTeq
 *
 *  Rijkevoort-de Walsert
 *  The Netherlands
 *
 **************************************************************************/



#include "IO_Driver.h"
#include "IO_UART.h"
#include "IO_ADC.h"
#include "IO_DIO.h"
#include "IO_RTC.h"
#include "IO_Power.h"
#include "IO_PWM.h"
#include "Apdb.h"
#include "ApdbCfg.h"
#include "CanHandler.h"


#include "UART.h"
#include "sysconf.h"
#include "Canbus.h"
#include "Control.h"
#include "Eeprom.h"



#include <stdarg.h>
#include <stdio.h>
/*
 * use these fields to define the software version in three levels
 * major.minor.patch. this version is accessible from the APDB and
 * can therefore be read using the ttc-downloader
 */
#define SW_VERSION_MAJOR 1 //  8-bit
#define SW_VERSION_MINOR 0 //  8-bit
#define SW_VERSION_PATCH 0 // 16-bit

/*
 * use these definitions (or replace content in appl_db) to define the
 * node-nr and baud-rate used by the device
 */
#define NODE_NR         1
#define CAN_BAUDRATE  250 // kbps



Planter_t planter;
CFG_t cfg;

/*
 * use these definitions to be able to identify the application software
 * using the TTC-Downloader (vendor and application ID)
 */
#define APDB_MANUF_ID  ((ubyte1) 0x00)
#define APDB_APP_ID    ((ubyte1) 0x00)

/* 
 * application database
 * needed by TTC-Downloader
 */
LOCATE_APDB appl_db = { 
      APDB_VERSION          // ubyte4 versionAPDB
    , {0}                   // BL_T_DATE flashDate
                            // BL_T_DATE buildDate
    , { (ubyte4) ((((  (ubyte4) RTS_TTC_FLASH_DATE_YEAR)   & 0x0FFF) <<  0)  |
                    ((((ubyte4) RTS_TTC_FLASH_DATE_MONTH)  & 0x000F) << 12)  |
                    ((((ubyte4) RTS_TTC_FLASH_DATE_DAY)    & 0x001F) << 16)  |
                    ((((ubyte4) RTS_TTC_FLASH_DATE_HOUR)   & 0x001F) << 21)  |
                    ((((ubyte4) RTS_TTC_FLASH_DATE_MINUTE) & 0x003F) << 26)) }
    , 0                      // ubyte4 nodeType
    , 0                      // ubyte4 startAddress
    , 0                      // ubyte4 codeSize
    , 0                      // ubyte4 legacyAppCRC
    , 0                      // ubyte4 appCRC
    , NODE_NR                // ubyte1 nodeNr
    , 0                      // ubyte4 CRCInit
    , 0                      // ubyte4 flags
    , 0                      // ubyte4 hook1
    , 0                      // ubyte4 hook2
    , 0                      // ubyte4 hook3
    , APPL_START             // ubyte4 mainAddress
    , {0, 1}                 // BL_T_CAN_ID canDownloadID
    , {0, 2}                 // BL_T_CAN_ID canUploadID
    , 0                      // ubyte4 legacyHeaderCRC
                             // ubyte4 version; (8bit.8bit.16bit)
    , { (ubyte4) (((ubyte4) ((ubyte1) (SW_VERSION_MAJOR & 0x00FF)) << 24) |
                  ((ubyte4) ((ubyte1) (SW_VERSION_MINOR & 0x00FF)) << 16) |
                  ((ubyte4) ((ubyte2) (SW_VERSION_PATCH & 0xFFFF))))  }
    , CAN_BAUDRATE           // ubyte2 canBaudrate
    , 0                      // ubyte1 canChannel
    , 0                      // ubyte4 password
    , 0                      // ubyte4 magicSeed
    , {  0,   0,   0,   0}   // ubyte1 targetIPaddress[4]
    , {  0,   0,   0,   0}   // ubyte1 subnetmask[4]
    , {  0,   0,   0,   0}   // ubyte1 dlMulticastIP[4]
    , 0                      // ubyte4 debugKey;
    , 3                      // timeout value for ABRD in calm mode. [seconds]
    , APDB_MANUF_ID          // ubyte1 manufacturerID
    , APDB_APP_ID            // ubyte1 application ID
    , { 0 }                  // reserved for future use (should be 0)
    , 0                      // ubyte4 headerCRC
};

static IO_ErrorType driver_init_rc;
static IO_ErrorType driver_task_begin_rc;
static IO_ErrorType driver_task_end_rc;

ubyte4 timestamp = 0;
ubyte4 loopTime1s = 0;
ubyte4 loopTime100ms = 0;
ubyte4 loopTime10ms =0;


void main (void)
{
    ubyte4 timestamp;

    IO_DRIVER_SAFETY_CONF driver_safety_cfg = {
              IO_DRIVER_SAFETY_SWITCH_NONE           // internal safety switch configuration
            , 40                                     // glitch filter time in [ms]
            , 15000                                  // maximum application cycle time in [us]
            , NULL                                   // no callback function implemented
        };

    // driver initialization, no safety configuration
    driver_init_rc = IO_Driver_Init( IO_DRIVER_MODE_DEFAULT, NULL );

    // initialize the UART with a baudrate of 115200 baud/s
    UART_init(115200);


    EepromInit();
    CanbusInit();
    ControlInit();

    // Activate the power stages for the PVG/Voltage Outputs
    IO_POWER_Set (IO_INT_PIN_PVG_VOUT_0_ENABLE, IO_POWER_ON);
    // for the sake of completeness initialize the second group too
    IO_POWER_Set (IO_INT_PIN_PVG_VOUT_1_ENABLE, IO_POWER_ON);
    // no safety switch is used power stage for PWMs has to be enabled manually
    IO_POWER_Set( IO_INT_POWERSTAGE_ENABLE, IO_POWER_ON );


    // print an initial message via UART
    UART_Printf (IO_UART, "\n\rStartup!\n\r");

    // read eeprom
    EepromRead (0, sizeof (cfg), FALSE, (ubyte1 *) &cfg);
    //cfg.checkWord = 0;
    if(cfg.checkWord != 99 )
    {
    	UART_Printf (IO_UART, "Restore configuration \n\r");

        //Set default parameters
        cfg.plantDistance =     0.33;
        cfg.wheelPerimeter =    3.0f;
        cfg.pulsesPerMeter = 	100; //1640 * 0.9
        cfg.targetAmpStep =     1.0f;
        cfg.checkWord = 		99;

        Save();
    }
    else
    {
    	UART_Printf (IO_UART, "cfg Checkword: %d \r\n", cfg.checkWord);
    

    }


    SendConfig1();



    while (1)
    {
        /* get a timestamp to implement the cycle time */
        (void) IO_RTC_StartTime (&timestamp);
        driver_task_begin_rc = IO_Driver_TaskBegin ();

        CanTick();

        // 10 ms loop time
		if (IO_RTC_GetTimeUS(loopTime10ms)>= 10000)
		{
			loopTime10ms += 10000;
			ControlUpdate();


#ifdef DEBUGMAIN
                    UART_Printf(IO_UART, "G2:%.2f  G1:%.2f  TargetAmp:%.2f  TargetAmp2:%.2f  MVar:%.2f\r\n",
					planter.gripperDeg2,
					planter.gripperDeg,
                    planter.targetAmp,
                    planter.targetAmp2,
                    planter.tempmeasurevar);
#endif
		}

		// 100ms run loop
		if (IO_RTC_GetTimeUS(loopTime100ms)>= 100000)
		{
			loopTime100ms += 100000;





		}

		// 1s run loop
		if (IO_RTC_GetTimeUS(loopTime1s)>= 1000000)
		{
			loopTime1s += 1000000;

			SendCanInfo();



			if(!planter.estopValue) UART_Printf(IO_UART, "estop active!!\n\r");
		}
        driver_task_end_rc = IO_Driver_TaskEnd ();
    }
}


