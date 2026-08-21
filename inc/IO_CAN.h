/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file IO_CAN.h
 *
 * \brief IO Driver functions for CAN communication.
 *
 * The CAN driver uses the MultiCAN module of the XC2000 CPU.
 *
 * The CAN driver supports 1 CAN channel (2 CAN channels for HY-TTC 32 variants), so-called CAN nodes.
 *
 * All CAN nodes share a common set of message objects, where each
 * message object may be individually allocated to one of the CAN
 * nodes. Besides serving as a storage container for incoming and
 * outgoing frames, message objects may be combined to build gateways
 * between the CAN nodes or to setup a FIFO buffer.
 *
 * The message objects are organized in double chained lists, where
 * each CAN node has its own list of message objects. A CAN node
 * stores frames only into message objects that are allocated to
 * the list of the CAN node. It only transmits messages from objects
 * of this list.
 *
 * The functions 'remote acknowledge' and 'remote request' are not
 * supported.
 *
 * \section can_ac_mask_usage Usage of the acceptance masks:
 *
 * The acceptance mask defines the relevant bits of the CAN ID.
 * A binary 1 marks a relevant bit in the CAN ID on the same position.
 *
 * Setting all bits of the acceptance mask (0x1FFFFFFF) only accepts
 * the ID set with the ID parameter and rejects all other IDs.
 * Setting the acceptance mask to 0 causes the message buffer to
 * accept any IDs.
 *
 * Using this mechanism a message buffer can be used to accept a
 * range of CAN IDs.
 *
 * Example:
 * \code
 *     ac_mask = 0x1FFFFF00 = 0 b 1 1111 1111 1111 1111 1111 0000 0000
 *     id      = 0x00000200 = 0 b 0 0000 0000 0000 0000 0010 0000 0000
 * \endcode
 * in this example all messages with an ID between 0x200 and 0x2FF
 * are accepted.
 *
 * \section can_example CAN code examples
 *
 * Please refer to section \ref appl_structure for understanding where to place the initialization and task function calls.
 *
 * Examples for CAN initialization:
 * \code
 * ubyte1 handle_w, handle_r, handle_fifo_w, handle_fifo_r;
 *
 * IO_CAN_Init( IO_CAN_CHANNEL_0
 *            , IO_CAN_BAUDRATE_500K ); // Configure with 500 kbit/s
 *
 *
 * // standard message buffers //
 *
 * IO_CAN_ConfigMsg( &handle_w
 *                 , IO_CAN_CHANNEL_0  // channel 0
 *                 , IO_CAN_MSG_WRITE  // transmit message buffer
 *                 , IO_CAN_STD_FRAME  // standard ID
 *                 , 0
 *                 , 0);
 *
 * IO_CAN_ConfigMsg( &handle_r
 *                 , IO_CAN_CHANNEL_0  // channel 0
 *                 , IO_CAN_MSG_READ   // receive message buffer
 *                 , IO_CAN_STD_FRAME  // standard ID
 *                 , 1
 *                 , 0x1FFFFFFF);      // accept only ID 1
 *
 * // FIFO message buffer //
 *
 * IO_CAN_ConfigFIFO( &handle_fifo_w
 *                  , IO_CAN_CHANNEL_0 // channel 0
 *                  , 20               // 20 items
 *                  , IO_CAN_MSG_WRITE // transmit fifo buffer
 *                  , IO_CAN_STD_FRAME // standard ID
 *                  , 0
 *                  , 0 );
 *
 * IO_CAN_ConfigFIFO( &handle_fifo_r
 *                  , IO_CAN_CHANNEL_0 // channel 0
 *                  , 20               // 20 items
 *                  , IO_CAN_MSG_READ  // receive fifo buffer
 *                  , IO_CAN_STD_FRAME // standard ID
 *                  , 0
 *                  , 0 );             // accept every ID
 *
 * \endcode
 *
 * Examples for CAN task function calls:
 * \code
 * IO_CAN_DATA_FRAME can_frame;
 * IO_ErrorType can_status;
 *
 * // check if new message has been received
 * can_status = IO_CAN_MsgStatus(handle_r);
 * 
 * if ((can_status == IO_E_OK) || (can_status == IO_E_CAN_OVERFLOW))
 * {
 *     // if message has been received, read the message from the buffer
 *     IO_CAN_ReadMsg(handle_r, &can_frame);
 *
 *     // received message is now stored in can_frame
 *     // and can be used by the application
 * }
 *
 *
 * // assemble CAN frame:
 * can_frame.id = 1;
 * can_frame.id_format = IO_CAN_STD_FRAME;
 * can_frame.length = 4;
 * can_frame.data[0] = 1;
 * can_frame.data[1] = 2;
 * can_frame.data[2] = 3;
 * can_frame.data[3] = 4;
 *
 * // transmit message
 * IO_CAN_WriteMsg(handle_w, &can_frame);
 *
 * // wait until the transmission has been finished:
 * while (IO_CAN_MsgStatus(handle_w) != IO_E_OK);
 *
 * \endcode
 *
 **************************************************************************/
