#define _CAN_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                     MODULES USED                                       **/
/**                                                                        **/
/****************************************************************************/

#include "CanHandler.h"
#include "IO_CAN.h"
#include "IO_RTC.h"
/****************************************************************************/
/**                                                                        **/
/**                     DEFINITIONS AND MACROS                             **/
/**                                                                        **/
/****************************************************************************/

#define MAX_RX_BUFSIZE 48 // max of HwRxBufferSize (see below)
#define MAX_TX_BUFSIZE 16 // max of HwTxBufferSize (see below)

/****************************************************************************/
/**                                                                        **/
/**                     TYPEDEFS AND STRUCTURES                            **/
/**                                                                        **/
/****************************************************************************/

typedef struct {
  CanFrame_t frame[CAN_TX_BUFSIZE];
  UI08_t     wPtr;
  UI08_t     rPtr;
} CanTxBuf_t;

typedef struct {
  Bool_t          used;
  UI08_t          channelId;
  Bool_t          eWarn;
  Bool_t          dataOverrun;
  Bool_t          errorPassive;
  Bool_t          arbitrationLost;
  Bool_t          busError;
  Bool_t          busOff;
  Bool_t          txReady;
  Bool_t          txError;
  UI08_t          txErrorCount;
  UI08_t          rxErrorCount;
  CanHandler_t    rxHandlerPtr;
  UI16_t          hRxFifo;
  UI16_t          hTxFifo[MAX_TX_BUFSIZE];
  UI08_t          lowestTxMailBox;
  CanTxBuf_t      txBuf;
} CanState_t;

/****************************************************************************/
/**                                                                        **/
/**                     PROTOTYPES OF LOCAL FUNCTIONS                      **/
/**                                                                        **/
/****************************************************************************/

static void   dummyRxHandler     (UI08_t bCanNum, CanFrame_t *pFrame);
static void   setNodeStatusFlags (UI08_t bCanNum);
static Bool_t sendFrame          (UI08_t bCanNum, CanFrame_t *pFrame);
static void   checkTxBuf         (UI08_t bCanNum);

/****************************************************************************/
/**                                                                        **/
/**                     EXPORTED VARIABLES                                 **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                     GLOBAL VARIABLES                                   **/
/**                                                                        **/
/****************************************************************************/

// Hardware capabilities (note: channels 0,1 and 2 are COMBINED max. 64)
static const UI08_t HwRxBufferSize [] = {48, 48, 48, 24, 24, 24, 24};
static const UI08_t HwTxBufferSize [] = {16, 16, 16, 8, 8, 8, 8};

static CanState_t CanState[NR_OF_CAN];

