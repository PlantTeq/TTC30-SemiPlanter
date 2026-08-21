/**************************************************************************
 * Copyright (c) 2018 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 *
 * \file IO_BRBL.h
 *
 * \brief API for accessing data in the branding block of the ECU
 *
 * \section read_did Read DataIdentifiers stored in the Branding Block
 *
 * This section describes how to read a DataIdentifier that has been stored
 * in the ECUs braning block.
 *
 * \code
 * IO_ErrorType rc_validate;
 * IO_ErrorType rc_get_did;
 * IO_ErrorType rc_get_can_params;
 * IO_BRBL_CUSTOM_DID did_item;
 * IO_BRBL_CAN_PARAM  can_params;
 *
 * // First the consistency of the BRBL has to be checked. The validity of
 * // the BRBL has to be checked only once per power cycle. It is best to
 * // call this function during the init phase (directly after calling \c IO_Driver_Init)
 * // because validating the BRBL will take approximately 1.2ms.
 * rc_brbl_validate = IO_BRBL_Validate();
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
 *
 *     rc_get_can_params = IO_BRBL_GetCanParam( &can_params );
 *     if (rc_get_can_params == IO_E_OK)
 *     {
 *         // The can parameters have been successfully read.
 *     }
 * }
 * else
 * {
 *     // Reading from the BRBL is not possible because it does
 * }
 * \endcode
 *
 **************************************************************************/

#ifndef IO_BRBL_H_
#define IO_BRBL_H_


/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/
#include "ptypes_xe167.h"
#include "IO_Constants.h"


/**************************************************************************
 *
 * D E F I N I T I O N S
 *
 **************************************************************************/

/**
 * \brief
 */
/*\{*/
#define IO_BRBL_XTEA_PRIV_KEY_LEN           4U      /**< Length of private key in 32bit words */
/*\}*/

/**
 * \brief Definitions for private key table.
 */
/*\{*/
#define IO_BRBL_XTEA_PRIV_KEY_IDX_0         0U      /**< Reserved by ECU manufacturer! */
#define IO_BRBL_XTEA_PRIV_KEY_IDX_1         1U      /**< Application reprogramming key. */
#define IO_BRBL_XTEA_PRIV_KEY_IDX_2         2U      /**< Key 2. */
#define IO_BRBL_XTEA_PRIV_KEY_IDX_3         3U      /**< Key 3. */
#define IO_BRBL_XTEA_PRIV_KEY_IDX_4         4U      /**< Key 4. */
#define IO_BRBL_XTEA_PRIV_KEY_IDX_5         5U      /**< Key 5. */
#define IO_BRBL_XTEA_PRIV_KEY_IDX_6         6U      /**< Key 6. */
#define IO_BRBL_XTEA_PRIV_KEY_IDX_7         7U      /**< Key 7. */
#define IO_BRBL_XTEA_PRIV_KEY_IDX_8         8U      /**< Key 8. */
#define IO_BRBL_XTEA_PRIV_KEY_IDX_9         9U      /**< Key 9. */
#define IO_BRBL_XTEA_PRIV_KEY_IDX_10        10U     /**< Key 10. */
#define IO_BRBL_XTEA_PRIV_KEY_IDX_11        11U     /**< Key 11. */
#define IO_BRBL_XTEA_PRIV_KEY_TBL_LEN       12U     /**< Number of entries available in private key table. */
/*\}*/

/**
 * \brief Definitions for custom DID table.
 */
/*\{*/
#define IO_BRBL_CUSTOM_DID_IDX_0            0U      /**< Custom DID at index 0  */
#define IO_BRBL_CUSTOM_DID_IDX_1            1U      /**< Custom DID at index 1  */
#define IO_BRBL_CUSTOM_DID_IDX_2            2U      /**< Custom DID at index 2  */
#define IO_BRBL_CUSTOM_DID_IDX_3            3U      /**< Custom DID at index 3  */
#define IO_BRBL_CUSTOM_DID_IDX_4            4U      /**< Custom DID at index 4  */
#define IO_BRBL_CUSTOM_DID_IDX_5            5U      /**< Custom DID at index 5  */
#define IO_BRBL_CUSTOM_DID_IDX_6            6U      /**< Custom DID at index 6  */
#define IO_BRBL_CUSTOM_DID_IDX_7            7U      /**< Custom DID at index 7  */
#define IO_BRBL_CUSTOM_DID_IDX_8            8U      /**< Custom DID at index 8  */
#define IO_BRBL_CUSTOM_DID_IDX_9            9U      /**< Custom DID at index 9  */
#define IO_BRBL_CUSTOM_DID_IDX_10           10U     /**< Custom DID at index 10 */
#define IO_BRBL_CUSTOM_DID_IDX_11           11U     /**< Custom DID at index 11 */
#define IO_BRBL_CUSTOM_DID_IDX_12           12U     /**< Custom DID at index 12 */
#define IO_BRBL_CUSTOM_DID_IDX_13           13U     /**< Custom DID at index 13 */
#define IO_BRBL_CUSTOM_DID_IDX_14           14U     /**< Custom DID at index 14 */
#define IO_BRBL_CUSTOM_DID_IDX_15           15U     /**< Custom DID at index 15 */
#define IO_BRBL_CUSTOM_DID_TBL_LEN          16U     /**< Number of entries available in the DID table. */
/*\}*/


