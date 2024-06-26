/******************************************************************************
 *                         COPYRIGHT 2011 SMARDTV                             *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_PERSO
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/dev/sources/chal/core/tdal/trunk/inc/tdal_perso.h $
 *            $Rev: 441 $
 *            $Date: 2011-09-28 13:56:52 +0200 (Wed, 28 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *      Header of the TDAL Perso
 *
 *****************************************************************************/

#ifndef _TDAL_PERSO_H_
#define _TDAL_PERSO_H_

/*******************************************************/
/*              Includes                               */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include <crules.h>

/*******************************************************/
/*              Defines                                */
/*******************************************************/

/** SmarDTV serial number. It is used as the entry point to the factory data base **/
#define PERSO_TAG_SDTV_SN           0x00000001L

/** Platform ID. It is used to target a platform at download phase **/
#define PERSO_TAG_PLATFORM_ID       0x00000004L

/** SmarDTV part number **/
#define PERSO_TAG_SMDTV_PART_NB     0x00000005L

/** Brand ID. It is used to target a product at download phase **/
#define PERSO_TAG_BRAND_ID          0x00000006L

/** Brand Name. The name that should be displayed in the menu **/
#define PERSO_TAG_BRAND_NAME        0x00000007L

/** Flash sector lock list. Hold a list of sector to be locked down at final test **/
#define PERSO_TAG_LOCK_LIST         0x0000000FL

/** Control Access Serial Number 
    Note that this value redundant with the beginning of the pairing keys **/
#define PERSO_TAG_STB_CA_SN         0x00000010L

/** Nagravision pairing keys **/
#define PERSO_TAG_PK                0x00000013L

/** Chipset Secure Channel Data **/
#define PERSO_TAG_CSCD              0x00000014L

/** Nagra provider Id **/ 
#define PERSO_TAG_PROVIDER_ID       0x00000015L

/** End marker */
#define PERSO_TAG_MAX_ID            0xFFFFFFFFL

/*******************************************************/
/*              Macros                                 */
/*******************************************************/

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/

/* These are error codes returned by the API */
typedef enum 
{
    eTDAL_PERSO_NO_ERROR,
    eTDAL_PERSO_NOT_INITIALIZED,
    eTDAL_PERSO_NOT_FORMATTED,
    eTDAL_PERSO_BAD_PARAMETER,
    eTDAL_PERSO_ERROR
}
eTDAL_PERSO_ErrorCode;

/* This is the identifier for tags readable in the personalization area */
typedef uint32_t tTDAL_PERSO_Tag;

/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/

eTDAL_PERSO_ErrorCode TDAL_PERSO_Init( void );
eTDAL_PERSO_ErrorCode TDAL_PERSO_Term( void );

eTDAL_PERSO_ErrorCode TDAL_PERSO_ReadTag( tTDAL_PERSO_Tag const, uint16_t* const, uint8_t* const );

#ifdef __cplusplus
}
#endif


#endif /*_TDAL_PERSO_H_*/