/****************************************************************************/
/**                                                                        **/
/**                     EXPORTED FUNCTIONS                                 **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
#ifdef USE_PARAM_FOR_EXTENDED_CAN
Bool_t CanInit(UI08_t bCanNum, UI32_t dwBaudRate, Bool_t bExtended)
#else
Bool_t CanInit(UI08_t bCanNum, UI32_t dwBaudRate)
#endif
/****************************************************************************/
{
  if (bCanNum >= NR_OF_CAN) return FALSE;

  if(CanState[bCanNum].used) {
#ifdef TRACE_SERIAL
	  sprintf(TraceBuf, "CAN[%hu] Init failed, CAN ID already in use\r\n", bCanNum); Trace();
#endif
	  return FALSE;
  }

  // Initialize CanState
  CanState[bCanNum].used            = FALSE;
  CanState[bCanNum].txBuf.wPtr      = 0;
  CanState[bCanNum].txBuf.rPtr      = 0;
  CanState[bCanNum].txReady         = FALSE;
  CanState[bCanNum].txError         = FALSE;
  CanState[bCanNum].dataOverrun     = FALSE;
  CanState[bCanNum].errorPassive    = FALSE;
  CanState[bCanNum].arbitrationLost = FALSE;
  CanState[bCanNum].busError        = FALSE;
  CanState[bCanNum].eWarn           = FALSE;
  CanState[bCanNum].busOff          = FALSE;
  ubyte1 baudRateId;
  if      (dwBaudRate == 125000) baudRateId = IO_CAN_BAUDRATE_125K;
  else if (dwBaudRate == 250000) baudRateId = IO_CAN_BAUDRATE_250K;
  else if (dwBaudRate == 500000) baudRateId = IO_CAN_BAUDRATE_500K;
  else return FALSE;

  // Set channel number
  CanState[bCanNum].channelId += IO_CAN_CHANNEL_0+bCanNum;

  IO_ErrorType ret = IO_CAN_Init(CanState[bCanNum].channelId, baudRateId);
  if (ret != IO_E_OK) {
#ifdef TRACE_SERIAL
    sprintf(TraceBuf, "CAN[%hu] Init failed: %hu\r\n", bCanNum, ret); Trace();
#endif
    return FALSE;
  }

  // Set up hardware rx fifo
  UI08_t id_format = IO_CAN_STD_FRAME;
  if (bExtended) id_format = IO_CAN_EXT_FRAME;
  ret = IO_CAN_ConfigFIFO( &CanState[bCanNum].hRxFifo , CanState[bCanNum].channelId, HwRxBufferSize[bCanNum], IO_CAN_MSG_READ , id_format, 0, 0);
  if (ret != IO_E_OK) {
#ifdef TRACE_SERIAL
    sprintf(TraceBuf, "CAN[%hu] ConfigFIFO failed (tried %hu) ret: %hu\r\n", bCanNum, HwRxBufferSize[bCanNum], ret); Trace();
#endif
    return FALSE;
  }

  // Set up hardware tx fifo
  for (UI08_t i = 0; ((i < HwTxBufferSize[bCanNum]) && (ret == IO_E_OK)); i++) {
    ret = IO_CAN_ConfigMsg(&CanState[bCanNum].hTxFifo[i], CanState[bCanNum].channelId, IO_CAN_MSG_WRITE, id_format, 0, 0);
  }
  //sprintf(TraceBuf, "CAN[%u]-->No. of mailboxes: %u\r\n", bCanNum, i); Trace();

  CanState[bCanNum].lowestTxMailBox = 0;
  CanState[bCanNum].used = TRUE;

  return TRUE;
}


/****************************************************************************/
void CanTick(void) // ToDo: can this be converted to CanGetRxBufFrame or something?
/****************************************************************************/
{
	IO_ErrorType      ret;
	IO_CAN_DATA_FRAME ttcFrame[MAX_RX_BUFSIZE];
	UI08_t            size = 1;
	CanFrame_t        frame;

	for (UI08_t c = 0; c < NR_OF_CAN; ++c) {
	  if (CanState[c].used) {
	    // Do receive from hardware
	    if (IO_CAN_FIFOStatus(CanState[c].hRxFifo) != IO_E_CAN_OLD_DATA) {
	        ret = IO_CAN_ReadFIFO(CanState[c].hRxFifo, ttcFrame, MAX_RX_BUFSIZE, &size);
	        if (!(ret == IO_E_OK || ret == IO_E_CAN_OLD_DATA)) {
#ifdef TRACE_SERIAL
	          sprintf(TraceBuf, "CAN[%hu] RxFIFOError %hu\r\n", c, ret); Trace();
#endif
	        }
	        else {
	          for (UI08_t i = 0; i < size; ++i) {
	            frame.id       = ttcFrame[i].id;
	            frame.length      = ttcFrame[i].length;
	            frame.extended = ttcFrame[i].id_format;
	            for (UI08_t j = 0; j < ttcFrame[i].length; ++j) frame.data[j] = ttcFrame[i].data[j];
	            (*CanState[c].rxHandlerPtr)(c, &frame);
	          }
	        }
	    }
	    // Do transmit and set status
	    checkTxBuf(c);
	    setNodeStatusFlags(c);
	  }
	}
}