#ifndef _IO_CAN_H
#define _IO_CAN_H

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

/**
 * \name CAN channels
 * \anchor can_channels_def
 *
 * Selects the CAN communication channel.
 */
/*@{*/
/** CAN Channel 0 \n
 *  uses the ECU pins C2 (CAN-H) and B2 (CAN-L)
 *  \n  \n
 *  CAN channel 1 (only available for HY-TTC 32 variants) \n
 *  uses the ECU pins K3 (CAN-H) and J3 (CAN-L)
 */
//#define IO_CAN_CHANNEL_0    IO_INT_PIN_CAN_CH0
/*@}*/

/**
 * \name Message buffer direction
 * \anchor can_buffer_direction
 *
 * Selects the transmission direction of a CAN message buffer
 */
/*@{*/
#define IO_CAN_MSG_READ     0   /**< used to setup a message buffer for receiving    */
#define IO_CAN_MSG_WRITE    1   /**< used to setup a message buffer for transmitting */
/*@}*/

/**
 * \name CAN frame format
 * \anchor can_buffer_direction
 *
 * Selects the format for a CAN frame
 */
/*@{*/
#define IO_CAN_STD_FRAME    0   /**< the ID parameter holds a standard (11-bit) ID  */
#define IO_CAN_EXT_FRAME    1   /**< the ID parameter holds an extended (29-bit) ID */
/*@}*/

/** Configure CAN Channel with 10KBit/s. Used settings:\n
 * BRP = 25, TSEG1 = 16, TSEG2 = 3, SJW = 2 and DIV8 = TRUE (Sampling Point 85%)
 */
#define IO_CAN_BAUDRATE_10K     0

/** Configure CAN Channel with 20KBit/s. Used settings:\n
 * BRP = 10, TSEG1 = 16, TSEG2 = 8, SJW = 2 and DIV8 = TRUE (Sampling Point 68%)
 */
#define IO_CAN_BAUDRATE_20K     1

/** Configure CAN Channel with 25KBit/s. Used settings:\n
 * BRP = 10, TSEG1 = 16, TSEG2 = 3, SJW = 2 and DIV8 = TRUE (Sampling Point 85%)
 */
#define IO_CAN_BAUDRATE_25K     2

/** Configure CAN Channel with 50KBit/s. Used settings:\n
 * BRP = 50, TSEG1 = 13, TSEG2 = 2, SJW = 2 and DIV8 = FALSE (Sampling Point 85%)
 */
#define IO_CAN_BAUDRATE_50K     3

/** Configure CAN Channel with 100KBit/s. Used settings:\n
 * BRP = 25, TSEG1 = 13, TSEG2 = 2, SJW = 2 and DIV8 = FALSE (Sampling Point 87.5%)
 */
#define IO_CAN_BAUDRATE_100K    4

/** Configure CAN Channel with 125KBit/s. Used settings:\n
 * BRP = 20, TSEG1 = 13, TSEG2 = 2, SJW = 2 and DIV8 = FALSE (Sampling Point 87.5%)
 */
#define IO_CAN_BAUDRATE_125K    5

