/*
This module provides support for the CAN buses on the LPC2129,
it uses the standard IAR ew_can modules.
*/

#ifndef _CANHANDLER_INCLUDED
#define _CANHANDLER_INCLUDED

/****************************************************************************/
/**                                                                        **/
/**                     MODULES USED                                       **/
/**                                                                        **/
/****************************************************************************/

#include "sysconf.h"

/****************************************************************************/
/**                                                                        **/
/**                     DEFINITIONS AND MACROS                             **/
/**                                                                        **/
/****************************************************************************/

#define CAN_FRAME_SIZE   15  // 4 byte ID, 1 byte DLC, 8 byte data,
                             // 1 byte priority, 1 byte extended

/****************************************************************************/
/**                                                                        **/
/**                     TYPEDEFS AND STRUCTURES                            **/
/**                                                                        **/
/****************************************************************************/

typedef struct {
  UI32_t id;         // 29 bit identifier, lsb send last
  UI08_t length;        // data length code
  UI08_t data[8];   // 8 byte data field
  UI08_t priority;   // priority (0..7)
  UI08_t extended;   // extended(29bit) = 1, standard(11bit) = 0
} CanFrame_t;

typedef struct {
  UI08_t priority;
  UI32_t pgn;
  UI32_t size;
  UI08_t *pBytes;
} CanMsg_t;

// CanHandler function prototype pointer
typedef void (*CanHandler_t) (UI08_t bCanNum, CanFrame_t *pFrame);

/****************************************************************************/
/**                                                                        **/
/**                     EXPORTED VARIABLES                                 **/
/**                                                                        **/
/****************************************************************************/

#ifndef _CAN_C_SRC
#endif

/****************************************************************************/
/**                                                                        **/
/**                     EXPORTED FUNCTIONS                                 **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
#ifdef USE_PARAM_FOR_EXTENDED_CAN
Bool_t CanInit(UI08_t bCanNum, UI32_t dwBaudRate, Bool_t bExtended);
#else
Bool_t CanInit(UI08_t bCanNum, UI32_t dwBaudRate);
#endif
/****************************************************************************/
/*
 * Initializes this module. Call this routine before calling any other
 * routine of this module.
 *
* Fixed initialisation: CAN1 and CAN2
 */

#ifndef CAN_ENABLE_RX_FRAME_BUFFERING
/****************************************************************************/
void CanSetRxHandler (UI08_t bCanNum, CanHandler_t handlerPtr);
/****************************************************************************/
/*
 * The RxHandler will be called if a receive interrupt occurs and interrupts
 * are enabled. This function is used to set a receive handler according to
 * the definition of 'canHandlerPtr'
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 * - handlerPtr : function pointer
 */

/****************************************************************************/
void CanResetRxHandler (UI08_t bCanNum);
/****************************************************************************/
/*
 * Sets the RxHandler to an internal dummy routine.
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 */
#endif

/****************************************************************************/
void CanSetTxHandler (UI08_t bCanNum, CanHandler_t handlerPtr);
/****************************************************************************/
/*
 * The TxHandler will be called if a transmit interrupt occurs and interrupts
 * are enabled.
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 * - handlerPtr : function pointer
 */

/****************************************************************************/
void CanResetTxHandler (UI08_t bCanNum);
/****************************************************************************/
/*
 * Sets the TxHandler to an internal dummy routine.
 *
 * Arguments:
 * - canNum: zero based can controller number to address
 */


/****************************************************************************/
Bool_t CanBusOff (UI08_t bCanNum);
/****************************************************************************/
/*
 * Returns TRUE if the CAN driver is in the bus-off state. This function can
 * be used to test if a transmission was not aborted by the bus-off condition.
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 */

/****************************************************************************/
Bool_t CanError (UI08_t bCanNum);
/****************************************************************************/
/*
 * Returns TRUE if the CAN driver was in an error state.
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 */

/****************************************************************************/
UI16_t CanGetErrorCount (UI08_t bCanNum);
/****************************************************************************/
/*
 * Returns the current ErrorCount value of CAN controller.
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 */

/****************************************************************************/
Bool_t CanTxReady (UI08_t bCanNum);
/****************************************************************************/
/*
 * This function returns TRUE if the CAN controller is ready to transmit a
 * frame. A frame can be transmitted using CanTxMessage. This function
 * can be used to check if a previous transmission was finished.
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 */

/****************************************************************************/
Bool_t CanTxError (UI08_t bCanNum);
/****************************************************************************/
/*
 * Returns TRUE if the last message was not send correctly.
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 */

/****************************************************************************/
Bool_t CanBusError (UI08_t bCanNum);
/****************************************************************************/

/****************************************************************************/
Bool_t CanDataOverrun (UI08_t bCanNum);
/****************************************************************************/

/****************************************************************************/
Bool_t CanErrorPassive (UI08_t bCanNum);
/****************************************************************************/

/****************************************************************************/
Bool_t CanArbitrationLost (UI08_t bCanNum);
/****************************************************************************/

/****************************************************************************/
void CanTxFrame (UI08_t bCanNum, CanFrame_t *pFrame);
/****************************************************************************/
/*
 * Puts a new frame in the transmit buffer. With callback
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 * - pFrame: pointer to a frame to be transmitted
 */

/****************************************************************************/
void CanTxFrameNoCallback (UI08_t bCanNum, CanFrame_t *pFrame);
/****************************************************************************/
/*
 * Puts a new frame in the transmit buffer. without callback
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 * - pFrame: pointer to a frame to be transmitted
 */

/****************************************************************************/
Bool_t CanCheckTxBuf (UI08_t bCanNum);
/****************************************************************************/
/*
 * Checks whether the transmit buffer contains frames to be send. If a new
 * frame is available in the transmit buffer it will be send.
 * This function can be called from the TxHandler, from a tick handler or
 * directly after CanTxFrame.
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 */

#ifdef CAN_ENABLE_RX_FRAME_BUFFERING
/****************************************************************************/
Bool_t CanCheckRxBuf (UI08_t bCanNum);
/****************************************************************************/

/****************************************************************************/
Bool_t CanGetRxBufFrame (UI08_t bCanNum, CanFrame_t *Frame);
/****************************************************************************/
#endif

/****************************************************************************/
void CanFlushTxBuf (UI08_t bCanNum);
/****************************************************************************/
/*
 * Empties the transmit buffer to immediately stop sending of frames.
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 */

/****************************************************************************/
void CanFlushRxBuf (UI08_t bCanNum);
/****************************************************************************/
/*
 * Empties the receive buffer.
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 */

#ifdef CAN_TX_DIRECT
/****************************************************************************/
Bool_t CanTxFrameDirect (UI08_t bCanNum, CanFrame_t *pFrame);
/****************************************************************************/
/*
 * Transmits the message directly. This function does not use frame buffering
 * and writes the frame directly to the CAN controller for transmission.
 *
 * Arguments:
 * - bCanNum: zero based can controller number to address
 */
#endif

#endif

/****************************************************************************/
void CanTick();
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                              EOF                                       **/
/**                                                                        **/
/****************************************************************************/

