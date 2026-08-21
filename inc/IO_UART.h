/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_UART.h
 *
 * \brief IO Driver functions for UART communication.
 *
 * The Universal Serial  Interface Channel module (USIC) is a
 * flexible interface module covering several serial
 * communication protocols.
 *
 * A USIC module contains two independent communication channels
 * named UxC0 and UxC1, with x being the number of the USIC module.
 *
 * The UART communication driver uses the USIC module as
 * universal asynchronous receiver transmitter.
 *
 * When initializing the UART communication driver it is possible to
 * define the baudrate individually. Apart from the baudrate, the
 * following parameters are fixed:
 *  - databits: 8
 *  - stopbits: 1
 *  - parity:   none
 *
 * \section uart_buffers The UART buffers
 *
 * The XC2000 has 16 byte transmit and receive buffers for
 * every UART channel.
 * The UART driver additionally implements a SW buffer of 128 bytes.
 *
 * This means that a maximum of 128 bytes can be written, using the
 * \c IO_UART_Write() function. The UART Task function sequentially copies
 * the data to the HW buffer and sends it. The function \c IO_UART_GetTxStatus()
 * returns the number of remaining bytes in the SW buffer. The write function
 * can be called any time, as long as the SW buffer is not full.
 *
 * The UART Task function is called automatically every SW cycle by \c IO_Driver_TaskEnd(). \n
 * To speed up the transmission the application can call the \c IO_UART_Task() function
 * at any time.
 *
 * In every software cycle a maximum of 16 bytes (size of HW buffer)
 * can be received via a UART interface. \n The data is copied to the
 * SW buffer by the UART task function. To increase the number of bytes
 * which can be received within a cycle the application can call the
 * function \c IO_UART_Task() during the cycle. \n
 * The function \c IO_UART_GetRxStatus()
 * returns the number of bytes in the SW buffer which is available
 * for reading.
 *
 * \section uart_examples UART code examples
 *
 * Please refer to section \ref appl_structure for understanding where to place the initialization and task function calls.
 *
 * UART initialization example:
 * \code
 * // setup UART channel:
 * IO_UART_Init( IO_UART
 *             , 115200 );              // 115200 baud
 * \endcode
 *
 * UART read example:
 * \code
 * ubyte1 data[40] = {0};
 * ubyte1 size;
 *
 * // check if new bytes have been received
 * IO_UART_GetRxStatus(IO_UART, &size);
 * if (size > 0)
 * {
 *     // read a maximum of 40 bytes
 *     IO_UART_Read(IO_UART, data, 40, &size);
 *
 *     // data now holds the received data
 *     // size holds the number of actually read bytes.
 * }
 * \endcode
 *
 * UART write example:
 * \code
 * ubyte1 data[5] = {0, 1, 2, 3, 4};
 * ubyte1 size;
 *
 * // write data to UART buffer:
 * IO_UART_Write(IO_UART, data, 5, &size);
 * // size holds the number of actually written bytes.
 *
 * // check if data has been transmitted
 * IO_UART_GetTxStatus(IO_UART, &size);
 * // when size returns 0 all the data has been transmitted
 * \endcode
 *
 **************************************************************************/
#ifndef _IO_UART_H
#define _IO_UART_H

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
 * \brief Initialization of the UART communication driver.
 *
 * Initialization of UART Serial Communication Driver
 *    - Enables module
 *    - Configures module for ASC
 *    - Initializes SW queue
 *    - Sets the communication mode to 8N1
 *
 * \param   channel         UART Channel: \c #IO_UART
 * \param   baudrate        Baud rate in baud/s (1200 ... 115200)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID  channel number is invalid
 * \retval #IO_E_INVALID_PARAMETER   a given parameter is out of range
 * \retval #IO_E_CHANNEL_BUSY        the channel is already initialized
 * \retval #IO_E_CH_CAPABILITY       channel is not capable of the desired functionality
 *
 **************************************************************************
 *
 * \remarks
 *   - Module is initialized only once.
 *   To re-initialize the module, the function \c IO_UART_DeInit()
 *   needs to be called.
 *
 * \remarks
 *   - The UART channel is not available on the external connector
 *   but only on the debugging interface.
 *
 **************************************************************************/
 IO_ErrorType IO_UART_Init( IO_PIN channel
                          , ubyte4 baudrate );

/**********************************************************************//**
 *
 * \brief Deinitialization of the UART channel
 *
 * Allows re-initialization by \c IO_UART_Init()
 *
 * \param   channel         UART Channel: \c #IO_UART
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     channel number is invalid
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED channel has not been initialized
 * \retval #IO_E_CH_CAPABILITY          channel should not be capable of the desired functionality
 *
 **************************************************************************/
 IO_ErrorType IO_UART_DeInit(IO_PIN channel);

