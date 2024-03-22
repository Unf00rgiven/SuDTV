/******************************************************************************
 *                          COPYRIGHT 2004 IWEDIA TECHNOLOGIES                *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_CRYP
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/dev/sources/chal/core/tdal/trunk/inc/tdal_crypt.h $
 *            $Rev: 7843 $
 *            $Date: 2013-03-26 19:01:44 +0100 (Tue, 26 Mar 2013) $
 *
 * Description			: Cryptography TDAL implementation
 * Target				: All
 * Author			    : Xavier RAZAVET.
 * Date	of creation		: 16 / 07 /	2004.
 *
 *****************************************************************************/

#ifndef _TDAL_CRYPT_H_
#define _TDAL_CRYPT_H_

/*--------------------------*/
/*	  Header needed.	    */
/*--------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#define kTDAL_CRYPT_SECURE_CHIP_KEY         "SECURE_CHIP_KEY"
#define kTDAL_CRYPT_SECURE_CHIP_KEY_SIZE    strlen(kTDAL_CRYPT_SECURE_CHIP_KEY)

/*------------------------------------------*/
/*           Public types definition        */
/*------------------------------------------*/

typedef enum
{
	eTDAL_CRYPT_NO_ERROR = 0,
	eTDAL_CRYPT_BAD_PARAMETER,
	eTDAL_CRYPT_ERROR
}
tTDAL_CRYPT_Error;

typedef enum
{
	eTDAL_CRYPT_TDES,
	eTDAL_CRYPT_AES
}
tTDAL_CRYPT_Algorithm;


typedef struct
{
	tTDAL_CRYPT_Algorithm    algo;

	uint8_t 			     *pKey;
	uint16_t 			     keylength;

	uint8_t 			     *pSource;
	uint8_t				     *pDest;

	uint32_t 			     offset;

	uint32_t 			     sizeToProcess;
	uint32_t 			     processedSize;
}
tTDAL_CRYPT_Params;
	


/*------------------------------------------*/
/*       Public Functions Declarations      */
/*------------------------------------------*/


tTDAL_CRYPT_Error TDAL_CRYPT_Init (void);
tTDAL_CRYPT_Error TDAL_CRYPT_Term(void);


tTDAL_CRYPT_Error TDAL_CRYPT_Encrypt(tTDAL_CRYPT_Params *pParams);
tTDAL_CRYPT_Error TDAL_CRYPT_Decrypt(tTDAL_CRYPT_Params *pParams);

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_CRYPT_H_*/