/** Configure CAN Channel with 250KBit/s. Used settings:\n
 * BRP = 10, TSEG1 = 13, TSEG2 = 2, SJW = 2 and DIV8 = FALSE (Sampling Point 87.5%)
 */
#define IO_CAN_BAUDRATE_250K    6

/** Configure CAN Channel with 500KBit/s. Used settings:\n
 * BRP = 5, TSEG1 = 13, TSEG2 = 2, SJW = 2 and DIV8 = FALSE (Sampling Point 87.5%)
 */
#define IO_CAN_BAUDRATE_500K    7

/** Configure CAN Channel with 800KBit/s. Used settings:\n
 * BRP = 2, TSEG1 = 16, TSEG2 = 8, SJW = 2 and DIV8 = FALSE (Sampling Point 68%)
 */
#define IO_CAN_BAUDRATE_800K    8

/** Configure CAN Channel with 1000KBit/s. Used settings:\n
 * BRP = 2, TSEG1 = 16, TSEG2 = 3, SJW = 2 and DIV8 = FALSE (Sampling Point 85%)
 */
#define IO_CAN_BAUDRATE_1000K   9

/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/**
 *
 * \brief CAN data frame
 *
 *  Stores a data frame for the CAN communication.
 *
 */
typedef struct _io_can_data_frame
{
    ubyte1 data[8];      /**< data buffer                          */
    ubyte1 length;       /**< number of words in transmit buffer   */
    ubyte1 id_format;    /**< standard or extended format          */
    ubyte4 id;           /**< ID for CAN communication             */
} IO_CAN_DATA_FRAME;


/**************************************************************************
 *
 * F U N C T I O N S
 *
 **************************************************************************/

/**********************************************************************//**
 *
 * \brief Initialization of the CAN communication driver.
 *
 *   The function
 *      - Enables the module
 *      - Sets the module clock to 40MHz
 *      - Automatically sets up the bit timing for the given baudrate
 *
 *
 * \param   channel         CAN channel, one of:
 *                               - \c #IO_CAN_CHANNEL_0
 *                               - \c #IO_CAN_CHANNEL_1 \note Second CAN Interface (IO_CAN_CHANNEL_1) is only available for HY-TTC 32 and HY-TTC 32S
 * \param   baudrate        Baud rate
 *                               - \c #IO_CAN_BAUDRATE_10K
 *                               - \c #IO_CAN_BAUDRATE_20K
 *                               - \c #IO_CAN_BAUDRATE_25K
 *                               - \c #IO_CAN_BAUDRATE_50K
 *                               - \c #IO_CAN_BAUDRATE_100K
 *                               - \c #IO_CAN_BAUDRATE_125K
 *                               - \c #IO_CAN_BAUDRATE_250K
 *                               - \c #IO_CAN_BAUDRATE_500K
 *                               - \c #IO_CAN_BAUDRATE_800K
 *                               - \c #IO_CAN_BAUDRATE_1000K
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_INVALID_PARAMETER   invalid parameter has been passed
 * \retval #IO_E_INVALID_CHANNEL_ID  invalid channel ID has been passed
 * \retval #IO_E_CHANNEL_BUSY        channel has been initialized before
 * \retval #IO_E_CH_CAPABILITY       the given channel does not support the desired functionality
 *
 **************************************************************************
 *
 * \remarks
 *   Module is initialized only once.
 *   To re-initialize the module, the function IO_CAN_DeInit()
 *   needs to be called.
 *
 **************************************************************************/
IO_ErrorType IO_CAN_Init( IO_PIN channel
                        , ubyte1 baudrate );

