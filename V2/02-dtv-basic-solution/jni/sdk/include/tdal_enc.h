#ifndef _TDAL_ENC_H_
#define _TDAL_ENC_H_
/*******************************************************/
/*              Includes                               */
/*******************************************************/
#include "tdal_av.h"
/*******************************************************/
/*              Typedefs                               */
/*******************************************************/
typedef enum
{
    eTDAL_ENC_NO_ERROR,
    eTDAL_ENC_ERROR,
    eTDAL_ENC_BAD_PARAMETER_ERROR,
    eTDAL_ENC_LIMITATION_ERROR
} tTDAL_ENC_Error;

typedef enum
{
    eTDAL_ENC_NO_TRANSCODING,
    eTDAL_ENC_TRANSCODING_PARAMS_0,
    eTDAL_ENC_TRANSCODING_PARAMS_1,
    eTDAL_ENC_TRANSCODING_PARAMS_2,
    eTDAL_ENC_TRANSCODING_PARAMS_3,
    eTDAL_ENC_TRANSCODING_PARAMS_4
} tTDAL_ENC_TranscodingParams;
/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/
tTDAL_ENC_Error TDAL_ENC_Init(void);
tTDAL_ENC_Error  pTDAL_ENCp_CreateEncoder(uint8_t encoderIdx);
tTDAL_ENC_Error TDAL_ENC_GetHandle(tTDAL_AV_Decoder decoderId, uint32_t* encHandle);
tTDAL_ENC_Error TDAL_ENC_AttachInput(tTDAL_AV_Decoder decoderId, uint32_t inputHandle);
tTDAL_ENC_Error TDAL_ENC_SetTranscodingParams(tTDAL_AV_Decoder decoderId, tTDAL_ENC_TranscodingParams transcodingParams);
tTDAL_ENC_Error TDAL_ENCm_isTranscodingPassthrough(tTDAL_AV_Decoder decoderId, bool *bIsPassthrough);
tTDAL_ENC_Error TDAL_ENC_DetachInput(tTDAL_AV_Decoder decoderId);
tTDAL_ENC_Error TDAL_ENC_Destroy(tTDAL_AV_Decoder decoderId);

#endif /* _TDAL_ENC_H_ */
