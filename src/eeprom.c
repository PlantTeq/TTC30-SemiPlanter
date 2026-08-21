/*
 * eeprom.c
 *
 *  Created on: Jan 13, 2022
 *      Author: Matthijs
 */

#include "eeprom.h"
#include "IO_Driver.h"
#include "IO_EEPROM.h"
#include "IO_EEPROM_Preload.h"

#include "uart.h"
#include "sysconf.h"

volatile IO_ErrorType io_error_eeprom = IO_E_OK;

/****************************************************************************/
void EepromInit (void)
/****************************************************************************/
{
	IO_EEPROM_DeInit();
    /* initialize EEPROM driver */
    io_error_eeprom = IO_EEPROM_Init();
    UART_Printf (IO_UART, "Eeprom init: %u \r\n", io_error_eeprom);
}

/****************************************************************************/
ubyte1 EepromWrite (ubyte2 dataAddress, ubyte2 length, bool CRC, ubyte1 *bufferPtr)
/****************************************************************************/
{
	// Wait for EEPROM to be ready
	while (IO_EEPROM_GetStatus() != IO_E_OK) IO_EEPROM_PreloadTask();

	io_error_eeprom = IO_EEPROM_GetStatus();
	if (io_error_eeprom != IO_E_OK) UART_Printf (IO_UART, "Eeprom start Writestatus: %u \r\n", io_error_eeprom);
    /* write to memory */
        if (IO_EEPROM_GetStatus() == IO_E_OK)
        {
            io_error_eeprom = IO_EEPROM_Write(dataAddress,      /* address 0x0000 */
            		 length,  /* size of the data */
            		 CRC,
                     bufferPtr);  /* local data buffer */
        }

    	// Wait for EEPROM to be ready
    	while (IO_EEPROM_GetStatus() != IO_E_OK) IO_EEPROM_PreloadTask();

        io_error_eeprom = IO_EEPROM_GetStatus();
        if (io_error_eeprom != IO_E_OK) UART_Printf (IO_UART, "EepromWritestatus: %u \r\n", io_error_eeprom);
        return (ubyte1) io_error_eeprom;
}

/****************************************************************************/
ubyte1 EepromRead (ubyte2 dataAddress, ubyte2 length, bool CRC, ubyte1 *bufferPtr)
/****************************************************************************/
{
	io_error_eeprom = IO_EEPROM_GetStatus();
	if (io_error_eeprom != IO_E_OK) UART_Printf (IO_UART, "Eeprom start readStatus %u \r\n", io_error_eeprom);

	// Wait for EEPROM to be ready
	while (IO_EEPROM_GetStatus() != IO_E_OK) IO_EEPROM_PreloadTask();

    /* read back memory */
    io_error_eeprom = IO_EEPROM_Read(dataAddress,       /* address 0x0000 */
    						length,   /* size of the data */
    						CRC,
                            bufferPtr);   /* local data buffer */


	// Wait for EEPROM to be ready
	while (IO_EEPROM_GetStatus() != IO_E_OK) IO_EEPROM_PreloadTask();

    /* wait for the EEPROM handler to become ready again */
    while (IO_EEPROM_GetStatus() != IO_E_OK);


    return (ubyte1) io_error_eeprom;
}


void Save(void)
{
	io_error_eeprom = IO_EEPROM_GetStatus();
	if (io_error_eeprom != IO_E_OK) UART_Printf (IO_UART, "Eeprom start Writestatus: %u \r\n", io_error_eeprom);

	EepromWrite (0, sizeof(cfg), FALSE, (ubyte1 *) &cfg);

    io_error_eeprom = IO_EEPROM_GetStatus();
    if (io_error_eeprom != IO_E_OK) UART_Printf (IO_UART, "Eeprom Writestatus: %u \r\n", io_error_eeprom);

    EepromRead (0, sizeof (cfg), FALSE, (ubyte1 *) &cfg);

    io_error_eeprom = IO_EEPROM_GetStatus();
    if (io_error_eeprom != IO_E_OK) UART_Printf (IO_UART, "Eeprom readstatus: %u \r\n", io_error_eeprom);

    /* wait for the EEPROM handler to become ready again */
    while (IO_EEPROM_GetStatus() != IO_E_OK);

}


