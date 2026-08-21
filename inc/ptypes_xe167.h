/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file ptypes_xe167.h
 *
 * \brief Primitive data types
 *
 *      This file defines the primitive data types used for the IO Driver
 *
 **************************************************************************/
/**************************************************************************
 *
 * PRQA Justification:
 * - 1027: long long is a language extension.
 *         MISRA-C:2004 Rule 1.
 *         -> 64 bit variables can only be defined this way
 *         -> this file needs to be modified when the product is ported
 *            to a different platform
 *
 *************************************************************************/

#ifndef _PTYPES_H_XE167
#define _PTYPES_H_XE167 1

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char       ubyte1; /**< unsigned, length: one byte (8bit)     */
typedef unsigned int        ubyte2; /**< unsigned, length: two bytes (16bit)   */
typedef unsigned long       ubyte4; /**< unsigned, length: four bytes (32bit)  */
typedef unsigned long long  ubyte8; /**< unsigned, length: eight bytes (64bit) */ /* PRQA S 1027 */

typedef signed char         sbyte1; /**< signed, length: one byte (8bit)       */
typedef signed int          sbyte2; /**< signed, length: two bytes (16bit)     */
typedef signed long         sbyte4; /**< signed, length: four bytes (32bit)    */
typedef signed long long    sbyte8; /**< signed, length: eight bytes (64bit)   *//* PRQA S 1027 */

typedef float               float4; /**< floating point, four bytes (32bit)   */

typedef unsigned char       bool;   /**< boolean type, should only be set to 0 (FALSE) or 1 (TRUE) */

#ifndef FALSE
    #define FALSE             ((bool)0) /**< FALSE value for boolean type */
#endif
#ifndef TRUE
    #define TRUE              (!FALSE)  /**< TRUE value for boolean type */
#endif

#ifndef NULL
    #define NULL              (0)   /**< NULL value, e.g. for invalid pointers */
#endif

#define UBYTE8_ALL_BITS_SET 0xFFFFFFFFFFFFFFFFULL
#define UBYTE4_ALL_BITS_SET 0xFFFFFFFFUL
#define UBYTE2_ALL_BITS_SET 0xFFFFU
#define UBYTE1_ALL_BITS_SET 0xFFU

#define SBYTE8_ALL_BITS_SET ((sbyte8)-1)
#define SBYTE4_ALL_BITS_SET ((sbyte4)-1)
#define SBYTE2_ALL_BITS_SET ((sbyte2)-1)
#define SBYTE1_ALL_BITS_SET ((sbyte1)-1)

#ifdef  __cplusplus
   }
#endif /* __cplusplus */

#endif /* _PTYPES_H_XE167 */
