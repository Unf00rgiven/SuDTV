/*
 * tdal_marlin_crypt.h
 *
 *  Created on: Feb 27, 2017
 *      Author: rtrk
 */

#ifndef _TDAL_MARLIN_CRYPT_H_
#define _TDAL_MARLIN_CRYPT_H_

/*--------------------------*/
/*    Header needed.        */
/*--------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
/*------------------------------------------*/
/*           Types definition        */
/*------------------------------------------*/

#define DRM_LEN_BITS_TIME           4
#define DRM_LEN_BITS_TIERBITS       8
#define DRM_LEN_BITS_RO             2
#define DRM_LEN_ECM                 98
#define DRM_LEN_KEY                 16
#define DRM_LEN_ID                  8
#define DRM_LEN_LICENSE_ID          32
#define DRM_LEN_PROTOCOL_VER        2

#define MARLIN_LICENSE_COUNT_MAX 32

typedef enum
{
    //! This is aligned with marlin lib
    eTDAL_MARLIN_CRYPT_NO_ERROR = 0,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_ERROR,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_SYSTEM_ERROR,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_NO_DATA,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_SERVER_ERROR,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_NO_LICENSE,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_HTTP_ERROR,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_SAC_ERROR,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_PROTOCOL_ERROR,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_VERIFY_ERROR,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_CERT_ERROR,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_NO_LICENSE_KW,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_NO_LICENSE_TIER,
    eTDAL_MARLIN_CRYPT_DRM_LIBRARY_NO_LICENSE_TIME,

    //! Our error codes
    eTDAL_MARLIN_CRYPT_ALREADY_INITIALIZED,
    eTDAL_MARLIN_CRYPT_NOT_INITIALIZED,
    eTDAL_MARLIN_CRYPT_GENERAL_ERROR,
    eTDAL_MARLIN_CRYPT_SUBSCRIPTION_ERROR,
    eTDAL_MARLIN_CRYPT_UNDEFINED,
    eTDAL_MARLIN_UNKNOWN_DRM_LIBRARY,
} tTDAL_MARLIN_CRYPT_Error;

typedef enum
{
    eTDAL_MARLIN_EVEN_KEY,
    eTDAL_MARLIN_ODD_KEY
} tTDAL_MARLIN_CRYPT_KEY;

typedef struct
{
    char licenseId[DRM_LEN_LICENSE_ID];
    time_t timeNotBefore;
    time_t timeNotAfter;
    time_t timeUpdateOffset;
    char tierBit[DRM_LEN_BITS_TIERBITS];
    char renderingObligation[DRM_LEN_BITS_RO];
} MarlinLicenseInfo;

typedef enum
{
    eTDAL_MARLIN_CRYPT_INIT_OK = 0,
    eTDAL_MARLIN_CRYPT_INIT_FAILED,
    eTDAL_MARLIN_CRYPT_INIT_STATUS_UNKNOWN,
} tTDAL_MARLIN_CRYPT_Init_Status;

typedef struct _TDAL_MARLIN_CRYPT_CTX_
{
    char sessionId[256];
    bool sessionOpened;
    char oddKey[DRM_LEN_KEY];
    char evenKey[DRM_LEN_KEY];
    char drmID[DRM_LEN_ID];
    char* acsUrl;
    char* crlUrl;
    char* signature;
    char* certificate;
    int serviceProvider;
    int numberOfLicenses;
    bool ecmProvided;
    MarlinLicenseInfo* licensesInfo;
    pthread_mutex_t decryptMutex;
    pthread_mutex_t licenseMutex;
    char aucDrmIDHex[8 * 2 + 1];
} TDAL_MARLIN_CRYPT_CTX;

typedef struct _TDAL_MARLIN_CRYPT_License
{
    const char* id;
    int type;
} TDAL_MARLIN_CRYPT_License;

/********************************************************/
/*              Typedefs                                */
/********************************************************/

typedef int32_t (*tTDAL_MARLIN_CRYPT_NotifyFct) (int32_t event, void * data);

/*------------------------------------------*/
/*       Functions Declarations      */
/*------------------------------------------*/

void TDAL_MARLIN_CRYPT_EarlyInit();
void TDAL_MARLIN_CRYPT_EarlyTerminate();
tTDAL_MARLIN_CRYPT_Error TDAL_MARLIN_CRYPT_InitDevicePacket(int factoryFlag);

tTDAL_MARLIN_CRYPT_Error TDAL_MARLIN_CRYPT_Init(const char* crlUrl, const char* acsUrl, const char* sig, const char* cert);
tTDAL_MARLIN_CRYPT_Error TDAL_MARLIN_CRYPT_Term(void);
tTDAL_MARLIN_CRYPT_Error TDAL_MARLIN_CRYPT_UpdateLicenses(TDAL_MARLIN_CRYPT_License *licenses,
        uint32_t numLicenses, bool download);
bool TDAL_MARLIN_CRYPT_IsLicenseAvailable(const char* licenseId, const char* tierbitMask);
char* TDAL_MARLIN_CRYPT_GetDrmId(int32_t type);
tTDAL_MARLIN_CRYPT_Error TDAL_MARLIN_CRYPT_OpenSession(char *licenseId, char* tierBitMask);
tTDAL_MARLIN_CRYPT_Error TDAL_MARLIN_CRYPT_CloseSession(void);
tTDAL_MARLIN_CRYPT_Error TDAL_MARLIN_CRYPT_PassEcmData(uint8_t *ecmData,
        uint32_t ecmLengt, uint8_t* event_key, uint8_t* odd_key,
        uint8_t* renderingObligation, uint32_t demuxID);
tTDAL_MARLIN_CRYPT_Error TDAL_MARLIN_CRYPT_DECRYPT(uint8_t *inputData, uint32_t dataLength, uint8_t* outputData,
    tTDAL_MARLIN_CRYPT_KEY cryptKey);
tTDAL_MARLIN_CRYPT_Error TDAL_MARLIN_CRYPT_ENCRYPT(uint8_t *inputData, uint32_t dataLength, uint8_t* outputData,
    tTDAL_MARLIN_CRYPT_KEY cryptKey);
bool TDAL_MARLIN_CRYPT_isEcmProvided(void);
bool TDAL_MARLIN_CRYPT_setServiceRegistration(int32_t type, const char* serverUri, const char* sigPath, const char* sigCertPath);
void TDAL_MARLIN_CRYPT_SetLicenseRequestCallback(tTDAL_MARLIN_CRYPT_NotifyFct callback);

#ifdef __cplusplus
}
#endif
#endif /* _TDAL_MARLIN_CRYPT_H_ */
