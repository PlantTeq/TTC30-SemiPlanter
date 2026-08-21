/**************************************************************************
 * Copyright (c) 2018 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 *
 * \file IO_Crypt.h
 *
 * \brief API for I/O driver cryptographic functions
 *
 * \section xtea_encipher_algo XTEA block cipher algorithm
 *
 * The following section shows the XTEA block cipher algorithm as C code examples.
 * The examples can be used as basis for integration of the algorithm into a
 * diagnostic tester. The tester only needs the functions for enciphering (a comparable
 * decipher function is used by the ECU). Nevertheless, for the purpose of completion both
 * encipher and decipher functions are listed. The XTEA algorithm is a freely available
 * algorithm for symmetrical encryption purposes. The current cipher implementation uses
 * 64 feistel rounds. The algorithm uses a 128bit secret key that can be chosen freely
 * but must of course be kept secret. If the chosen secret key becomes public security
 * is compromised!
 *
 * The application code does not have to implement the following functions because the
 * algorithm is already included in the I/O driver library:
 * - \c IO_Crypt_XteaEncipher
 * - \c IO_Crypt_XteaDecipher
 * - \c IO_Crypt_XteaEncipher32
 * - \c IO_Crypt_XteaDecipher32
 *
 * \section xtea_encipher_algo_impl XTEA block cipher algorithm details
 *
 * \code
 * #define XTEA_DELTA          (ubyte4)0x9E3779B9UL
 * #define XTEA_DELTA_SHORT    (ubyte2)0x79B9U
 * #define XTEA_NUM_CYCLES     (ubyte4)32UL // 1 cycle corresponds to 2 feistel rounds. 32 cycles := 64 feistel rounds
 * \endcode
 *
 * \code
 * // Enciphers a 32bit value
 * void xtea_encipher_u32( ubyte4 * const v
 *                       , const ubyte4 * const key )
 * {
 *     ubyte2 i; // use a machine word for the loop variable
 *     ubyte2 sum;
 *     ubyte2 v0;
 *     ubyte2 v1;
 *
 *     sum = 0;
 *
 *     v0 = (ubyte2)*v;
 *     v1 = (ubyte2)(*v >> 16);
 *
 *     for (i = 0; i < XTEA_NUM_CYCLES; i++) // 1 cycle corresponds to 2 feistel rounds. 32 cycles := 64 feistel rounds
 *     {
 *         v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + (ubyte2)key[sum & 3]);
 *         sum += XTEA_DELTA_SHORT; // adding XTEA delta value. overrun of sum is irrelevant
 *         v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + (ubyte2)key[(sum>>11) & 3]);
 *     }
 *
 *     *v = (ubyte4)v0;
 *     *v |= (ubyte4)v1 << 16;
 * }
 * \endcode
 *
 * \code
 * // Deciphers a 32bit value
 * void xtea_decipher_u32( ubyte4 * const v
 *                       , const ubyte4 * const key )
 * {
 *     ubyte2 i; // use a machine word for the loop variable
 *     ubyte2 sum;
 *     ubyte2 v0;
 *     ubyte2 v1;
 *
 *     sum = (ubyte2)((ubyte4)XTEA_DELTA_SHORT * (ubyte4)XTEA_NUM_CYCLES); // overrun of sum is irrelevant
 *
 *     v0 = (ubyte2)*v;
 *     v1 = (ubyte2)(*v >> 16);
 *
 *     for (i = 0; i < XTEA_NUM_CYCLES; i++) // 1 cycle corresponds to 2 feistel rounds. 32 cycles := 64 feistel rounds
 *     {
 *         v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + (ubyte2)key[(sum>>11) & 3]);
 *         sum -= XTEA_DELTA_SHORT;
 *         v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + (ubyte2)key[sum & 3]);
 *     }
 *
 *     *v = (ubyte4)v0;
 *     *v |= (ubyte4)v1 << 16;
 * }
 * \endcode
 *
 * \code
 * // Enciphers a 64bit value (2*32bit, high word in v1, low word in v0)
 *  void xtea_encipher( ubyte4 * const v0
 *                    , ubyte4 * const v1
 *                    , const ubyte4 * const key )
 * {
 *     ubyte2 i; // use a machine word for the loop variable
 *     ubyte4 sum;
 *
 *     sum = 0;
 *
 *     for (i = 0; i < XTEA_NUM_CYCLES; i++) // 1 cycle corresponds to 2 feistel rounds. 32 cycles := 64 feistel rounds
 *     {
 *         *v0 += (((*v1 << 4) ^ (*v1 >> 5)) + *v1) ^ (sum + key[sum & 3]);
 *         sum += XTEA_DELTA; // overrun of sum is irrelevant
 *         *v1 += (((*v0 << 4) ^ (*v0 >> 5)) + *v0) ^ (sum + key[(sum>>11) & 3]);
 *     }
 * }
 * \endcode
 *
 * \code
 * // Deciphers a 64bit value (2*32bit, high word in v1, low word in v0)
 * void xtea_decipher( ubyte4 * const v0
 *                   , ubyte4 * const v1
 *                   , const ubyte4 * const key )
 * {
 *     ubyte2 i; // use a machine word for the loop variable
 *     ubyte4 sum;
 *
 *     sum = (ubyte4)((ubyte8)XTEA_DELTA * (ubyte8)XTEA_NUM_CYCLES); // overrun of sum is irrelevant
 *
 *     for (i = 0; i < XTEA_NUM_CYCLES; i++) // 1 cycle corresponds to 2 feistel rounds. 32 cycles := 64 feistel rounds
 *     {
 *         *v1 -= (((*v0 << 4) ^ (*v0 >> 5)) + *v0) ^ (sum + key[(sum>>11) & 3]);
 *         sum -= XTEA_DELTA;
 *         *v0 -= (((*v1 << 4) ^ (*v1 >> 5)) + *v1) ^ (sum + key[sum & 3]);
 *     }
 * }
 * \endcode
 *
 **************************************************************************/

