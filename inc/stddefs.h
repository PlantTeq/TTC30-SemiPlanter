/*
                             *******************
******************************* C HEADER FILE *******************************
**                           *******************                           **
**                                                                         **
** project   : General modules                                             **
** filename  : STDDEFS.H                                                   **
** version   : 1.1                                                         **
** date      : May 06, 2014                                                **
**                                                                         **
*****************************************************************************
**       Property of PRECISION MAKERS B.V.      All rights reserved                **
*****************************************************************************

VERSION HISTORY:
----------------

Version     : 1.1
Date        : May 06, 2014
Revised by  : Ard Nieuwenhuizen
Description : Added Endianswap macros to convert bigendian to littleendian
              and vice versa

Version     : 1
Date        : June 24, 2010
Revised by  : Vincent Achten
Description : * Original version.

*/

#ifndef _STDDEFS_INCLUDED
#define _STDDEFS_INCLUDED

/****************************************************************************/
/**                                                                        **/
/**                     MODULES USED                                       **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                     TYPEDEFS AND STRUCTURES                            **/
/**                                                                        **/
/****************************************************************************/

/**** C type extensions ****/
typedef unsigned char   Bool_t;
typedef signed   char   Char_t;

typedef signed char     I08_t;
typedef signed short    I16_t;
typedef signed long     I32_t;

typedef unsigned char   UI08_t;
typedef unsigned short  UI16_t;
typedef unsigned long   UI32_t;

typedef float           F32_t;
typedef double          F64_t;

typedef void *          Pointer_t;
typedef char *          String_t;
typedef void            (*HandlerPtr_t) (void);
typedef void            (*HandlerPtr_argb_t) (UI08_t arg);
typedef void            (*HandlerPtr_arg2b_t) (UI08_t arg1, UI08_t arg2);


/****************************************************************************/
/**                                                                        **/
/**                     DEFINITIONS AND MACROS                             **/
/**                                                                        **/
/****************************************************************************/

/**** Common boolean definitions ****/
#ifndef FALSE
    #define FALSE           0
#endif
#ifndef TRUE
    #define TRUE           1
#endif


/**** Common definition OK ****/
#define OK              0

/**** Common math definitions ****/
//#define PI              ((float) 3.141592654)
#define PI             3.1415926535897932384626433832795f
#define RAD2DEG        57.295779513082320876798154814105f   // 180/PI
#define DEG2RAD        0.017453292519943295769236907684886f // PI/180


/**** Bit macros ****/
#define SET_BIT(x, bitNr)       ((x) = (x) |  (0x01 << (bitNr)))
#define RESET_BIT(x, bitNr)     ((x) = (x) & ~(0x01 << (bitNr)))
#define GET_BYTE_BIT(x, bitNr)  ((((byte) (x) & \
                                   (byte) (0x01 << (bitNr)))) != 0)
#define GET_WORD_BIT(x, bitNr)  ((((word) (x) & \
                                   (word) (0x01 << (bitNr)))) != 0)

/**** Bitc macros ****/
#define SET_BITC(x, bitcNr, c)  ((x) = ((x) & ~(0x03 << (bitcNr<<1))) | (c << (bitcNr<<1)))
#define RESET_BITC(x, bitcNr)   ((x) = (x) & ~(0x03 << (bitcNr<<1)))
#define GET_BITC(x, bitcNr)     (( (x) & (0x03 << (bitcNr<<1)) ) >> (bitcNr<<1) )

/**** Hi/Lo byte and word macros ****/
#define HiByte(x)       *(((UI08_t *) &x) + 1)
#define LoByte(x)       *(((UI08_t *) &x) + 0)
#define HiWord(x)       *(((UI16_t *) &x) + 1)
#define LoWord(x)       *(((UI16_t *) &x) + 0)

/**** Extended macros for interrupt enable/disable ****/
//#define _enable()       __EI()
//#define _disable()      __DI()
//#define _enabled()      i_enabled()

/**** Macro can be used to prevent compiler warnings ****/
#define REFER(x)        if (x)

/****************************************************************************/
/**** Macro to byte swap 16, 32, 64 bits unsigned int to and from        ****/
/**** Big endian or little endian and reverse                            ****/

#define EndianSwap16(value) \
        (((UI16_t)((value) & 0x00FF)) << 8) | \
        (((UI16_t)((value) & 0xFF00)) >> 8)

#define EndianSwap32(value) \
        (((UI32_t)((value) & 0x000000FF)) << 24) | \
        (((UI32_t)((value) & 0x0000FF00)) << 8)  | \
        (((UI32_t)((value) & 0x00FF0000)) >> 8)  | \
        (((UI32_t)((value) & 0xFF000000)) >> 24)

inline F32_t EndianSwapF32( const F32_t inFloat )
{
   F32_t retVal = 0;
   char *floatToConvert = ( char* ) & inFloat;
   char *returnFloat = ( char* ) & retVal;
   /*swap the bytes into a temporary buffer*/
   returnFloat[0] = floatToConvert[3];
   returnFloat[1] = floatToConvert[2];
   returnFloat[2] = floatToConvert[1];
   returnFloat[3] = floatToConvert[0];
   return retVal;
}

inline UI32_t EndianSwapUI32_t( const UI32_t inFloat )
{
   UI32_t retVal = 0;
   char *floatToConvert = ( char* ) & inFloat;
   char *returnFloat = ( char* ) & retVal;
   // swap the bytes into a temporary buffer
   returnFloat[0] = floatToConvert[3];
   returnFloat[1] = floatToConvert[2];
   returnFloat[2] = floatToConvert[1];
   returnFloat[3] = floatToConvert[0];
   return retVal;
}

#define EndianSwap64(value) \
        ((()((value) & 0x00000000000000FFULL)) << 56) | \
        (((ubyte8)((value) & 0x000000000000FF00ULL)) << 40) | \
        (((ubyte8)((value) & 0x0000000000FF0000ULL)) << 24) | \
        (((ubyte8)((value) & 0x00000000FF000000ULL)) << 8)  | \
        (((ubyte8)((value) & 0x000000FF00000000ULL)) >> 8)  | \
        (((ubyte8)((value) & 0x0000FF0000000000ULL)) >> 24) | \
        (((ubyte8)((value) & 0x00FF000000000000ULL)) >> 40) | \
        (((ubyte8)((value) & 0xFF00000000000000ULL)) >> 56)



/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                     EXPORTED VARIABLES                                 **/
/**                                                                        **/
/****************************************************************************/

#ifndef _STDDEFS_C_SRC
#endif

/****************************************************************************/
/**                                                                        **/
/**                     EXPORTED FUNCTIONS                                 **/
/**                                                                        **/
/****************************************************************************/


#endif

/****************************************************************************/
/**                                                                        **/
/**                               EOF                                      **/
/**                                                                        **/
/****************************************************************************/