/**********************************************************************//**
 *
 * \brief Initialization of the CAN communication driver for given bit timings.
 *
 *  The function
 *     - Enables the module
 *     - Sets the module clock to 40MHz
 *     - Configures the CAN channel with the given bit timings
 *
 * \param   channel         CAN channel, one of:
 *                               - \c #IO_CAN_CHANNEL_0
 *                               - \c #IO_CAN_CHANNEL_1 \note Second CAN Interface (IO_CAN_CHANNEL_1) is only available for HY-TTC 32 and HY-TTC 32S
 * \param   brp             Baudrate prescaler (1 ... 63)
 * \param   div8            Configures an additional clock divider of 8
 * \param   tseg1           Time segment before sample point (3 ... 16)
 * \param   tseg2           Time segment after sample point (2 ... 8)
 * \param   sjw             Synchronization jump width (1 ... 4)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_INVALID_PARAMETER   invalid parameter has been passed
 * \retval #IO_E_INVALID_CHANNEL_ID  invalid channel ID has been passed
 * \retval #IO_E_CHANNEL_BUSY        channel has been initialized before
 * \retval #IO_E_CH_CAPABILITY       the given channel does not support the desired functionality
 *
 **************************************************************************
 *
 * \remarks
 *   - The timing parameters and baudrate are calculated as follows:\n
 *   The time quanta "tq" results from the Baudrate prescaler and the
 *   additional clock divider:\n
 *   tq = brp / 40,000,000Hz         , if div8 = FALSE\n
 *   tq = 8 * brp / 40,000,000Hz     , if div8 = TRUE\n
 *   \n
 *   The syncronization time (Tsync [s]), Phase Buffer Segment Time 1 (Tseg1 [s])\n
 *   and Phase Buffer Segment Time (Tseg2 [s]) are calculated as follows based
 *   upon "tq":\n
 *   Tsync = 1 * tq\n
 *   TSeg1 = seg1 * tq\n
 *   TSeg2 = seg2 * tq\n
 *   \n
 *   The overall bit time [s] and baudrate [bit/s] are calculated with:\n
 *   bit_time = Tsync + TSeg1 + TSeg2\n
 *   baudrate = 1 / bit_time\n
 *   \n
 *   Example:
 *   brp = 5, seg1 = 13, seg2 = 2, sjw = 2 and div8 = FALSE\n
 *   tq = 5 / 40,000,000Hz = 125ns\n
 *   Tsync = 1 * 125ns = 125ns\n
 *   TSeg1 = 13 * 125ns = 1625ns\n
 *   TSeg2 = 2 * 125ns = 250ns\n
 *   \n
 *   bit_time = 125ns + 1625ns + 250ns = 2000ns\n
 *   baudrate = 1/2000ns = 500,000 bit/s\n
 *   \n
 *   The sampling point can be calculated with:\n
 *   sampling_point = (1 + seg1) / (1 + seg1 + seg2)
 *
 * \remarks
 *   - Module is initialized only once.
 *   To re-initialize the module, the function \c IO_CAN_DeInit()
 *   needs to be called.
 **************************************************************************/
IO_ErrorType IO_CAN_InitTimings( IO_PIN channel
                               , ubyte1 brp
                               , bool   div8
                               , ubyte1 tseg1
                               , ubyte1 tseg2
                               , ubyte1 sjw );

/**********************************************************************//**
 *
 * \brief Deinitializes a single message handle
 *
 * Allows re-initialization by \c IO_CAN_ConfigMsg() or \c IO_CAN_ConfigFIFO()
 *
 *
 * \param   handle      CAN message handle
 *                          (retrieved from \c IO_CAN_ConfigMsg()
 *                           or \c IO_CAN_ConfigFIFO() )
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_PARAMETER       invalid parameter has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  handle has not been initialized
 *
 **************************************************************************/
 IO_ErrorType IO_CAN_DeInitHandle(ubyte1 handle);

/**********************************************************************//**
 *
 * \brief Deinitializes the given CAN channel
 *
 * Allows re-initialization by \c IO_CAN_Init()
 *
 *
 * \param   channel         CAN channel, one of:
 *                               - \c IO_CAN_CHANNEL_0
 *                               - \c #IO_CAN_CHANNEL_1 \note Second CAN Interface (IO_CAN_CHANNEL_1) is only available for HY-TTC 32 and HY-TTC 32S
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      invalid parameter has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  channel has not been initialized
 * \retval #IO_E_CH_CAPABILITY           the given channel does not support the desired functionality
 *
 **************************************************************************/
 IO_ErrorType IO_CAN_DeInit(IO_PIN channel);