/****************************************************************************/
void CanSetRxHandler (UI08_t bCanNum, CanHandler_t handlerPtr)
/****************************************************************************/
{
  CanState[bCanNum].rxHandlerPtr = handlerPtr;
}

/****************************************************************************/
void CanResetRxHandler (UI08_t bCanNum)
/****************************************************************************/
{
  CanState[bCanNum].rxHandlerPtr = dummyRxHandler;
}

/****************************************************************************/
Bool_t CanBusOff (UI08_t bCanNum)
/****************************************************************************/
{
  setNodeStatusFlags (bCanNum);

  return (CanState[bCanNum].busOff);
}

/****************************************************************************/
Bool_t CanError (UI08_t bCanNum)
/****************************************************************************/
{
  setNodeStatusFlags (bCanNum);

  return (CanState[bCanNum].eWarn);
}

/****************************************************************************/
UI16_t CanGetErrorCount (UI08_t bCanNum)
/****************************************************************************/
{
  UI16_t errorCount = 0;

  setNodeStatusFlags (bCanNum);

  errorCount += CanState[bCanNum].rxErrorCount;
  errorCount += CanState[bCanNum].txErrorCount;

  return errorCount;
}

/****************************************************************************/
Bool_t CanTxReady (UI08_t bCanNum)
/****************************************************************************/
{
  return (CanState[bCanNum].txReady);
}

/****************************************************************************/
Bool_t CanBusError (UI08_t bCanNum)
/****************************************************************************/
{
  setNodeStatusFlags (bCanNum);
  return (CanState[bCanNum].busError);
}

/****************************************************************************/
Bool_t CanDataOverrun (UI08_t bCanNum)
/****************************************************************************/
{
  setNodeStatusFlags (bCanNum);
  return (CanState[bCanNum].dataOverrun);
}

/****************************************************************************/
Bool_t CanErrorPassive (UI08_t bCanNum)
/****************************************************************************/
{
  setNodeStatusFlags (bCanNum);
  return (CanState[bCanNum].errorPassive);
}

/****************************************************************************/
Bool_t CanArbitrationLost (UI08_t bCanNum)
/****************************************************************************/
{
  setNodeStatusFlags (bCanNum);
  return (CanState[bCanNum].arbitrationLost);
}

/****************************************************************************/
Bool_t CanTxError (UI08_t bCanNum)
/****************************************************************************/
{
  setNodeStatusFlags (bCanNum);
 
  return (CanState[bCanNum].txError);
}

/****************************************************************************/
void CanTxFrame (UI08_t bCanNum, CanFrame_t *pFrame)
{
  // Buffer the frame in software
  memcpy(&CanState[bCanNum].txBuf.frame[CanState[bCanNum].txBuf.wPtr], pFrame, sizeof(CanFrame_t));
  if (CanState[bCanNum].txBuf.wPtr < (CAN_TX_BUFSIZE-1)) CanState[bCanNum].txBuf.wPtr++;
  else CanState[bCanNum].txBuf.wPtr = 0;

  // Try to transmit immediately via hardware
  checkTxBuf(bCanNum);
}

/****************************************************************************/
Bool_t CanCheckTxBuf (UI08_t bCanNum)
/****************************************************************************/
{
  // Check if there are frames in the transmit buffer
  return CanState[bCanNum].txReady ? FALSE : TRUE;
}

/****************************************************************************/
void CanFlushTxBuf (UI08_t bCanNum)
/****************************************************************************/
{
  CanState[bCanNum].txBuf.wPtr = 0;
  CanState[bCanNum].txBuf.rPtr = 0;
}

/****************************************************************************/
/**                                                                        **/
/**                     LOCAL FUNCTIONS                                    **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
static void dummyRxHandler (UI08_t bCanNum, CanFrame_t *pFrame)
/****************************************************************************/
{
  // Empty handler, do nothing
  REFER(bCanNum);
  REFER(pFrame);
}

