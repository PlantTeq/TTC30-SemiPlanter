/**
 * \file Apdb.h
 *
 * \latexonly \hypertarget{APDB}{} \endlatexonly
 *
 * \brief APDB define for bootloader
 *
 *  Contains the definition for the application database. This database is used by the bootloader.
 *
 *  The bootloader needs this information to determine where the application actually starts. For
 *  this reason the field "MainAddress" must be provided by the application.
 *
 *  APDB Usage:
 *   - \ref apdb_examples "Example for APDB definition"
 *
 * \section apdb_examples APDB Code Example
 * \brief Example for using the APDB
 * \subsection apdb_example Example for APDB definition in an application
 * \code
 *     volatile const ApdbType Apdb =
 *     {
 *         APDB_VERSION,           // APDB version
 *         {0},                    // Flash date (provided by TTC-Downloader)
 *                                 // Build date
 *         {((((RTS_TTC_FLASH_DATE_YEAR)   & 0x0FFF) <<  0) |
 *           (((RTS_TTC_FLASH_DATE_MONTH)  & 0x0F  ) << 12) |
 *           (((RTS_TTC_FLASH_DATE_DAY)    & 0x1F  ) << 16) |
 *           (((RTS_TTC_FLASH_DATE_HOUR)   & 0x1F  ) << 21) |
 *           (((RTS_TTC_FLASH_DATE_MINUTE) & 0x3F  ) << 26))},
 *         0,                      // Node type
 *         0,                      // CRC start address (provided by TTC-Downloader)
 *         0,                      // Code size (provided by TTC-Downloader)
 *         0,                      // Legacy application CRC (provided by TTC-Downloader)
 *         0,                      // Application CRC (provided by TTC-Downloader)
 *         1,                      // Node number
 *         0,                      // CRC seed (provided by TTC-Downloader)
 *         0,                      // Flags
 *         0,                      // Hook 1
 *         0,                      // Hook 2
 *         0,                      // Hook 3
 *         APPL_START,             // Main address, i.e., application entry point
 *         {0, 1},                 // CAN download ID (standard format, ID 0x1)
 *         {0, 2},                 // CAN upload ID (standard format, ID 0x2)
 *         0,                      // Legacy header CRC (provided by TTC-Downloader)
 *                                 // Application version (major.minor.revision)
 *         ((((uint32)REVISION_NUMBER) <<  0) |
 *          (((uint32)   MINOR_NUMBER) << 16) |
 *          (((uint32)   MAJOR_NUMBER) << 24)),
 *         500,                    // CAN baud rate in kbps
 *         0,                      // CAN channel
 *         0,                      // Password (disable password protection)
 *         0,                      // Magic seed
 *         { 10, 100,  30, 200},   // Target IP address
 *         {255, 255,   0,   0},   // Subnet mask
 *         {239,   0,   0,   1},   // Multicast IP address
 *         0,                      // Debug key
 *         0,                      // Automatic baud rate detection timeout
 *         0x00,                   // Manufacturer ID (The generic ID 0xFF shall be used for private use.)
 *         0x00,                   // Application ID
 *         {0},                    // Reserved, must be set to zero
 *         0                       // Header CRC (provided by TTC-Downloader)
 *     };
 * \endcode
 *
 * Copyright (c) TTControl. All rights reserved. Confidential and proprietary.
 */
#ifndef APDB_H
#define APDB_H

/**************************************************************************************************
 *
 * I N C L U D E S
 *
 *************************************************************************************************/
#include "TypesGen.h"

/**************************************************************************************************
 *
 * D E F I N I T I O N S
 *
 *************************************************************************************************/
/* APDB version */
#define APDB_VERSION                (0x00000206lu)  /**< Current APDB version is version 2.6.    */

/* APDB size */
#define APDB_SIZE                   (0x80u)         /**< Size of the APDB in bytes               */

/**
 * \name APDB flags
 * \anchor apdb_flags
 *
 * Defined APDB flags.
 */
#define APDB_FLAGS_ABRD_ENABLE      (0x00000001lu)  /**< Enables automatic baudrate detection at
                                                         start-up (HY-TTC30X family only).
                                                         Access mode: read/write.                */
#define APDB_FLAGS_CRC64_ENABLE     (0x40000000lu)  /**< Indicates whether or not CRC-64 is used
                                                         for application CRC.
                                                         Access mode: read only.                 */
#define APDB_FLAGS_MULTI_APP        (0x80000000lu)  /**< Indicates whether or not the app-
                                                         lication is distributed over multiple
                                                         (incoherent) application regions.
                                                         Access mode: read only.                 */

/**************************************************************************************************
 *
 * D A T A   S T R U C T U R E S
 *
 *************************************************************************************************/