/**************************************************************************
 * NORMAL CAN MESSAGES
 **************************************************************************/

/**********************************************************************//**
 *
 * \brief Configures a message object.
 *
 * Configures a message object for the given CAN channel and
 * returns a message object handle.
 *
 *
 * \param   handle         Returns the message object handle
 * \param   channel         CAN channel, one of:
 *                               - \c #IO_CAN_CHANNEL_0
 *                               - \c #IO_CAN_CHANNEL_1 \note Second CAN Interface (IO_CAN_CHANNEL_1) is only available for HY-TTC 32 and HY-TTC 32S
 * \param   mode           Mode for CAN Message, one of:
 *                          - \c #IO_CAN_MSG_READ,
 *                          - \c #IO_CAN_MSG_WRITE
 * \param   id_format      Format of message identifier, one of:
 *                          - \c #IO_CAN_STD_FRAME,
 *                          - \c #IO_CAN_EXT_FRAME
 * \param   id             CAN message identifier
 * \param   ac_mask        CAN acceptance mask, refer to \ref can_ac_mask_usage
 *                              "Usage of the acceptance mask" for further details.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_CAN_MAX_MO_REACHED      no more HW message objects are available
 * \retval #IO_E_CAN_MAX_HANDLES_REACHED no more handles are available
 * \retval #IO_E_NULL_POINTER            null Pointer has been passed
 * \retval #IO_E_INVALID_CHANNEL_ID      invalid Channel ID has been passed
 * \retval #IO_E_INVALID_PARAMETER       invalid Parameter has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel was not initialized
 * \retval #IO_E_CH_CAPABILITY           the given channel does not support the desired functionality
 *
 **************************************************************************/
 IO_ErrorType IO_CAN_ConfigMsg( ubyte1 * const handle
                              , IO_PIN channel
                              , ubyte1 mode
                              , ubyte1 id_format
                              , ubyte4 id
                              , ubyte4 ac_mask );

/**********************************************************************//**
 *
 * \brief Returns the data of a message object
 *
 * Reads a message from a given message object.
 * Returns whether the message is new or not.
 *
 *
 * \param   handle      CAN message object handle
 *                          (retrieved from \c IO_CAN_ConfigMsg())
 * \param   buffer      Pointer to data buffer structure.
 *                          The received frame will be stored there.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CAN_OVERFLOW           overflow of message object
 * \retval #IO_E_CAN_OLD_DATA           no new data has been received since the last read
 * \retval #IO_E_CAN_WRONG_HANDLE       invalid handle has been passed
 * \retval #IO_E_NULL_POINTER           null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given handle was not configured
 * \retval #IO_E_CAN_INVALID_DATA       the received data is invalid - read again to get the valid data
 *
 *
 **************************************************************************/
 IO_ErrorType IO_CAN_ReadMsg( ubyte1 handle
                            , IO_CAN_DATA_FRAME * const buffer );

/**********************************************************************//**
 *
 * \brief Transmits a CAN Message
 *
 * Transmits a CAN message, using the given channel and message object.
 * Returns whether the transmission has been started successfully or not.
 *
 *
 * \param   handle         CAN message object handle
 *                              (retrieved from \c IO_CAN_ConfigMsg())
 * \param   data           Pointer to data structure.
 *                              The data in this structure will be transmitted.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                       everything fine
 * \retval #IO_E_BUSY                     message object busy - no data has been transmitted
 * \retval #IO_E_CAN_WRONG_HANDLE         invalid handle has been passed
 * \retval #IO_E_NULL_POINTER             null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED   the given handle has not been configured
 *
 **************************************************************************/
 IO_ErrorType IO_CAN_WriteMsg( ubyte1 handle
                             , const IO_CAN_DATA_FRAME * const data );


/**************************************************************************
 * FIFO BUFFERED CAN MESSAGES
 *************************************************************************/

