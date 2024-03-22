/******************************************************************************
 *                          COPYRIGHT 2015 IWEDIA TECHNOLOGIES                *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DISEQC
 *
 * FILE NAME: tdal_diseqc.h
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _TDAL_DISEQC_H_
#define _TDAL_DISEQC_H_


/*****************************************************************************/
/*                     Includes                                              */
/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/*                     Defines                                               */
/*****************************************************************************/

/*****************************************************************************/
/*                     Macros                                                */
/*****************************************************************************/

/*****************************************************************************/
/*                     Typedefs                                              */
/*****************************************************************************/

/**
 * Enumeration which contains values for DiSEqC errors.
 */
typedef enum
{
   TDAL_DISEQC_NO_ERROR = 0,
   TDAL_DISEQC_ERROR_BAD_PARAMETER,
   TDAL_DISEQC_ERROR_NOT_DONE,
   TDAL_DISEQC_ERROR_MAX
}tTDAL_DISEQC_Error;

/**
 * Enumeration which contains values for DiSEqC voltages.
 */
typedef enum
{
   TDAL_DISEQC_VOLTAGE_E13V, /* Polarization Vertical*/
   TDAL_DISEQC_VOLTAGE_E18V, /* Polarization Horizontal */
   TDAL_DISEQC_VOLTAGE_MAX
} tTDAL_DISEQC_VOLTAGE;

/**
 * Enumeration which contains values for DiSEqC tone bursts.
 */
typedef enum
{
   TDAL_DISEQC_TONE_BURST_NONE,
   TDAL_DISEQC_TONE_BURST_UNMODULATED,
   TDAL_DISEQC_TONE_BURST_NOMINAL,
   TDAL_DISEQC_TONE_BURST_MAX
} tTDAL_DISEQC_TONE_BURST;

/**
 * Enumeration which contains values for DiSEqC tone modes.
 */
typedef enum
{
   TDAL_DISEQC_TONE_MODE_TONE,
   TDAL_DISEQC_TONE_MODE_ENVELOPE,
   TDAL_DISEQC_TONE_MODE_MAX
} tTDAL_DISEQC_TONE_MODE;

/**
 * Enumeration which contains values for DiSEqC message framing overrides.
 *
 * DiSEqC messages can embed whether or not a reply is expected. When set to
 * TDAL_DISEQC_FRAMING_DEFAULT, diseqc will use the value in the command to
 * determine whether or not a reply is expected.
 * When set to TDAL_DISEQC_FRAMING_EXPECT_REPLY, this is overridden, and it
 * forces a reply to be expected.
 * When set to TDAL_DISEQC_FRAMING_DONT_EXPECT_REPLY, this is overridden, and no
 * reply is expected.
 */
typedef enum
{
   TDAL_DISEQC_FRAMING_DEFAULT,
   TDAL_DISEQC_FRAMING_EXPECT_REPLY,
   TDAL_DISEQC_FRAMING_DONT_EXPECT_REPLY,
   TDAL_DISEQC_FRAMING_MAX
} tTDAL_DISEQC_FRAMING;

/**
 * Structure is used to store DiSEqC settings for tone and voltage control.
 */
typedef struct tTDAL_DISEQC_Settings
{
    uint8_t                    bLNBEnabled; /* LNBP voltage (low noise blocker power)
                                        // is enabled, when it is set to true.
                                        // Otherwise, LNBP voltage is zero volt.*/

    tTDAL_DISEQC_VOLTAGE    eVoltage;
    uint8_t                    bToneEnabled;
    tTDAL_DISEQC_TONE_BURST eToneBurst;
    tTDAL_DISEQC_TONE_MODE  eToneMode;

    tTDAL_DISEQC_FRAMING    eFraming;

    uint32_t                ui32PreTransmitDelay; /* Delay is in milliseconds.*/

    uint8_t                    bEnabled; /* If set to false, power down the diseqc core.*/
} tTDAL_DISEQC_Settings;

/**
 * Enumeration contains values for DiSEqC message transmission statuses.
 */
typedef enum
{
   TDAL_DISEQC_MESSAGE_STATUS_SUCCESS = 0, /* No error*/
   TDAL_DISEQC_MESSAGE_STATUS_RX_OVERFLOW,
   TDAL_DISEQC_MESSAGE_STATUS_RX_REPLY_TIMEOUT,
   TDAL_DISEQC_MESSAGE_STATUS_RX_PARITY_ERROR,
   TDAL_DISEQC_MESSAGE_STATUS_ACW_TIMEOUT,
   TDAL_DISEQC_MESSAGE_STATUS_BUSY,
   TDAL_DISEQC_MESSAGE_STATUS_MAX
} tTDAL_DISEQC_MESSAGE_STATUS;

