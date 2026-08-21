/**************************************************************************
 * Copyright (c) 2014 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_EEPROM_Preload.h
 *
 * \brief Pre-load functions for EEPROM
 *
 * The EEPROM pre-load functions allow writing to the SPI
 * EEPROM as well as reading from it without initializing the
 * whole IO driver. Therefore it is possible to read data from the memory
 * which could be necessary for a proper initialization of the IO driver
 * (e.g. safety configuration data).
 *
 * The EEPROM driver is a high level SPI device driver and
 * uses the low level SPI driver.
 *
 * The memory amounts 8128 Byte, because 64 Byte of the EEPROM are
 * reserved for internal use.
 *
 * Since the content of the EEPROM is not initialized or verified during
 * the production procedure, there is no guarantee of a defined
 * initial content (of 0xFF) on any position.
 *
 * The communication with the EEPROM is handled in a cyclic manner.
 * EEPROM operations take plenty of time. A write as well as a
 * read process is only triggered by calling the respective function.
 * A status function is provided which returns whether the driver
 * has finished the last task or not.
 *
 * The EEPROM driver gives the option of calculating CRC32 checksums
 * (size of 4 Bytes, polynome 0x04C11DB7 and initialization
 * value 0x00000000) on write and read operations. When enabled, the
 * user must take care to leave enough space for the checksum and
 * not to overwrite it. For example, writing to offset zero data
 * with a size of 6 Bytes, the checksum will be appended to this 6
 * Bytes. This means, that offsets 0-5 contain the actual written
 * data and offsets 6-9 contain the checksum. The user must not
 * overwrite the bytes 6-9 via other calls of #IO_EEPROM_Write.
 * Otherwise the next read operation with checksum enabled will
 * result in an #IO_E_EEPROM_CRC_MISMATCH error.
 *
 * \section eeprom_preload_examples EEPROM pre-load code examples
 *
 * Please refer to section \ref appl_structure for understanding where to place the initialization and task function calls.
 *
 * Typical usage of the pre-load functions:
 * \code
 * ubyte1       data[50] = {0};
 *
 * IO_EEPROM_PreloadInit();
 *
 * IO_EEPROM_PreloadRead(0, 50, FALSE, data);
 * while( IO_EEPROM_PreloadStatus() != IO_E_OK)
 * {
 *     IO_EEPROM_PreloadTask();
 * }
 *
 * IO_EEPROM_PreloadDeInit();
 * \endcode
 *
 * EEPROM write example:
 * \code
 * ubyte1 data[6] = {0, 1, 2, 3, 4, 5};
 *
 * // check if EEPROM is busy
 * if (IO_EEPROM_PreloadStatus() == IO_E_OK)
 * {
 *     // if not busy write data without checksum
 *     IO_EEPROM_PreloadWrite(0, 6, FALSE, data);
 *
 *     // wait until write cycle is over
 *     while( IO_EEPROM_PreloadStatus() != IO_E_OK)
 *     {
 *         IO_EEPROM_PreloadTask();
 *     }
 * }
 *
 * \endcode
 *
 * EEPROM read example:
 * \code
 * ubyte1 data[2000] = {0};
 *
 * // check if EEPROM is busy
 * if (IO_EEPROM_PreloadStatus() == IO_E_OK)
 * {
 *     // if not busy start reading without checksum
 *     IO_EEPROM_PreloadRead(0, 2000, FALSE, data);
 *
 *     // wait until read cycle is over
 *     while( IO_EEPROM_PreloadStatus() != IO_E_OK)
 *     {
 *         IO_EEPROM_PreloadTask();
 *     }
 * }
 *
 * \endcode
 *
 * \note
 * It is highly recommended to de-initialize the EEPROM pre-load module
 * before calling the IO_Driver_Init() function.
 *
 **************************************************************************/
#ifndef _IO_EEPROM_PRELOAD_H
#define _IO_EEPROM_PRELOAD_H

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
 * \brief Preload initialization of the EEPROM driver.
 *
 *   Initialization of EEPROM driver.
 *      - Initializes internal data structure
 *      - Configures SPI driver
 *
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_CHANNEL_BUSY        module has been initialized before
 *
 **************************************************************************
 *
 * \remarks
 *   - Module is initialized only once.
 *   To re-initialize the module, the function \c IO_EEPROM_PreloadDeInit()
 *   needs to be called.
 *
 **************************************************************************/