/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/**
 *
 * \brief CAN ID structure
 *
 */
typedef struct _io_brbl_can_id_
{
    ubyte4 extended;                /**< Type of CAN identifier to be used. Valid values are:
                                            0 ... standard CAN identifier is used
                                            1 ... extended CAN identifier is used                */
    ubyte4 ID;                      /**< The CAN identifier (LSB must start at bit 0):
                                            bit 0-10 ... if standard CAN identifier is used
                                            bit 0-28 ... if extended CAN identifier is used      */
} IO_BRBL_CAN_ID;

/**
 * \brief Branding block CAN parameter structure
 */
typedef struct _io_brbl_can_param
{
    ubyte4 CANBaudrate;                     /**< Baud rate in kbit/s used for CAN communication. */
    ubyte4 CANChannel;                      /**< The channel used for CAN communication.         */
    IO_BRBL_CAN_ID CANDownloadID;           /**< The CAN identifier used for download direction
                                             *   (TTC-Downloader -> target).
                                             *                                                   */
    IO_BRBL_CAN_ID CANUploadID;             /**< The CAN identifier used for upload direction
                                             *   (target -> TTC-Downloader).
                                             *                                                   */
    IO_BRBL_CAN_ID UDSOnCANRxID;            /**< The CAN identifier used for download direction
                                             *   (diagnostic tester -> target).
                                             *                                                   */
    IO_BRBL_CAN_ID UDSOnCANTxID;            /**< The CAN identifier used for upload direction
                                             *   (target -> diagnostic tester).
                                             *                                                   */
    IO_BRBL_CAN_ID UDSOnCANFuncRxID;        /**< The CAN identifier used for download direction
                                             *   (diagnostic tester -> target).
                                             *                                                   */
} IO_BRBL_CAN_PARAM;

/**
 * \brief Entry definition for DID table.
 */
typedef struct _io_brbl_dids
{
    ubyte2 Did;                                 /**< Data Identifier */
    ubyte2 DidLength;                           /**< Length of Data Identifier */
    const ubyte1 * DidData;                     /**< Pointer to DID data. DID data must be stored MSB first
                                                  *  (Array index 0 -> MSB, index X -> LSB) and must not exceed
                                                  *  DidLength. */
}IO_BRBL_CUSTOM_DID;

/**************************************************************************
 *
 * F U N C T I O N S
 *
 **************************************************************************/

/**********************************************************************//**
 *
 * \brief Validates the branding block.
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK             Everything ok.
 * \retval #IO_E_INVALID_CRC    Branding block inconsistent.
 * \retval #IO_E_UNKNOWN        Branding block does not fit to the
 *                              installed bootloader.
 *
 **************************************************************************/
IO_ErrorType IO_BRBL_Validate( void );


/**********************************************************************//**
 *
 * \brief Returns a secret key from the secret key table that is located
 *        in the branding block.
 *
 * \param[in]   key_num         Index within key table to read from.
 *                              Can be one of:
 *                              IO_BRBL_XTEA_PRIV_KEY_IDX_0 .. IO_BRBL_XTEA_PRIV_KEY_IDX_11
 * \param[out]  key_buf         Array with 4 elements to store the key to.
 * \param[in]   key_buf_len     Length of array passed for parameter
 *                              \c key_buf
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                     Everything ok.
 * \retval #IO_E_NULL_POINTER           A NULL pointer has been passed.
 * \retval #IO_E_INVALID_PARAMETER      An invalid parameter has been passed
 *                                      (out of range).
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED Branding block has not been validated
 *                                      successfully yet.
 *
 **************************************************************************
 *
 * \remark The secret key is returned encrypted. It can only be used in
 *         combination with the cipher functions in the module \c IO_Crypt.
 *
 **************************************************************************/
IO_ErrorType IO_BRBL_GetXteaKey( ubyte1 key_num
                               , ubyte4 * const key_buf
                               , ubyte1 key_buf_len );

/**********************************************************************//**
 *
 * \brief Returns a DID from the DID table that is located
 *        in the branding block.
 *
 * \param[in]   did_tbl_idx     Index within DID table to read from.
 *                              Can be one of:
 *                              IO_BRBL_CUSTOM_DID_IDX_0 .. IO_BRBL_CUSTOM_DID_IDX_15
 * \param[out]  did_entry       Structure that holds the DID number,
 *                              the length of its data and a pointer
 *                              to the DID data.
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                     Everything ok.
 * \retval #IO_E_NULL_POINTER           A NULL pointer has been passed.
 * \retval #IO_E_INVALID_PARAMETER      An invalid parameter has been passed
 *                                      (out of range).
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED Branding block has not been validated
 *                                      successfully yet.
 *
 **************************************************************************/
IO_ErrorType IO_BRBL_GetDid( ubyte1 did_tbl_idx
                           , IO_BRBL_CUSTOM_DID * const did_entry );

/**********************************************************************//**
 *
 * \brief Returns the CAN parameters set in the Branding Block
 *
 * \param[out]  brbl_can_param  Branding block CAN parameter structure
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                     Everything ok.
 * \retval #IO_E_NULL_POINTER           A NULL pointer has been passed.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED Branding block has not been validated
 *                                      successfully yet.
 *
 **************************************************************************/
IO_ErrorType IO_BRBL_GetCanParam( IO_BRBL_CAN_PARAM * const brbl_can_param );

#endif /* IO_BRBL_H_ */
