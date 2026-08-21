/*
                             *******************
******************************* C SOURCE FILE *******************************
**                           *******************                           **
**                                                                         **
*****************************************************************************/


/****************************************************************************/
/**                                                                        **/
/**                     MODULES USED                                       **/
/**                                                                        **/
/****************************************************************************/
#include "IO_UART.h"
#include "IO_Driver.h"
#include "UART.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "Apdb.h"
#include "ApdbCfg.h"


/****************************************************************************/
/**                                                                        **/
/**                     GLOBAL VARIABLES                                   **/
/**                                                                        **/
/****************************************************************************/

IO_ErrorType io_error_uart;

/****************************************************************************/
/**                                                                        **/
/**                     EXPORTED FUNCTIONS                                 **/
/**                                                                        **/
/****************************************************************************/
void UART_init(ubyte4 Baudrate)
{
	   // initialize the UART with a baudrate of 115200 baud/s
	   (void) IO_UART_Init (IO_UART, Baudrate);
}

void UART_Printf( IO_PIN channel, const char * format, ...)
{
    ubyte1 buffer[128] = {0};
    ubyte1 size;

    va_list arg;

    // parse arguments
    va_start (arg, format);
    vsprintf ((char*) buffer, format, arg);
    va_end (arg);

    IO_UART_Write (
          IO_UART
        , (ubyte1*) buffer
        , strlen ((char*) buffer)
        , &size);

    do
    {
        (void) IO_UART_GetTxStatus(channel, &size);
        (void) IO_UART_Task();
    } while (size != 0);
}



/****************************************************************************/
/**                                                                        **/
/**                               EOF                                      **/
/**                                                                        **/
/****************************************************************************/
