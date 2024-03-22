/******************************************************************************
 *                         COPYRIGHT 2001 IWEDIA TECHNOLOGIES                 *
 ******************************************************************************
 *
 * MODULE NAME:     TDAL_DESC
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/dev/sources/chal/core/tdal/trunk/inc/tdal_desc.h $
 *      $Rev: 7318 $
 *      $Date: 2013-02-26 20:16:38 +0100 (Tue, 26 Feb 2013) $
 *
 * PUBLIC
 *
 * DESCRIPTION:     Header of desc
 *
 *****************************************************************************/

#ifndef _TDAL_DESC_H_
#define _TDAL_DESC_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*              Defines                                */
/*******************************************************/
#define kTDAL_DESC_ILLEGAL_DESCRAMBLER 0xFFFFFFFF
#define kTDAL_DESC_ILLEGAL_PID 0x1FFF
#define kTDAL_DESC_MAX_DESCRAMBLERS     5

#define kTDAL_DESC_MAX_KEY_EVEN_ODD_SIZE 8
#define kTDAL_DESC_MAX_KEY_L1_SIZE 16
/*******************************************************/
/*              Macros                                 */
/*******************************************************/

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/
typedef uint32_t tTDAL_DESC_descrambler;

typedef enum
{
    eTDAL_DESC_NO_ERROR = 0,
    eTDAL_DESC_NOT_DONE,
    eTDAL_DESC_ERROR_UNKNOW_ID,
    eTDAL_DESC_ERROR
}tTDAL_DESC_Error;

/* for compatibility */
typedef enum
{
    TDAL_DESC_RESERVED = 0x00,
    TDAL_DESC_VIDEO = 0x01,
    TDAL_DESC_OTHER_VIDEO = 0x02,
    TDAL_DESC_AUDIO = 0x03,
    TDAL_DESC_OTHER_AUDIO = 0x04,
    TDAL_DESC_PRIVATE_SECTION = 0x05,
    TDAL_DESC_PES_PACKETS_CONTAINING_PRIVATE_DATA  = 0x06,
    TDAL_DESC_MHEG = 0x07,
    TDAL_DESC_DSM_CC = 0x08
}TDAL_DESC_stream_type_e;

typedef enum
{
    TDAL_DESC_DVB_CSA       = 0x00,
    TDAL_DESC_AES_128_CBC   = 0x01,
    TDAL_DESC_DES_56_ECB    = 0x02
}TDAL_DESC_DescType;

typedef enum
{
    TDAL_DESC_ODD_KEY = 0,
	TDAL_DESC_EVEN_KEY,
	TDAL_DESC_GENERIC_KEY
}tTDAL_DESC_KeyType;

typedef struct {
    uint8_t bNbDescrambler_Max;
    uint8_t isDVB_CSA_Supported;
    uint8_t isAES_128_CBC_Supported;
    uint8_t isDES_56_ECB_Supported;
}tTDAL_DESC_Capability;



/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/

tTDAL_DESC_Error TDAL_DESC_Init(void);
tTDAL_DESC_Error TDAL_DESC_Terminate(void);
tTDAL_DESC_descrambler TDAL_DESC_Open_Descrambler (TDAL_DESC_stream_type_e stream_type);
tTDAL_DESC_Error TDAL_DESC_Is_Descrambler_Open(tTDAL_DESC_descrambler descrambler);

tTDAL_DESC_Error TDAL_DESC_Close_Descrambler    (tTDAL_DESC_descrambler descrambler);
tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Pid  (tTDAL_DESC_descrambler descrambler, int16_t pid);
tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Keys (tTDAL_DESC_descrambler descrambler,
                                        int16_t odd_key_length, const int8_t *odd_key,
                                        int16_t even_key_length, const int8_t *even_key);

tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Keys_L1 (tTDAL_DESC_descrambler descrambler,
                                        int16_t L1_key_length, const int8_t *L1_key,
                                        int16_t odd_key_length, const int8_t *odd_key,
                                        int16_t even_key_length, const int8_t *even_key);                                        
                                        
tTDAL_DESC_Error TDAL_DESC_Stream_Authorisation (int16_t pid, uint8_t authorize);

tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Type (tTDAL_DESC_descrambler descrambler,
                     TDAL_DESC_DescType descrambler_type);
tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Key  (tTDAL_DESC_descrambler descrambler,
                                                 tTDAL_DESC_KeyType KeyType,
                                                 int16_t key_length, const int8_t *key);
const char * TDAL_DESC_APIRevisionGet( void );
const char * TDAL_DESC_PlatformRevisionGet( void );
tTDAL_DESC_Error TDAL_DESC_GetCapability(tTDAL_DESC_Capability *pstCapability);
tTDAL_DESC_Error TDAL_DESCg_Open_DescramblerChannel(uint8_t demuxId,uint16_t pid);
tTDAL_DESC_Error TDAL_DESCg_Close_DescramblerChannel(uint8_t demuxId,uint16_t pid);


#ifdef TDAL_DESC_DVB_SECURE_CHIPSET
tTDAL_DESC_Error TDAL_DESC_SECURE_GetChipId(uint32_t *chipId);
tTDAL_DESC_Error TDAL_DESC_SECURE_SetCurrentKey(uint8_t *currentKey);
tTDAL_DESC_Error TDAL_DESC_SECURE_DescrambleCW(uint8_t *controlWord, uint8_t **ChipsetValue);
#endif

int TDAL_DESCRAMBLE_TsDescramble( uint8_t* pu8Data, uint32_t u32Size );


#ifdef __cplusplus
}
#endif

#endif /*_TDAL_DESC_H_*/