/**
 * \brief APDB structure
 *
 *  Data structure for accessing the Application Descriptor Block.
 */
typedef struct
{
    uint32    ApdbVersion;              /**< The APDB version:
                                                bit 0-7  ... minor number
                                                bit 8-15 ... major number                        */
    uint32    FlashDate;                /**< The date when the application has been flashed
                                             (automatically provided by the TTC-Downloader).     */
    uint32    BuildDate;                /**< The application's build date (must be provided by
                                             the customer).                                      */
    uint32    NodeType;                 /**< The hardware type the application is built for.     */
    uint32    CrcStartAddress;          /**< Start address for CRC calculation or if a CRC table
                                             is used, start address of the CRC table
                                             (automatically provided by the TTC-Downloader).     */
    uint32    CodeSize;                 /**< Code size in bytes (used for CRC calculation) or if
                                             a CRC table is used, number of CRC table entries
                                             (automatically provided by the TTC-Downloader).     */
    uint32    LegacyApplicationCrc;     /**< Legacy application CRC for flash checker
                                             (automatically provided by the TTC-Downloader).     */
    uint32    ApplicationCrc;           /**< CRC-32 value calculated over the application or if a
                                             CRC table is used, CRC-32 value calculated over the
                                             CRC table (automatically provided by the
                                             TTC-Downloader).                                    */
    uint32    NodeNumber;               /**< The unique node number used to identify nodes of a
                                             CODESYS application. Note that only values from 0 to
                                             127 are allowed.                                    */
    uint32    CrcSeed;                  /**< Seed for application CRC calculation (automatically
                                             provided by the TTC-Downloader).                    */
    uint32    Flags;                    /**< Predefined application flags can be specified here.
                                             Following flags can be specified:
                                                bit 0 ... Auto-BaudrateDetectionEnable
                                                   0 = disable automatic baud rate detection
                                                   1 = enable automatic baud rate detection      */
    uint32    Hook1;                    /**< Custom hook 1.                                      */
    uint32    Hook2;                    /**< Custom hook 2.                                      */
    uint32    Hook3;                    /**< Custom hook 3.                                      */
    uint32    MainAddress;              /**< The application's vector address. Note that the
                                             bootloader uses this address to start the application
                                             after reset/power-up.                               */
    CanIdType CanDownloadId;            /**< The CAN identifier used for download direction
                                             (TTC-Downloader -> target).                         */
    CanIdType CanUploadId;              /**< The CAN identifier used for upload direction
                                             (target -> TTC-Downloader).                         */
    uint32    LegacyHeaderCrc;          /**< Legacy header CRC for flash checker (automatically
                                             provided by the TTC-Downloader).                    */
    uint32    ApplicationVersion;       /**< The application version (must be provided by the
                                             customer):
                                                bit  0-15 ... revision number
                                                bit 16-23 ... minor number
                                                bit 24-31 ... major number                       */
    uint32    CanBaudrate;              /**< Baud rate in kbit/s used for CAN communication.     */
    uint32    CanChannel;               /**< The channel used for CAN communication.             */
    uint32    Password;                 /**< The password hash for memory access. Set this field
                                             to 0 or 0xFFFFFFFF to disable password protection.
                                             Note that it is highly recommended to set a password
                                             upon application download with the TTC-Downloader.  */
    uint32    MagicSeed;                /**< Seed for CRC calculation with the MCHK HW module.   */
    uint8     TargetIpAddress[4];       /**< Target IP address for Ethernet download (most
                                             significant byte first).                            */
    uint8     SubnetMask[4];            /**< Subnet mask for Ethernet download (most
                                             significant byte first).                            */
    uint8     DlMulticastIpAddress[4];  /**< Multicast IP address of the TTC-Downloader (most
                                             significant byte first).                            */
    uint32    DebugKey;                 /**< Debug key for booting the device in debug mode.     */
    uint32    AbrdTimeout;              /**< The timeout for automatic CAN baud rate detection.  */
    uint8     ManufacturerId;           /**< The manufacturer identifier (must be provided by the
                                             customer). The generic ID 0xFF shall be used for
                                             private use.                                        */
    uint8     ApplicationId;            /**< The application identifier (must be provided by the
                                             customer).                                          */
    uint16    Reserved;                 /**< Reserved for future use. Shall be set to 0.         */
    uint32    HeaderCrc;                /**< The CRC value calculated over the whole APDB (except
                                             HeaderCRC field).                                   */
} ApdbType;

// /***********************************************************************************************
// *
// * E X T E R N A L S
// *
// ***********************************************************************************************/
// /**
// * \name Application database (APDB)
// *
// * The APDB, which is defined by the user.
// */
// extern volatile const ApdbType Apdb;

#endif /* APDB_H */

/* EOF */