/**********************************************************************//**
 *
 * \brief Task function for UART communication
 *
 * The task function copies the data to be sent from the SW buffer to the
 * HW buffer and the received data from the HW buffer to the SW buffer.
 *
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_UART_BUFFER_FULL    SW receive queue is full and data
 *                                   has been lost
 * \retval #IO_E_UART_OVERFLOW       HW receive buffer overrun
 * \retval #IO_E_UART_PARITY         parity check failed
 *
 *
 **************************************************************************/
 IO_ErrorType IO_UART_Task(void);

/**********************************************************************//**
 *
 * \brief Read data from serial interface
 *
 * Reads the data from the SW buffer.
 * If new data is available the content will transferred from the
 * SW buffer to the data array. In case there is no new data available
 * the array is untouched.
 *
 * \param   channel         UART Channel: \c #IO_UART
 * \param   data            Data array
 * \param   len             Maximum size of data array
 * \param   rx_len          Actually read bytes
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_UART_BUFFER_FULL    SW receive queue is full and data
 *                                   has been lost
 * \retval #IO_E_UART_OVERFLOW       HW receive buffer overrun
 * \retval #IO_E_UART_PARITY         parity check failed
 * \retval #IO_E_INVALID_CHANNEL_ID  invalid channel ID has been passed
 * \retval #IO_E_NULL_POINTER        null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED channel has not been initialized
 * \retval #IO_E_CH_CAPABILITY       channel is not capable of the desired functionality
 *
 **************************************************************************/
 IO_ErrorType IO_UART_Read( IO_PIN channel
                          , ubyte1 * const data
                          , ubyte1 len
                          , ubyte1 * const rx_len);

/**********************************************************************//**
 *
 * \brief Write data to serial interface
 *
 * Writes the data to the SW buffer and starts the transmission.
 *
 * \param   channel         UART Channel: \c #IO_UART
 * \param   data            Data array
 * \param   len             Number of bytes in data array
 * \param   tx_len          Actually written bytes
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_UART_BUFFER_FULL    SW transmit queue is full, no data
 *                                    has been written
 * \retval #IO_E_INVALID_CHANNEL_ID  invalid channel ID has been passed
 * \retval #IO_E_NULL_POINTER        null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED channel has not been initialized
 * \retval #IO_E_CH_CAPABILITY       channel is not capable of the desired functionality
 *
 * \note
 *   It is highly recommended not to modify the content of the data array
 *   while a write operation is ongoing! Furthermore please note that the
 *   data array must not be a local variable since the driver does not create
 *   a copy of the content but saves the address of the pointer.
 *
 **************************************************************************/
 IO_ErrorType IO_UART_Write( IO_PIN channel
                           , const ubyte1 * const data
                           , ubyte1 len
                           , ubyte1 * const tx_len);

/**********************************************************************//**
 *
 * \brief Retrieve the status of the receive buffer
 *
 * Returns the current status of the receive buffer: new data available, error has occured...
 *
 * \param   channel         UART Channel: \c #IO_UART
 * \param   rx_len          Number of received data frames in receive buffer
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_UART_BUFFER_FULL    SW receive queue is full and data
 *                                   has been lost
 * \retval #IO_E_UART_OVERFLOW       HW receive buffer overrun
 * \retval #IO_E_UART_PARITY         parity check failed
 * \retval #IO_E_INVALID_CHANNEL_ID  invalid channel ID has been passed
 * \retval #IO_E_NULL_POINTER        null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED channel has not been initialized
 * \retval #IO_E_CH_CAPABILITY       channel is not capable of the desired functionality
 *
 **************************************************************************/
 IO_ErrorType IO_UART_GetRxStatus( IO_PIN channel
                                 , ubyte1 * const rx_len );

/**********************************************************************//**
 *
 * \brief Retrieve the status of the transmit buffer
 *
 * Returns the number of remaining bytes in the SW buffer.
 *
 * \param   channel         UART Channel: \c #IO_UART
 * \param   tx_len          Number of received data frames in receive buffer
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID  invalid channel ID has been passed
 * \retval #IO_E_NULL_POINTER        null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED channel has not been initialized
 * \retval #IO_E_CH_CAPABILITY       channel is not capable of the desired functionality
 *
 **************************************************************************/
 IO_ErrorType IO_UART_GetTxStatus( IO_PIN channel
                                 , ubyte1 * const tx_len );


#endif /* _IO_UART_H */