#ifndef IO_CRYPT_H_
#define IO_CRYPT_H_


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
 * \brief Definition of key length for XTEA algorithm
 */
/*\{*/
#define IO_CRYPT_XTEA_KEY_LEN       4U  /**< The XTEA algorithm operates with 128 bit keys (4*32bit) */
/*\}*/

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
 * \brief Returns a pseudo random number
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                     Everything ok.
 * \retval #IO_E_NULL_POINTER           A NULL pointer has been passed.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED I/O driver has not been initialized
 *                                      before.
 *
 **************************************************************************
 *
 * \remark
 *   This functions returns a 32bit random number. If a 64bit random number
 *   is needed, the function can be called twice like in code example below.
 *
 * \code
 *
 * ubyte8 prn64;
 * ubyte4 prn32;
 *
 * IO_Crypt_GetPseudoRandomNumber(&prn32);
 * prn64 = (ubyte8)prn32 << 32;
 * IO_Crypt_GetPseudoRandomNumber(&prn32);
 * prn64 |= prn32;
 *
 * \endcode
 *
 **************************************************************************/
IO_ErrorType IO_Crypt_GetPseudoRandomNumber( ubyte4 * const prn );


/**********************************************************************//**
 *
 * \brief Enciphers a 64bit value with the XTEA algorithm (in-place)
 *
 * \param[in,out]   v0              lower half of 64bit block to encipher
 * \param[in,out]   v1              upper half of 64bit block to encipher
 * \param[in]       key             secret key, must have 4 elements
 * \param[in]       key_encrypted   set to #TRUE if given key is encrypted
 *                                  (e.g. because it originates from the
 *                                  branding block), otherwise to #FALSE.
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                 Everything ok.
 * \retval #IO_E_NULL_POINTER       A NULL pointer has been passed.
 *
 **************************************************************************/
IO_ErrorType IO_Crypt_XteaEncipher( ubyte4 * const v0
                                  , ubyte4 * const v1
                                  , const ubyte4 * const key
                                  , bool key_encrypted );

/**********************************************************************//**
 *
 * \brief Deciphers a 64bit value with the XTEA algorithm (in-place)
 *
 * \param[in,out]   v0              lower half of 64bit block to decipher
 * \param[in,out]   v1              upper half of 64bit block to decipher
 * \param[in]       key             secret key, must have 4 elements
 * \param[in]       key_encrypted   set to #TRUE if given key is encrypted
 *                                  (e.g. because it originates from the
 *                                  branding block), otherwise to #FALSE.
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                 Everything ok.
 * \retval #IO_E_NULL_POINTER       A NULL pointer has been passed.
 *
 **************************************************************************/
IO_ErrorType IO_Crypt_XteaDecipher( ubyte4 * const v0
                                  , ubyte4 * const v1
                                  , const ubyte4 * const key
                                  , bool key_encrypted );

/**********************************************************************//**
 *
 * \brief Enciphers a 32bit value with the XTEA algorithm (in-place)
 *
 * \param[in,out]   v               32bit block to encipher
 * \param[in]       key             secret key, must have 4 elements
 * \param[in]       key_encrypted   set to #TRUE if given key is encrypted
 *                                  (e.g. because it originates from the
 *                                  branding block), otherwise to #FALSE.
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                 Everything ok.
 * \retval #IO_E_NULL_POINTER       A NULL pointer has been passed.
 *
 **************************************************************************
 *
 * \remark The XTEA algorithm has been designed to work on 64bit blocks
 *         (2x32). This 32bit version is a derivative of it that operates
 *         based on the same secret key but using only 4x16bit of it instead
 *         of 4x32bit.
 *
 **************************************************************************/
IO_ErrorType IO_Crypt_XteaEncipher32( ubyte4 * const v
                                    , const ubyte4 * const key
                                    , bool key_encrypted );

/**********************************************************************//**
 *
 * \brief Deciphers a 32bit value with the XTEA algorithm (in-place)
 *
 * \param[in,out]   v               32bit block to decipher
 * \param[in]       key             secret key, must have 4 elements
 * \param[in]       key_encrypted   set to #TRUE if given key is encrypted
 *                                  (e.g. because it originates from the
 *                                  branding block), otherwise to #FALSE.
 *
 * \return IO_ErrorType
 * \retval #IO_E_OK                 Everything ok.
 * \retval #IO_E_NULL_POINTER       A NULL pointer has been passed.
 *
 **************************************************************************
 *
 * \remark The XTEA algorithm has been designed to work on 64bit blocks
 *         (2x32). This 32bit version is a derivative of it that operates
 *         based on the same secret key but using only 4x16bit of it instead
 *         of 4x32bit.
 *
 **************************************************************************/
IO_ErrorType IO_Crypt_XteaDecipher32( ubyte4 * const v
                                    , const ubyte4 * const key
                                    , bool key_encrypted );


#endif /* IO_CRYPT_H_ */
