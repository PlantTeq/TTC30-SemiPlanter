/**
 * \file TypesGen.h
 *
 * \brief Types header file.
 *
 * Copyright (c) TTControl. All rights reserved. Confidential and proprietary.
 *
 */
#ifndef TYPES_H
#define TYPES_H

/*************************************************************************************************
 *
 * I N C L U D E S
 *
 *************************************************************************************************/
#if defined(__C166__)
    #include "ptypes_xe167.h"
#elif defined(__TC161__) || \
      defined(__TC162__)
    #include "TtPtypes.h"
#else
    #error "Invalid CPU"
#endif

/*************************************************************************************************
 *
 * D E F I N I T I O N S
 *
 *************************************************************************************************/
/**
 * \brief Compiler instruction to create full-sized pointers
 *
 * On some CPUs it is possible to select a memory model that creates pointers with a reduced size
 * (e.g. CPUs where the bit width of the ALU is smaller than the bit-width of the address bus).
 * For these memory models the below definition is important to create full-sized pointers to
 * be able to address memory objects that are greater than the range of the reduced size pointer.
 *
 * For environments where those memory models that operate with reduced size pointers do not exist
 * or are not used the definition below can be empty
 */
#if defined(__C166__)
    #define HUGE        __huge
    #define TT_NULL     NULL
    #define TT_TRUE     TRUE
    #define TT_FALSE    FALSE

    typedef bool        boolean;
    typedef ubyte1      uint8;
    typedef ubyte2      uint16;
    typedef ubyte4      uint32;
    typedef ubyte8      uint64;
#elif defined(__TC161__) || \
      defined(__TC162__)
    #define HUGE
#else
    #error "Invalid CPU"
#endif

#define UINT32_ALL_BITS_SET     (0xFFFFFFFFlu)
#define UINT16_ALL_BITS_SET     (0xFFFFu)
#define UINT8_ALL_BITS_SET      (0xFFu)

#define BL_AUTH_SEED			(0x39EB0919lu)	/* must match Downloader.tBLDB.SECRET_SEED */
#define BL_AUTH_KEY				(0xF12C13C9lu)  /* must match Downloader.tBLDB.SECRET_KEY */

/*************************************************************************************************
 *
 * D A T A   S T R U C T U R E S
 *
 *************************************************************************************************/
/**
 * \brief CAN ID structure
 */
typedef struct can_id
{
    uint32 Extended;                /**< Type of CAN identifier to be used. Valid values are:
                                            0 ... standard CAN identifier is used
                                            1 ... extended CAN identifier is used                */
    uint32 Id;                      /**< The CAN identifier (LSB must start at bit 0):
                                            bit 0-10 ... if standard CAN identifier is used
                                            bit 0-28 ... if extended CAN identifier is used      */
} CanIdType;

#endif /* TYPES_H */

/* EOF */