/**********************************************************************//**
 *
 * \brief Configures a FIFO buffer.
 *
 * Configures a FIFO buffer for the given CAN channel.
 *
 *
 * \param   handle         Returns the FIFO buffer handle
 * \param   channel         CAN channel, one of:
 *                               - \c #IO_CAN_CHANNEL_0
 *                               - \c #IO_CAN_CHANNEL_1 \note Second CAN Interface (IO_CAN_CHANNEL_1) is only available for HY-TTC 32 and HY-TTC 32S
 * \param   size           Size of FIFO buffer (number of frames).
 * \param   mode           Mode for CAN Message, one of:
 *                              - \c #IO_CAN_MSG_READ
 *                              - \c #IO_CAN_MSG_WRITE
 * \param   id_format      Format of message identifier, one of:
 *                              - \c #IO_CAN_STD_FRAME
 *                              - \c #IO_CAN_EXT_FRAME
 * \param   id             CAN message identifier
 * \param   ac_mask        CAN acceptance mask, refer to \ref can_ac_mask_usage
 *                              "Usage of the acceptance mask" for further details.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_CAN_MAX_MO_REACHED      no more HW message objects are available
 * \retval #IO_E_CAN_MAX_HANDLES_REACHED no more free handles are available
 * \retval #IO_E_NULL_POINTER            null pointer has been passed
 * \retval #IO_E_INVALID_CHANNEL_ID      invalid channel number has been passed
 * \retval #IO_E_INVALID_PARAMETER       a parameter is out of range
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  channel has not been initialized
 * \retval #IO_E_CH_CAPABILITY           the given channel does not support the desired functionality
 *
 **************************************************************************/
IO_ErrorType IO_CAN_ConfigFIFO( ubyte1 * const handle
                              , IO_PIN channel
                              , ubyte1 size
                              , ubyte1 mode
                              , ubyte1 id_format
                              , ubyte4 id
                              , ubyte4 ac_mask );

/**********************************************************************//**
 *
 * \brief Reads the data from a FIFO buffer.
 *
 * Copies received CAN frames from a given FIFO buffer to a SW frame buffer.
 *
 *
 * \param   handle         CAN FIFO buffer handle
 *                    (retrieved from \c IO_CAN_ConfigFIFO())
 * \param   buffer         Pointer to SW frame buffer structure.
 *                    The received frames will be stored there.
 * \param   buffer_size    Size of \c buffer, maximum number of frames to be
 *                    written to the buffer. If buffer_size is higher
 *                    than the actual size of the buffer, data outside
 *                    the buffer will be overwritten!
 * \param   rx_frames      Number of valid frames which have been copied to \c buffer
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_NULL_POINTER           null pointer has been passed to function
 * \retval #IO_E_CAN_WRONG_HANDLE       invalid handle has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given handler has not been configured
 * \retval #IO_E_CAN_OVERFLOW           overflow of FIFO buffer occurred. Data
 *                                      has been lost. In rare cases received
 *                                      data might be in a wrong order. Please note that
 *                                      the function doesn't check whole buffer for
 *                                      overflow. For whole buffer overflow check please call
 *                                      \c IO_CAN_FIFOStatus function
 * \retval #IO_E_CAN_OLD_DATA           no new data is available
 *
 **************************************************************************/
 IO_ErrorType IO_CAN_ReadFIFO( ubyte1 handle
                             , IO_CAN_DATA_FRAME * const buffer
                             , ubyte1 buffer_size
                             , ubyte1 * const rx_frames );

/**********************************************************************//**
 *
 * \brief Writes CAN frames to a FIFO buffer.
 *
 * Copies CAN Frames from a SW frame buffer to a transmit FIFO buffer.
 * The CAN transmission will be started.
 *
 *
 * \param   handle         CAN message object handle
 *                    (retrieved from \c IO_CAN_ConfigFIFO())
 * \param   data           Pointer to data structure.
 *                    The data in this structure will be transmitted.
 * \param   tx_length      Number of frames in \c data which shall be transmitted.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                everything fine
 * \retval #IO_E_CAN_FIFO_FULL     FIFO is full - no data has been copied
 *                                 into FIFO
 * \retval #IO_E_CAN_WRONG_HANDLE  invalid handle has been passed
 * \retval #IO_E_NULL_POINTER      null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel has not been configured
 *
 **************************************************************************/
 IO_ErrorType IO_CAN_WriteFIFO( ubyte1 handle
                              , const IO_CAN_DATA_FRAME * const data
                              , ubyte1 tx_length );