/**
 * Structure is used to store DiSEqC status
 *
 * This retrives the state of hardware, not current software state.
 */
typedef struct tTDAL_DISEQC_Status
{
   uint8_t                        bToneEnabled; /* 22KHz continuous tone present*/
   uint32_t                    ui32Voltage; /* voltage estimation*/
   tTDAL_DISEQC_MESSAGE_STATUS eSendStatus; /* Last DiSeqC send status.*/
                                            /* Use this if no reply buffer is needed.*/
} tTDAL_DISEQC_Status;

/**
 * Prototype of DiSEqC callback.
 */
typedef void (*tTDAL_DISEQC_Callback)(void *param);

/**
 * Structure is used to store DiSEqC message data.
 */
typedef struct tTDAL_DISEQC_Message
{
   tTDAL_DISEQC_Callback SendCompleteCallback; /* The callback provided will be
                                               // called when the transfer is
                                               // complete, at which point the
                                               // reply/status can be read.*/
   uint32_t ui32DataSize; /* Data size in bytes*/
   uint8_t *pui8Data;     /* Message data*/
} tTDAL_DISEQC_Message;

typedef struct tTDAL_DISEQC_Reply
{
   tTDAL_DISEQC_MESSAGE_STATUS eStatus;
   uint8_t  *pui8ReplyBuffer; /* Reply buffer must be pre-allocated.*/
   uint32_t ui32ReplyBufferSize; /* Size of pre-allocated reply buffer (in bytes)*/
   uint32_t ui32ReplyLength; /* Actual size (in bytes) of reply buffer returned by TDAL_DISEQC_GetReply.*/
} tTDAL_DISEQC_Reply;

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/