IO_ErrorType IO_EEPROM_PreloadInit(void);

 /**********************************************************************//**
  *
  * \brief De-initializes the preloaded EEPROM driver.
  *
  *   De-initializes the EEPROM module as well as the SPI driver.
  *
  *
  * \return #IO_ErrorType
  * \retval #IO_E_OK                        everything fine
  * \retval #IO_E_CHANNEL_NOT_CONFIGURED    the module is not initialized
  *
  **************************************************************************/
IO_ErrorType IO_EEPROM_PreloadDeInit(void);

/**********************************************************************//**
 *
 * \brief Read data from the pre-loaded EEPROM module
 *
 *   The function only triggers a read operation.
 *   The read operation can take several cycles to be completed,
 *   depending on the SPI load and the amount of data to be read.
 *
 *   The read data is available on the address where the data parameter
 *   points to as soon as the read operation is finished.
 *   The state can be polled using the IO_EEPROM_PreloadStatus() function.
 *
 *
 * \param   offset      EEPROM memory offset (0..8127)
 * \param   length      Length of data to be read (0..8128)
 * \param   use_crc     Indicates if the CRC value stored by IO_EEPROM_Write
 *                      should be read and evaluated
 * \param   data        Pointer to data
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                         everything fine
 * \retval #IO_E_BUSY                       EEPROM module is still busy
 * \retval #IO_E_EEPROM_RANGE               invalid address offset or range
 * \retval #IO_E_NULL_POINTER               a null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED     the module is not initialized
 *
 ***************************************************************************/
IO_ErrorType IO_EEPROM_PreloadRead( ubyte2 offset
                                  , ubyte2 length
                                  , bool use_crc
                                  , ubyte1 * const data);

/**********************************************************************//**
  *
  * \brief Write data to the pre-loaded EEPROM module
  *
  *   The function triggers a write operation.
  *   The write operation can take several cycles to be completed,
  *   depending on the SPI load and the amount of data to be read.
  *
  *   The write operation is completed as soon as the SPI communication
  *   has been finished.
  *   The state can be polled using the IO_EEPROM_PreloadStatus() function.
  *
  *
  * \param   offset      EEPROM memory offset (0..8127)
  * \param   length      Length of data to be written (0..8128)
  * \param   use_crc     Indicates if a CRC value (4-bytes) should be stored automatically
  *                      immediately after the written bytes. If TRUE, the
  *                      maximum writable address is 8124.
  * \param   data        Pointer to data
  *
  * \return #IO_ErrorType
  * \retval #IO_E_OK                        everything fine
  * \retval #IO_E_BUSY                      EEPROM module is still busy
  * \retval #IO_E_EEPROM_RANGE              invalid address offset or range
  * \retval #IO_E_NULL_POINTER              a null pointer has been passed
  * \retval #IO_E_CHANNEL_NOT_CONFIGURED    the module is not initialized
  *
  * \note
  *   Please note that the parameter 'data' must not be a local variable
  *   since the driver does not create a copy of the content but saves the
  *   address of the pointer.
  *
  ***************************************************************************/
IO_ErrorType IO_EEPROM_PreloadWrite( ubyte2 offset
                                   , ubyte2 length
                                   , bool use_crc
                                   , const ubyte1 * const data);

/**********************************************************************//**
 *
 * \brief Returns the status of the pre-load EEPROM driver
 *
 *   Returns whether the EEPROM is idle or if a read or write operation
 *   is ongoing.
 *
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine, driver is idle
 * \retval #IO_E_EEPROM_CRC_MISMATCH    the stored and calculated CRC value of
 *                                      a read operation do not match
 * \retval #IO_E_BUSY                   a read or a write operation is ongoing,
 *                                      driver is busy.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the module is not initialized
 *
 **************************************************************************/
IO_ErrorType IO_EEPROM_PreloadStatus(void);

/**********************************************************************//**
 *
 * \brief Task function for the pre-load EEPROM driver
 *
 *   Handles the SPI communication and calls the EEPROM read/write function task.
 *
 *   When using the EEPROM pre-load functions it is mandatory to call the
 *   task function periodically. Otherwise no SPI communication takes place
 *   and any read or write operation will fail.
 *
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                 everything fine
 * \retval #IO_E_BUSY               SPI communication is still ongoing
 *
 **************************************************************************/
IO_ErrorType IO_EEPROM_PreloadTask(void);

#endif /* _IO_EEPROM_PRELOAD_H */