/**************************************************************************
 * AUXILIARY FUNCTIONS
 **************************************************************************/

/**********************************************************************//**
 *
 * \brief Returns the error counters of the CAN channel.
 *
 * Returns the transmit and receive error counters of the selected CAN channel.
 *
 *
 * \param   channel           CAN channel, one of:
 *                               - \c #IO_CAN_CHANNEL_0
 *                               - \c #IO_CAN_CHANNEL_1 \note Second CAN Interface (IO_CAN_CHANNEL_1) is only available for HY-TTC 32 and HY-TTC 32S
 * \param   rx_error_counter  Value of the receive error counter
 * \param   tx_error_counter  Value of the transmit error counter
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CAN_ERROR_PASSIVE      controller is in error passive state
 * \retval #IO_E_CAN_BUS_OFF            controller is in bus off state
 * \retval #IO_E_INVALID_CHANNEL_ID     wrong channel number has been passed
 * \retval #IO_E_NULL_POINTER           null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel has not been initialized
 * \retval #IO_E_CH_CAPABILITY          the given channel does not support the desired functionality
 *
 **************************************************************************/
 IO_ErrorType IO_CAN_Status( IO_PIN channel
                           , ubyte1 * const rx_error_counter
                           , ubyte1 * const tx_error_counter );

/**********************************************************************//**
 *
 * \brief Returns the status of a message buffer object.
 *
 * If a CAN node is in bus-off state, the bus-off recovery sequence detection
 * is started automatically. By default the CAN node is not allowed to
 * continue bus communication once the bus-off recovery sequence
 * has finished. It is necessary to call #IO_CAN_MsgStatus() or
 * #IO_CAN_FIFOStatus() respectively to enable further bus communication.
 *
 * \param   handle         CAN message object handle
 *                          (retrieved from \c IO_CAN_ConfigMsg())
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                everything fine
 * \retval #IO_E_BUSY              transmission is ongoing
 * \retval #IO_E_CAN_OVERFLOW      message object overflow
 * \retval #IO_E_CAN_OLD_DATA      no new data received
 * \retval #IO_E_CAN_WRONG_HANDLE  invalid handle has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given handle has not been configured
 *
 **************************************************************************
 *
 * \remarks
 *   - Calling this function also starts the Bus Off recovery sequence
 *   if the respective CAN node is in Bus Off.
 *
 **************************************************************************/
 IO_ErrorType IO_CAN_MsgStatus( ubyte1 handle );

/**********************************************************************//**
 *
 * \brief Returns the status of a FIFO buffer.
 *
 * If a CAN node is in bus-off state, the bus-off recovery sequence detection
 * is started automatically. By default the CAN node is not allowed to
 * continue bus communication once the bus-off recovery sequence
 * has finished. It is necessary to call #IO_CAN_MsgStatus() or
 * #IO_CAN_FIFOStatus() respectively to enable further bus communication.
 *
 * \param   handle         CAN message object handle
 *                          (retrieved from \c IO_CAN_ConfigFIFO())
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                everything fine
 * \retval #IO_E_BUSY              transmission is ongoing. Only reported for
 *                                 write handle.
 * \retval #IO_E_CAN_OVERFLOW      Overflow of FIFO buffer occurred. Data
 *                                 has been lost. Only reported for read handle.
 * \retval #IO_E_CAN_OLD_DATA      no new data has been received. Only reported
 *                                 for read handle.
 * \retval #IO_E_CAN_WRONG_HANDLE  invalid handle has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given handle has not been configured
 *
 **************************************************************************
 *
 * \remarks
 *   - Calling this function also starts the Bus Off recovery sequence
 *   if the respective CAN node is in Bus Off.
 *
 **************************************************************************/
 IO_ErrorType IO_CAN_FIFOStatus( ubyte1 handle );

#endif /* _IO_CAN_H */