/****************************************************************************/
static void setNodeStatusFlags(UI08_t bCanNum)
/****************************************************************************/
{
	IO_ErrorType ret = IO_CAN_Status(CanState[bCanNum].channelId, &CanState[bCanNum].rxErrorCount, &CanState[bCanNum].txErrorCount);
	switch (ret) {
		case IO_E_OK:
			CanState[bCanNum].errorPassive = FALSE;
			CanState[bCanNum].eWarn = FALSE;
			CanState[bCanNum].busOff = FALSE;
			break;
		case IO_E_CAN_ERROR_PASSIVE:
			CanState[bCanNum].errorPassive = TRUE;
			break;
		case IO_E_CAN_BUS_OFF:
			CanState[bCanNum].busOff = TRUE;
			break;
	}
}

static void checkTxBuf (UI08_t bCanNum)
{
  CanFrame_t frame;

  // Check if there are new frames in software transmit buffer
  while (CanState[bCanNum].txBuf.rPtr != CanState[bCanNum].txBuf.wPtr) {
    // New frames in software buffer, get frame
    memcpy(&frame, &(CanState[bCanNum].txBuf.frame[CanState[bCanNum].txBuf.rPtr]), sizeof(CanFrame_t));
    // try to transmit this frame via hardware
    if (sendFrame(bCanNum, &frame)) {
      // if succeeded (hardware fifo not full) shift software fifo
      if (CanState[bCanNum].txBuf.rPtr < (CAN_TX_BUFSIZE-1)) CanState[bCanNum].txBuf.rPtr++;
      else CanState[bCanNum].txBuf.rPtr = 0;
    }
    else break;
  }
}

/****************************************************************************/
Bool_t sendFrame (UI08_t bCanNum, CanFrame_t *pFrame)
/****************************************************************************/
{
  UI08_t i = 0;

  CanState[bCanNum].txReady = FALSE;

  // Set lowest unused mailbox to 0 if the whole queue was sent
  if (CanState[bCanNum].lowestTxMailBox != 0) {
    if ((IO_CAN_MsgStatus(CanState[bCanNum].hTxFifo[CanState[bCanNum].lowestTxMailBox - 1]) == IO_E_OK)) {
      CanState[bCanNum].lowestTxMailBox = 0;
    }
  }

  // Get a free mailbox for TX
  for (i = CanState[bCanNum].lowestTxMailBox; i < HwTxBufferSize[bCanNum]; i++) {
    // Check for pending TX message in mailbox
    if (IO_CAN_MsgStatus(CanState[bCanNum].hTxFifo[i]) == IO_E_OK) break;
  }

  IO_ErrorType ret = IO_E_OK;
  // All mailboxes in use
  if (i >= HwTxBufferSize[bCanNum]) ret = IO_E_BUSY;

  // Check, if a free mailbox has been found
  if (ret == IO_E_OK) {
    IO_CAN_DATA_FRAME ttcFrame;

    // Send the given CAN message using the mailbox from above
    ttcFrame.id = pFrame->id;
    ttcFrame.id_format = pFrame->extended;
    ttcFrame.length = pFrame->length;

    for (UI08_t j = 0; j < ttcFrame.length; j++) ttcFrame.data[j] = pFrame->data[j];
    ret = IO_CAN_WriteMsg(CanState[bCanNum].hTxFifo[i], &ttcFrame);
    if (ret == IO_E_OK) {
      // Increment lowest used mailbox
      CanState[bCanNum].lowestTxMailBox++;
      CanState[bCanNum].txReady = TRUE;
      return TRUE;
    }
  }
  return FALSE;
}


/****************************************************************************/
/**                                                                        **/
/**                              EOF                                       **/
/**                                                                        **/
/****************************************************************************/