/**
 * @brief Function initializes DiSEqC module.
 *
 * @param
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_Init(void);

/**
 * @brief Function terminates DiSEqC module.
 *
 * @param
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_Terminate(void);

/**
 * @brief Function enables DiSEqC LNB.
 *
 * @param [in] pFrontendHandle - Frontend handle.
 * @param [in] bEnableLNB - LNB is enabled if it is set to true.
 *                          Otherwise, it is disabled.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 *
 * NOTE: Initialization of frontend enables DiSEqC LNB by default.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_EnableLNB(void *pFrontendHandle,
                                         uint8_t bEnableLNB);

/**
 * @brief Function checks if DiSEqC LNB is enabled.
 *
 * @param [in]  pFrontendHandle - Frontend handle.
 * @param [out] pbIsLNBEnabled - If it is true, LNB is enabled.
 *                              Otherwise, it is disabled.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_IsLNBEnabled(void *pFrontendHandle,
                                            uint8_t *pbIsLNBEnabled);

/**
 * @brief Function enables DiSEqC tone.
 *
 * @param [in] pFrontendHandle - Frontend handle.
 * @param [in] bEnableTone - DiSEqC tone is enabled if it is set to true.
 *                          Otherwise, it is disabled.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_EnableTone(void *pFrontendHandle,
                                          uint8_t bEnableTone);

/**
 * @brief Function checks if DiSEqC tone is enabled.
 *
 * @param [in]  pFrontendHandle - Frontend handle.
 * @param [out] pbIsEnableTone - If it is true, DiSEqC tone is enabled.
 *                               Otherwise, it is disabled.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_IsToneEnabled(void *pFrontendHandle,
                                             uint8_t *pbIsEnableTone);

/**
 * @brief Function sets DiSEqC voltage.
 *
 * @param [in] pFrontendHandle - Frontend handle.
 * @param [in] eVoltage - DiSEqC voltage.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_SetVoltage(void *pFrontendHandle,
                                          tTDAL_DISEQC_VOLTAGE eVoltage);

/**
 * @brief Function gets DiSEqC voltage.
 *
 * @param [in]  pFrontendHandle - Frontend handle.
 * @param [out] peVoltage - DiSEqC voltage.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_GetVoltage(void *pFrontendHandle,
                                          tTDAL_DISEQC_VOLTAGE *peVoltage);

/**
 * @brief Function sets DiSEqC tone burst.
 *
 * @param [in] pFrontendHandle - Frontend handle.
 * @param [in] eToneBurst - DiSEqC tone burst.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_SetToneBurst(void *pFrontendHandle,
                                            tTDAL_DISEQC_TONE_BURST eToneBurst);

/**
 * @brief Function gets DiSEqC tone burst.
 *
 * @param [in]  pFrontendHandle - Frontend handle.
 * @param [out] peToneBurst - DiSEqC tone burst.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_GetToneBurst(void *pFrontendHandle,
                                            tTDAL_DISEQC_TONE_BURST *peToneBurst);

/**
 * @brief Function sets DiSEqC tone mode.
 *
 * @param [in] pFrontendHandle - Frontend handle.
 * @param [in] eToneMode - DiSEqC tone mode.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_SetToneMode(void *pFrontendHandle,
                                           tTDAL_DISEQC_TONE_MODE eToneMode);

/**
 * @brief Function gets DiSEqC tone mode.
 *
 * @param [in]  pFrontendHandle - Frontend handle.
 * @param [out] peToneMode - DiSEqC tone mode.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_GetToneMode(void *pFrontendHandle,
                                           tTDAL_DISEQC_TONE_MODE *peToneMode);

/**
 * @brief Function sets DiSEqC message framing override.
 *
 * @param [in] pFrontendHandle - Frontend handle.
 * @param [in] eFraming - DiSEqC message framing override.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_SetFraming(void *pFrontendHandle,
                                          tTDAL_DISEQC_FRAMING eFraming);

/**
 * @brief Function gets DiSEqC message framing override.
 *
 * @param [in]  pFrontendHandle - Frontend handle.
 * @param [out] peFraming - DiSEqC message framing override.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_GetFraming(void *pFrontendHandle,
                                          tTDAL_DISEQC_FRAMING *peFraming);

/**
 * @brief Function sets DiSEqC pre-transmit delay in milliseconds.
 *
 * @param [in] pFrontendHandle - Frontend handle.
 * @param [in] ui32PreTransmitDelay - DiSEqC pre-transmit delay in milliseconds.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_SetPreTransmitDelay(void *pFrontendHandle,
                                                   uint32_t ui32PreTransmitDelay);

/**
 * @brief Function gets DiSEqC pre-transmit delay in milliseconds.
 *
 * @param [in]  pFrontendHandle - Frontend handle.
 * @param [out] pui32PreTransmitDelay - DiSEqC pre-transmit delay in milliseconds.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_GetPreTransmitDelay(void *pFrontendHandle,
                                                   uint32_t *pui32PreTransmitDelay);

/**
 * @brief Function sets all DiSEqC settings.
 *
 * @param [in] pFrontendHandle - Frontend handle.
 * @param [in] pSettings - DiSEqC Settings.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_SetAllSettings(void *pFrontendHandle,
                                              tTDAL_DISEQC_Settings *pSettings);

/**
 * @brief Function gets all DiSEqC settings.
 *
 * @param [in] pFrontendHandle - Frontend handle.
 * @param [out] pSettings - DiSEqC Settings.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_GetAllSettings(void *pFrontendHandle,
                                              tTDAL_DISEQC_Settings *pSettings);

/**
 * @brief Function gets DiSEqC status.
 *
 * @param [in]  pFrontendHandle - Frontend handle.
 * @param [out] pStatus - DiSEqC Status.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_GetStatus(void *pFrontendHandle,
                                         tTDAL_DISEQC_Status *pStatus);

/**
 * @brief Function sends a DiSEqC message.
 *
 * @param [in] pFrontendHandle - Frontend handle.
 * @param [in] pMessage - DiSEqC Message.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_SendMessage(void *pFrontendHandle,
                                           tTDAL_DISEQC_Message *pMessage);

/**
 * @brief Function resets DiSEqC.
 *
 * @param [in] pFrontendHandle - Frontend handle.
 * @param [in] ui8Options - Options.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_Reset(void *pFrontendHandle,
                                     uint8_t ui8Options);

/**
 * @brief Function reads a DiSEqC message reply.
 *        This function will read up to the number of bytes specified from a
 *        DiSEqC message reply. Any remaining bytes will be discarded.
 *
 * @param [in]  pFrontendHandle - Frontend handle.
 * @param [out] pReply - DiSEqC reply.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_GetReply(void *pFrontendHandle,
                                        tTDAL_DISEQC_Reply *pReply);

/**
 * @brief Function sends DiSEqc Auto Control Word.
 *
 * @param [in] pFrontendHandle - Frontend handle.
 * @param [in] ui8CodeWord - DiSEqC auto control word.
 *
 * @return TDAL_DISEQC_NO_ERROR : No Error.
 */
tTDAL_DISEQC_Error TDAL_DISEQC_SendAcw(void *pFrontendHandle,
                                       uint8_t ui8CodeWord);

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_DISEQC_H_*/
