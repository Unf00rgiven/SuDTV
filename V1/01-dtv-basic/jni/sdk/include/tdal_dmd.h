/******************************************************************************
 *                          COPYRIGHT 2004 IWEDIA TECHNOLOGIES                *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DMD
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/dev/sources/chal/core/tdal/trunk/inc/tdal_dmd.h $
 *            $Rev: 12362 $
 *            $Date: 2014-01-24 18:20:37 +0100 (Fri, 24 Jan 2014) $
 *
 * Description			: Front-End TDAL library primitives header.
 * Target				: All
 * Author			    : Xavier RAZAVET.
 * Date	of creation		: 18 / 02 /	2004.
 *
 *****************************************************************************/

#ifndef _TDAL_DMD_H_
#define _TDAL_DMD_H_


/*******************************************************/
/*              Includes                               */
/*******************************************************/
#include "tdal_diseqc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Defines                                */
/*******************************************************/
#define SATIP_STRING_LEN 250
#define TDAL_DMD_PHYSICAL_FRONTEND_MAX_NUM 8
#define TDAL_DMD_MAX_ISDB_LAYERS_NUM 3

/* Frequencies band boundaries */
/* --------------------------- */
#define eTDAL_DMD_C_BAND_START         3000000 /*kHZ*/
#define eTDAL_DMD_C_BAND_END           4200000 /*kHZ*/
#define eTDAL_DMD_KU_LOW_BAND_START   10700000 /*kHZ*/
#define eTDAL_DMD_KU_LOW_BAND_END     11725000 /*kHZ*/ // ognjen changed, comedia aligment (11700000)
#define eTDAL_DMD_KU_HIGH_BAND_START  11725001 /*kHZ*/ // ognjen changed, comedia aligment (11700001)
#define eTDAL_DMD_KU_HIGH_BAND_END    12750000 /*kHZ*/

/*******************************************************/
/*              Macros                                 */
/*******************************************************/

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/

typedef enum
{
    eTDAL_DMD_NO_ERROR = 0,
    eTDAL_DMD_BAD_DEMODULATOR,
    eTDAL_DMD_BAD_PARAMETER,
    eTDAL_DMD_DEVICE_BUSY,
    eTDAL_DMD_NOT_SCANNING,
    eTDAL_DMD_NOT_LOCKED,
    eTDAL_DMD_ERROR
}tTDAL_DMD_Error;

typedef enum
{
eTDAL_DMD_FE_0 = 0x00000000,
eTDAL_DMD_FE_1 = 0x00000001,
eTDAL_DMD_FE_2 = 0x00000002,
eTDAL_DMD_FE_3 = 0x00000003,
eTDAL_DMD_FE_4 = 0x00000004,
eTDAL_DMD_FE_5 = 0x00000005,
eTDAL_DMD_FE_6 = 0x00000006,
eTDAL_DMD_FE_7 = 0x00000007,
eTDAL_DMD_NB_MAX_FE_ID
} tTDAL_DMD_FE;

typedef struct
{
    uint8_t                uiFENumber;
} tTDAL_DMD_Capability;

typedef enum
{
    eTDAL_DMD_TYPE_QPSK = 0x00000001,
    eTDAL_DMD_TYPE_QAM	= 0x00000002,
    eTDAL_DMD_TYPE_OFDM = 0x00000004,
    eTDAL_DMD_TYPE_IP 	  = 0x00000010,
    eTDAL_DMD_TYPE_QPSK_IP = 0x00000020,
    eTDAL_DMD_TYPE_QAM_IP  = 0x00000040,
    eTDAL_DMD_TYPE_OFDM_IP = 0x00000080
} tTDAL_DMD_Type;

typedef enum
{
    eTDAL_DMD_DIGITAL,
    eTDAL_DMD_ANALOG,
    eTDAL_DMD_HYBRID
}tTDAL_DMD_SourceSupport;  

typedef struct
{
    uint32_t              eType;
    tTDAL_DMD_SourceSupport     eSourceSupport;
#ifdef USE_MULTI_PLATFORM_SUPPORT
    uint8_t                     platformId;
#endif
} tTDAL_DMD_FECapability;

typedef enum
{
    eTDAL_DMD_LOCKED = 0,
    eTDAL_DMD_LOCK_FAILED,
    eTDAL_DMD_FOUND,
    eTDAL_DMD_END_OF_RANGE,
    eTDAL_DMD_SIGNAL_LOST,
    eTDAL_DMD_SIGNAL_RETURN,
    eTDAL_DMD_SATIP_SERVER_DROPPED,
    eTDAL_DMD_SATIP_SERVER_LIST_UPDATE
} tTDAL_DMD_NotifyStatus;


typedef enum
{
	eTDAL_DMD_RF0,
	eTDAL_DMD_RF1,
	eTDAL_DMD_RF_MAX
} tTDAL_DMD_RF;

/*---------------------------------------*/
/*  COMMON Front-End types Declarations  */
/*---------------------------------------*/
typedef enum
{
    eTDAL_DMD_MOD_BPSK = 0,
    eTDAL_DMD_MOD_QPSK,
    eTDAL_DMD_MOD_QAM4 = eTDAL_DMD_MOD_QPSK,
    eTDAL_DMD_MOD_8PSK,
    eTDAL_DMD_MOD_QAM16,
    eTDAL_DMD_MOD_QAM32,
    eTDAL_DMD_MOD_QAM64,
    eTDAL_DMD_MOD_QAM128,
    eTDAL_DMD_MOD_QAM256,
    eTDAL_DMD_MOD_AUTO,
    eTDAL_DMD_MOD_MAX
} tTDAL_DMD_Modulation;

typedef enum
{
    eTDAL_DMD_FEC_NONE,
    eTDAL_DMD_FEC_1_2,
    eTDAL_DMD_FEC_2_3,          /* only used for VSB (US market) */
    eTDAL_DMD_FEC_3_4,
    eTDAL_DMD_FEC_4_5,          /* not used normally in satellite and terrestrial */
    eTDAL_DMD_FEC_5_6,
    eTDAL_DMD_FEC_6_7,          /* not used normally in terrestrial */
    eTDAL_DMD_FEC_7_8,
    eTDAL_DMD_FEC_8_9,          /* not used normally in satellite and terrestrial */
    eTDAL_DMD_FEC_AUTO,
    eTDAL_DMD_FEC_MAX
}tTDAL_DMD_Convolution;

// dimitrije - aligned with apilib values
typedef enum
{
    eTDAL_DMD_SPECTRUM_AUTO = 0,
    eTDAL_DMD_SPECTRUM_NORMAL,
    eTDAL_DMD_SPECTRUM_INVERTED,
    eTDAL_DMD_SPECTRUM_MAX
} tTDAL_DMD_Spectrum;

typedef enum
{
	eTDAL_DMD_BANDWIDTH_8MHz = 8000000,
    eTDAL_DMD_BANDWIDTH_7MHz = 7000000,
    eTDAL_DMD_BANDWIDTH_6MHz = 6000000,
    eTDAL_DMD_BANDWIDTH_5MHz = 5000000,
    eTDAL_DMD_BANDWIDTH_MAX
} tTDAL_DMD_Bandwidth;
/*------------------------------------------*/
/*  SATTELITE Front-End types Declarations  */
/*------------------------------------------*/
typedef enum
{
    eTDAL_DMD_POLAR_HORIZONTAL,
    eTDAL_DMD_POLAR_VERTICAL,
    eTDAL_DMD_POLAR_CIRCULAR_LEFT,      /* not implemented */
    eTDAL_DMD_POLAR_CIRCULAR_RIGHT,     /* not implemented */
    eTDAL_DMD_POLAR_ALL                 /* not implemented */
} tTDAL_DMD_LNB_Polarization;

typedef enum
{
	eTDAL_DMD_PILOT_OFF,
	eTDAL_DMD_PILOT_ON,
	eTDAL_DMD_PILOT_UNKNOWN
}tTDAL_DMD_Pilot;

typedef enum
{
	eTDAL_DMD_ROLL_OFF_20,
	eTDAL_DMD_ROLL_OFF_25,
	eTDAL_DMD_ROLL_OFF_35,
	eTDAL_DMD_ROLL_OFF_UNKNOWN
}tTDAL_DMD_RollOff;

typedef	enum
{
	eTDAL_DMD_SCAN			= 0,
	eTDAL_DMD_TUNE_ZAPPING
}tTDAL_DMD_TuneMode;

typedef struct
{
    uint32_t                    SymbolRateBw;
    tTDAL_DMD_Modulation        Modulation;
    tTDAL_DMD_Spectrum          SpecInv;
    tTDAL_DMD_Convolution       Convolution;
    tTDAL_DMD_LNB_Polarization  Polarization;
    tTDAL_DMD_Pilot             Pilot;
    tTDAL_DMD_RollOff           RollOff;
    char                        SatelliteName[100];
    tTDAL_DMD_TuneMode          TuneMode;
}tTDAL_DMD_SatTunData;

typedef enum
{
    eTDAL_DMD_CFG_SAT_INIT,
    eTDAL_DMD_CFG_SAT_OL_BAND,
    eTDAL_DMD_CFG_STAND_BY_MODE,
    eTDAL_DMD_CFG_SAT_ODU_TONE_STATE,
    eTDAL_DMD_CFG_SAT_ODU_POWER,
    eTDAL_DMD_CFG_SAT_ODU_TONE_BURST,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_UNICABLE,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_SET_SETTINGS,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_GET_SETTINGS,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_ENABLE_LNB,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_IS_LNB_ENABLED,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_ENABLE_TONE,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_IS_TONE_ENABLED,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_SET_VOLTAGE,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_GET_VOLTAGE,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_SET_TONE_BURST,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_GET_TONE_BURST,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_SET_TONE_MODE,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_GET_TONE_MODE,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_SET_FRAMING,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_GET_FRAMING,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_SET_PRETRANSMITION_DELAY,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_GET_PRETRANSIMTION_DELAY,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_RESET,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_ACW,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_STATUS,
    eTDAL_DMD_CFG_SAT_ODU_DISEQC_REPLY,
    eTDAL_DMD_CFG_SAT_OTHER
}tTDAL_DMD_SatConfigType;

typedef enum
{
    eTDAL_DMD_BANDTYPE_KU = 0,
    eTDAL_DMD_BANDTYPE_C,
    eTDAL_DMD_BANDTYPE_L
}tTDAL_DMD_BandType;

typedef enum
{
    eTDAL_DMD_LNB_POWER_13V,
    eTDAL_DMD_LNB_POWER_18V
} tTDAL_DMD_LNB_Power;

typedef struct
{
    uint32_t            SwitchFreq;         /* usually 22000Hz */
    tTDAL_DMD_LNB_Power Vertical;           /* usually 13V */
    tTDAL_DMD_LNB_Power Horizontal;         /* usually 18V */
    uint32_t*           pSymbolRateList;
} tTDAL_DMD_SatInitData;

typedef enum
{
    eTDAL_DMD_STANDBYMODE_OFF,
    eTDAL_DMD_STANDBYMODE_ON
} tTDAL_DMD_CFG_StandByMode;

typedef struct
{
    tTDAL_DMD_BandType  BandType;
    uint32_t            LowLNBFreq;
    uint32_t            HighLNBFreq;
} tTDAL_DMD_SatOlBand;

typedef enum
{
    eTDAL_SatelliteA,
    eTDAL_SatelliteB
}tTDAL_DMD_ODU_ToneBurst;

typedef     struct
{
    uint8_t  Frm;
    uint8_t  Add;
    uint8_t  Cmd;
    uint8_t  Data0;
    uint8_t  Data1;
    uint8_t  Data2;
}tTDAL_DMD_ODU_DiSEqCFrame;

typedef     struct
{
    uint8_t                   NbByte;
    tTDAL_DMD_ODU_DiSEqCFrame Frame;
}tTDAL_DMD_ODU_DiSEqC;

typedef enum
{
    eTDAL_DMD_ODU_TONE_NONE,
    eTDAL_DMD_ODU_TONE_OFF,
    eTDAL_DMD_ODU_TONE_ON
}tTDAL_DMD_ODU_ToneState;


typedef enum
{
    eTDAL_DMD_ODU_POWER_NONE,
    eTDAL_DMD_ODU_POWER_13V,
    eTDAL_DMD_ODU_POWER_18V
}tTDAL_DMD_ODU_Power;

typedef struct
{
    tTDAL_DMD_SatConfigType         ConfigType;
    union
    {
        tTDAL_DMD_SatInitData       Init;
        tTDAL_DMD_SatOlBand         OlBand;
        tTDAL_DMD_CFG_StandByMode   StandByMode;
        tTDAL_DMD_ODU_ToneBurst     ToneBurst;
        tTDAL_DMD_ODU_DiSEqC        DiSEqC;
        tTDAL_DISEQC_Settings        DiSEqCSettings;
        uint8_t                      DiSEqCResetOptions;
        uint8_t                      DiSEqCCodeWord;
        tTDAL_DISEQC_Status          DiSEqCStatus;
        tTDAL_DISEQC_Reply           DiSEqCReply;
        tTDAL_DMD_ODU_ToneState     ToneState;
        tTDAL_DMD_ODU_Power         Power;
        uint32_t*                   pOther;
    }uCfg;
}tTDAL_DMD_SatCfgData;

/*------------------------------------------*/
/*    CABLE Front-End types Declarations    */
/*------------------------------------------*/
typedef struct
{
    uint32_t                    SymbolRateBw;
    tTDAL_DMD_Modulation        Modulation;
    tTDAL_DMD_Spectrum          SpecInv;
}tTDAL_DMD_CabTunData;

typedef enum
{
    eTDAL_DMD_CFG_CAB_INIT,
    eTDAL_DMD_CFG_CAB_OTHER
}tTDAL_DMD_CabConfigType;

typedef struct
{
    uint32_t*   pSymbolRateList;
}tTDAL_DMD_CabInitData;

typedef struct
{
    tTDAL_DMD_CabConfigType     ConfigType;
    union
    {
        tTDAL_DMD_CabInitData   Init;
        uint32_t*               pOther;
    }uCfg;
} tTDAL_DMD_CabCfgData;

/*------------------------------------------*/
/* TERRESTRIAL Front-End types Declarations */
/*------------------------------------------*/
typedef enum
{
    eTDAL_DMD_CFG_SOURCE_IS_ATV,
    eTDAL_DMD_CFG_SOURCE_IS_DTV
}tTDAL_DMD_TerSourceState;

typedef enum
{
    eTDAL_DMD_MODE_2K,
    eTDAL_DMD_MODE_8K,
    eTDAL_DMD_MODE_AUTO,
    eTDAL_DMD_MODE_MAX
}tTDAL_DMD_TerMode;

typedef enum
{
    eTDAL_DMD_GUARD_1_32,
    eTDAL_DMD_GUARD_1_16,
    eTDAL_DMD_GUARD_1_8,
    eTDAL_DMD_GUARD_1_4,
    eTDAL_DMD_GUARD_AUTO,
    eTDAL_DMD_GUARD_MAX
}tTDAL_DMD_TerGuard;

typedef enum
{
    eTDAL_DMD_HIER_NONE,
    eTDAL_DMD_HIER_1,
    eTDAL_DMD_HIER_2,
    eTDAL_DMD_HIER_4,
    eTDAL_DMD_HIER_AUTO,
    eTDAL_DMD_HIER_MAX
}tTDAL_DMD_TerHierarchy;

typedef enum
{
    eTDAL_DMD_PRIORITY_STREAM_HIGH,
    eTDAL_DMD_PRIORITY_STREAM_LOW,
    eTDAL_DMD_PRIORITY_STREAM_MAX
}tTDAL_DMD_TerPrioStream;


typedef enum
{
    eTDAL_DMD_OFFSET_NONE,
    eTDAL_DMD_OFFSET_1_6_MORE,      /* + 1/6 Mhz */
    eTDAL_DMD_OFFSET_1_6_MINUS,     /* - 1/6 Mhz */
    eTDAL_DMD_OFFSET_2_6_MORE,      /* + 2/6 Mhz */
    eTDAL_DMD_OFFSET_2_6_MINUS,     /* - 2/6 Mhz */
    eTDAL_DMD_OFFSET_3_6_MORE,      /* + 3/6 Mhz */
    eTDAL_DMD_OFFSET_3_6_MINUS,     /* - 3/6 Mhz */
    eTDAL_DMD_OFFSET_AUTO,
    eTDAL_DMD_OFFSET_MAX
}tTDAL_DMD_TerFreqOff;

#ifdef STORE_DVBT_SYSTEM
typedef enum
{
    eTDAL_DMD_TER_SYSTEM_UNKNOWN,
    eTDAL_DMD_TER_SYSTEM_DVBT,
    eTDAL_DMD_TER_SYSTEM_DVBT2,
    eTDAL_DMD_TER_SYSTEM_MAX
} tTDAL_DMD_TerSystem;
#endif

typedef enum
{
    eTDAL_DMD_STANDARD_UNDEFINED  = 0x00,
    eTDAL_DMD_STANDARD_DVBT       = 0x01,
    eTDAL_DMD_STANDARD_DVBT2      = 0x02,
    eTDAL_DMD_STANDARD_ISDBT      = 0x04,
    eTDAL_DMD_STANDARD_ISDBTB     = 0x08,
    eTDAL_DMD_STANDARD_ATSC       = 0x10,
    eTDAL_DMD_STANDARD_DTMB       = 0x20,
    eTDAL_DMD_STANDARD_ISDBTSB    = 0x40
}tTDAL_DMD_TerStandard;

typedef enum
{
    eTDAL_DMD_INTLV_NATIVE_OFF,
    eTDAL_DMD_INTLV_NATIVE_ON,
    eTDAL_DMD_INTLV_NATIVE_AUTO
}tTDAL_DMD_TerIntlvNative;

typedef enum
{
    eTDAL_DMD_VIT_ALPHA_1,
    eTDAL_DMD_VIT_ALPHA_2,
    eTDAL_DMD_VIT_ALPHA_4,
    eTDAL_DMD_VIT_ALPHA_AUTO
}tTDAL_DMD_TerVitAlpha;

typedef struct
{
    tTDAL_DMD_TerStandard       Standard;
    uint8_t                     IsSignalModulated;
    uint32_t                    SymbolRateBw;
    tTDAL_DMD_Spectrum          SpecInv;
    tTDAL_DMD_TerMode           Mode;
    tTDAL_DMD_TerGuard          Guard;
    tTDAL_DMD_Modulation        Modulation;
    tTDAL_DMD_TerIntlvNative    IntlvNative;
    tTDAL_DMD_TerHierarchy      Hierarchy;
    tTDAL_DMD_TerPrioStream     Priority;
    tTDAL_DMD_TerVitAlpha       VitAlpha;
    tTDAL_DMD_Convolution       ConvolutionHP;
    tTDAL_DMD_Convolution       ConvolutionLP;
    tTDAL_DMD_TerFreqOff        Offset;
#ifdef ENABLE_DVBT2_SUPPORT
    uint16_t                    NumDataSymbols;
    int32_t                     PlpIdx;
    int32_t                     PlpNB;
    struct {
        int32_t                 PlpId;
        tTDAL_DMD_TerMode       PlpMode;
        tTDAL_DMD_Convolution   PlpConvolution;
        tTDAL_DMD_Modulation    PlpConstellation;
        uint8_t                 FrameInterval;
        uint8_t                 TimeIlType;
        uint8_t                 TimeIlLength;
    } plpInfo[TDAL_DMD_PLP_NUM];
#endif /* ENABLE_DVBT2_SUPPORT */

#ifdef ENABLE_ISDBT_SUPPORT
    uint8_t                     SbMode;             /**< 0 = 13seg, 1 = 1 or 3seg */
    uint8_t                     PartialReception;   /**< sb_mode=0 -> 0=no, 1=yes, sb_mode=1 -> 0=1seg, 1=3seg */
    uint8_t                     SbConnTotalSeg;
    uint8_t                     SbWantedSeg;
    struct {
        tTDAL_DMD_Modulation    Constellation;
        tTDAL_DMD_Convolution   CodeRate;
        uint8_t                 NbSegments;
        uint8_t                 TimeIntlv;
    } layer[3];
#endif /* ENABLE_ISDBT_SUPPORT */
#ifdef HD_BOOK
    tTDAL_DMD_TuneMode          TuneMode;
#endif
#ifdef STORE_DVBT_SYSTEM
    tTDAL_DMD_TerSystem            TuneSystem;
#endif
}tTDAL_DMD_TerTunData;


typedef enum
{
    eTDAL_DMD_CFG_TER_INIT,
    eTDAL_DMD_CFG_TER_OTHER,
    eTDAL_DMD_CFG_TER_ANTENNA_POWER_STATUS,
    eTDAL_DMD_CFG_TER_SOURCE_TRANSITION,
    eTDAL_DMD_CFG_TER_STAND_BY_MODE
}tTDAL_DMD_TerConfigType;


typedef struct
{
    uint32_t*   pBandWidthList;
}tTDAL_DMD_TerInitData;



typedef struct
{
    tTDAL_DMD_TerConfigType         ConfigType;
    union
    {
        tTDAL_DMD_TerInitData       Init;
        uint32_t*                   pOther;
        uint8_t                     bAntennaPowerStatus;
        tTDAL_DMD_TerSourceState    SourceTransition;
        tTDAL_DMD_CFG_StandByMode   StandByMode;
    }uCfg;
}tTDAL_DMD_TerCfgData;

typedef struct
{
    uint8_t physicalFeID;
    int32_t signalStrength;
}tTDAL_DMD_FeSignalInfo;

typedef struct
{
    tTDAL_DMD_FE logicalFeID;
    uint32_t frequency;
    tTDAL_DMD_TerStandard standard;

    union
    {
        struct
        {
            uint8_t signalQuality;
            float   bitErrorRate;
            float   SNR;
            int32_t packetErrorRate;
            uint8_t signalStrengthPercentage;
        }DVBT;
        struct
        {
            uint8_t plpId;
            uint8_t signalQuality;
            float   bitErrorRate;
            float   SNR;
            int32_t packetErrorRate;
            uint8_t signalStrengthPercentage;
        }DVBT2;
        struct
        {
            uint8_t layerId;
            uint8_t segmentNumber;
            uint8_t signalQuality;
            float   bitErrorRate;
            float   SNR;
            int32_t packetErrorRate;
            uint8_t signalStrengthPercentage;
        }ISDB[TDAL_DMD_MAX_ISDB_LAYERS_NUM];
    }signalInfo;

    uint8_t physicalFeNum;
    tTDAL_DMD_FeSignalInfo feSignalInfo[TDAL_DMD_PHYSICAL_FRONTEND_MAX_NUM];
}tTDAL_DMD_MonitoringInfo;

typedef struct
{
    uint8_t tunerId;                      /* Tuner ID */
    uint32_t frequency;                   /* Frequency */
    tTDAL_DMD_TerStandard dtvStandard;    /* DTV standard */
    uint8_t isSignalModulated;            /* Is Signal Modulated */
    tTDAL_DMD_Convolution codeRate;       /* Code rate */
    tTDAL_DMD_Modulation constellation;   /* Constellation */
    union
    {
        struct {
            double   BER;               /* Bit error rate */
            uint16_t locks;             /* Lock bitmask field */
            uint16_t PER;               /* Packet error rate */
            double   combinedCn;        /* Combined C/N */
        }DVB;
        struct {
            uint8_t plpId;              /* Physical layer pipe id */
            uint8_t ldpcIter;           /* LDPC number of iterations */
            uint32_t PE;                /* BCH and CRC data incorrect indicator */
            uint16_t PC;                /* BCH data */
            uint16_t locks;             /* Lock bitmask field */
            double   PER;               /* Packet error rate */
            double   combinedCn;        /* Combined C/N */
        }DVBT2;
        struct {
            uint8_t layerId;            /* Transmission layer id */
            double   BER;               /* Bit error rate */
            uint16_t locks;             /* Lock bitmask field */
            uint16_t PER;               /* Packet error rate */
            double   combinedCn;        /* Combined C/N */
        }ISDB;
    }standardParameters;
    uint8_t signalStrengthNumber;                                   /* Signal strength number */
    int32_t signalStrength[TDAL_DMD_PHYSICAL_FRONTEND_MAX_NUM];     /* Signal strength */
}tTDAL_DMD_SignalParameters;

typedef struct
{
    uint8_t signalStrength; /* Signal strength expressed in percentage */
    uint8_t signalQuality;  /* Signal quality expressed in percentage */
}tTDAL_DMD_SignalDesc;

/*-------------------------------------------------*/
/* TERRESTRIAL ANALOG Front-End types Declarations */
/*-------------------------------------------------*/
typedef enum
{
    eTDAL_DMD_ANALOG_STD_PAL_BG         = 0x00000001,  /* Picture carrier : 39.875 MHz / Sound Carrier : 34.375 MHz*/
    eTDAL_DMD_ANALOG_STD_PAL_I          = 0x00000002,  /* Picture carrier : 39.875 MHz / Sound Carrier : 33.875 MHz*/
    eTDAL_DMD_ANALOG_STD_PAL_DK         = 0x00000004,  /* Picture carrier : 39.875 MHz / Sound Carrier : 33.375 MHz*/
    eTDAL_DMD_ANALOG_STD_SECAM_L        = 0x00000008,  /* Picture carrier : 39.875 MHz / Sound Carrier : 33.375 MHz*/
    eTDAL_DMD_ANALOG_STD_SECAM_L_prime  = 0x00000010,  /* Picture carrier : 33.625 MHz / Sound Carrier : 40.125 MHz*/
    eTDAL_DMD_ANALOG_STD_SECAM_BG       = 0x00000020,  /* ? */
    eTDAL_DMD_ANALOG_STD_SECAM_DK       = 0x00000040  /* ? */
}tTDAL_DMD_AnalogStandardSystem;

typedef struct
{   
    uint32_t                            SymbolRateBw;    
    tTDAL_DMD_AnalogStandardSystem      eStandardSystem;            /* Current standard */
    uint8_t                             bEnableDriftMonitoring;     /* Enable/Disable the automatic drift detection */
    int32_t                             lManualDriftToApply;        /* In Hz _ Not used if bEnableDriftMonitoring = true */
}tTDAL_DMD_TerAnalogTunData; 

/*-------------------------------------------------*/
/* S2 Front-End types Declarations */
/*-------------------------------------------------*/
struct S2_property
{
         uint32_t cmd;
         uint32_t reserved[3];
         union {
                 uint32_t data;
                 struct {
                         unsigned char data[32];
                         uint32_t len;
                         uint32_t reserved1[3];
                         void *reserved2;
                 } buffer;
         } u;
         int result;
} __attribute__ ((packed));

 /* num of properties cannot exceed DTV_IOCTL_MAX_MSGS per ioctl */
 #define DTV_IOCTL_MAX_MSGS 64

typedef struct
{
         uint32_t num;
         struct S2_property *props;
         tTDAL_DMD_LNB_Polarization polarisation;
         unsigned int switch_pos; /* {0,1,2,3} */
}tTDAL_DMD_DVBS2TunData;


/* Miladin */
/*------------------------------------------*/
/* IP		   Front-End types Declarations */
/*------------------------------------------*/
typedef enum
{
	eTDAL_DMD_IP_SOURCE_HTTP,
	eTDAL_DMD_IP_SOURCE_RTP
}tTDAL_DMD_IpSource;

typedef enum
{
	eTDAL_DMD_IP_PROTOCOL_UDP,
	eTDAL_DMD_IP_PROTOCOL_TCP
}tTDAL_DMD_IpProtocol;

typedef enum
{
    eTDAL_DMD_CFG_IP_INIT,
    eTDAL_DMD_CFG_IP_OTHER
}tTDAL_DMD_IpConfigType;

typedef enum
{
    eTDAL_DMD_TS = 0,
    eTDAL_DMD_TLV
} tTDAL_DMD_TransportType;

typedef struct
{
	tTDAL_DMD_IpSource		Source;
	tTDAL_DMD_IpProtocol	Protocol;
	uint8_t					*pOther;
}tTDAL_DMD_IpTunData;

typedef struct
{
	tTDAL_DMD_IpConfigType	ConfigType;
	uint8_t					*pOther;
}tTDAL_DMD_IpCfgData;

typedef struct
{
    uint32_t     fecMode;
    uint32_t     fecL;
    uint32_t     fecD;
    char*        licenseId;
    char*        tierBitMask;
    uint32_t     serviceType;
    uint8_t      retransmissionType;
    uint8_t      remoteControlKeyID;
    char*        keyPuId;
    char*        contractCrid;
    char*        uncontractCrid;
    char*        linkageDescriptorUrl;
    uint32_t     renderingObligation;
    uint32_t     renderingObligationRa;
}Iptvfj_t;

typedef struct
{
    //Add other specific data here
    Iptvfj_t      iptvfj;

}tTDAL_DMD_IpServiceExtendedInfo;

#ifdef USE_IP_SERVICE_INSTALLATION
typedef struct
{
    char*       psServiceName;
    char*       psServiceURL;
    uint32_t    uiLCN;
    uint32_t    uiONID;
    uint32_t    uiTSID;
    uint32_t    uiSID;
    tTDAL_DMD_TransportType eTransportType;
    
    tTDAL_DMD_IpServiceExtendedInfo      extInfo;

} tTDAL_DMD_IpServiceInfo;
#endif /* USE_IP_SERVICE_INSTALLATION */

typedef struct
{
    char*       psServiceName;
    char*       psServiceURL;
    uint32_t    iChannelNumber;
    uint32_t    uiONID;
    uint32_t    uiTSID;
    uint32_t    uiSID;
    tTDAL_DMD_TransportType eTransportType;

    tTDAL_DMD_IpServiceExtendedInfo      extInfo;
} ServiceInfo_t;

/*---------------------------------------*/
/*  COMMON Front-End types Declarations  */
/*---------------------------------------*/
typedef union
{
    tTDAL_DMD_SatTunData        Sat;
    tTDAL_DMD_CabTunData        Cab;
    tTDAL_DMD_TerTunData        Ter;
    tTDAL_DMD_TerAnalogTunData  AnalogTer;
    tTDAL_DMD_IpTunData			Ip; /* Miladin */
	tTDAL_DMD_DVBS2TunData DVBS2;
}tTDAL_DMD_TunData;

typedef  tTDAL_DMD_TunData   tTDAL_DMD_ScanData;

typedef struct
{    
    tTDAL_DMD_TerSourceState    eSourceState;
    uint32_t                    TunFrequency;
    tTDAL_DMD_TunData           TunData;
    tTDAL_DMD_NotifyStatus      CarrierStatus;
    uint8_t                     SignalQuality;
    uint32_t                    SignalBER;
    uint8_t                     SignalLevel;
    uint8_t                     SignalStrength;
}tTDAL_DMD_Info;

typedef union
{
    tTDAL_DMD_SatCfgData        Sat;
    tTDAL_DMD_CabCfgData        Cab;
    tTDAL_DMD_TerCfgData        Ter;
    tTDAL_DMD_IpCfgData			Ip; /* Miladin */
}tTDAL_DMD_ConfigData;

/* FE wrapper */
typedef struct
{
    uint32_t                    FeIndex; /* Real FE index (from existing CHAL implementations) - maped from tTDAL_DMD_FE */
    tTDAL_DMD_Type              eFeType;
}tTDAL_DMD_FE_MappingEntry;
/* FE wrapper */

typedef struct
{
	char friendlyName[SATIP_STRING_LEN];
	char udn[SATIP_STRING_LEN];
}tTDAL_DMD_SatipServerDesc;

typedef struct
{
    int bit_rate;                  // Representation bit rate.
    int width;                     // Video width.
    int height;                    // Video height.
    float frame_rate;              // Video frame rate
    char codec[20];                // Representation codec.
    int libav_codec_id;
}tTDAL_DMD_Representation;

/*------------------------------------------*/
/*   CALL BACK Definition                   */
/*------------------------------------------*/
typedef void (*tTDAL_DMD_NotifyStatusFct)   (tTDAL_DMD_FE ucFeID,
                                             tTDAL_DMD_NotifyStatus  Status);

typedef uint8_t tTDAL_DMD_NotifyProgress;   /* percent value */
typedef void (*tTDAL_DMD_NotifyProgressFct) (tTDAL_DMD_FE ucFeID,
                                             tTDAL_DMD_NotifyProgress  Progress);
typedef void (*tTDAL_DMD_MonitoringFct) (tTDAL_DMD_MonitoringInfo* monitoringInfo);
typedef void (*tTDAL_DMD_CalculateSignal)(tTDAL_DMD_SignalParameters* signalParameters,
        tTDAL_DMD_SignalDesc* signalDesc);

/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/
tTDAL_DMD_Error TDAL_DMD_Init               (void);

tTDAL_DMD_Error TDAL_DMD_Terminate          (void);

const char *    TDAL_DMD_APIRevisionGet     (void);

const char *    TDAL_DMD_PlatformRevisionGet(void);

tTDAL_DMD_Error TDAL_DMD_GetCapability      (tTDAL_DMD_Capability           *pstCapability);

tTDAL_DMD_Error TDAL_DMD_GetFECapability    (tTDAL_DMD_FE eFeID,
                                             tTDAL_DMD_FECapability         *pstFECapability);

tTDAL_DMD_Error TDAL_DMD_GetCurentFEConfiguration (tTDAL_DMD_FE eFeID, tTDAL_DMD_Type *pstFEType);

tTDAL_DMD_Error TDAL_DMD_SetCurentFEConfiguration (tTDAL_DMD_FE eFeID, tTDAL_DMD_Type pstFEType);

tTDAL_DMD_Error TDAL_DMD_OpenFEInstance     (tTDAL_DMD_FE                   eFeID,
                                             tTDAL_DMD_NotifyStatusFct      pNotifyStatusFct,
                                             tTDAL_DMD_NotifyProgressFct    pNotifyProgressFct);

tTDAL_DMD_Error TDAL_DMD_CloseFEInstance    (tTDAL_DMD_FE                   eFeID);

tTDAL_DMD_Error TDAL_DMD_HandleGet          (tTDAL_DMD_FE                   eFeID, 
                                             void                           *pHandle);

tTDAL_DMD_Error TDAL_DMD_Config             (tTDAL_DMD_FE                   eFeID,
                                             tTDAL_DMD_ConfigData           *psCfgData);

tTDAL_DMD_Error TDAL_DMD_Tune               (tTDAL_DMD_FE                   eFeID,
                                             tTDAL_DMD_TunData              *psTunData,
                                             uint32_t                       uiFrequency);

tTDAL_DMD_Error TDAL_DMD_Unlock             (tTDAL_DMD_FE                   eFeID);


tTDAL_DMD_Error TDAL_DMD_StartScan          (tTDAL_DMD_FE                   eFeID,
                                             tTDAL_DMD_ScanData             *psScanData,
                                             uint32_t                       uiStartFreq,
                                             uint32_t                       uiEndFreq);

tTDAL_DMD_Error TDAL_DMD_ContScan           (tTDAL_DMD_FE                   eFeID);


tTDAL_DMD_Error TDAL_DMD_StopScan           (tTDAL_DMD_FE                   eFeID);

tTDAL_DMD_Error TDAL_DMD_GetInfo            (tTDAL_DMD_FE                   eFeID,
                                             tTDAL_DMD_Info                 *psInfo);

tTDAL_DMD_Error TDAL_DMD_GetTSReliability   (tTDAL_DMD_FE                   eFeID,
                                             tTDAL_DMD_Info                 *psInfo,
                                             uint8_t                        *pTSReliability, 
                                             uint32_t                       *pQuo, 
                                             uint32_t                       *pRem, 
                                             int32_t                        *pExp);

tTDAL_DMD_Error TDAL_DMD_GetNumberOfSatipServers(uint32_t *serverNum);
tTDAL_DMD_Error TDAL_DMD_GetSatipServerDescByIndex(uint32_t serverIndex, tTDAL_DMD_SatipServerDesc *serverDesc);
tTDAL_DMD_Error TDAL_DMD_SelectSatipServer(char* udn);
tTDAL_DMD_Error TDAL_DMD_GetSelectedSatipServer(char* udn);
tTDAL_DMD_Error TDAL_DMD_GetSatipDiseqcIndex(uint32_t diseqcIndex, char* satelliteName);
tTDAL_DMD_Error TDAL_DMD_SetSatipDiseqcIndex(uint32_t diseqcIndex, char* satelliteName);
tTDAL_DMD_Error TDAL_DMD_SetSatipFeIndex(int32_t feIndex);
tTDAL_DMD_Error TDAL_DMD_GetSatipFeIndex(int32_t *feIndex);
tTDAL_DMD_Error TDAL_DMD_GetSatipFeCount(uint32_t *feCnt);

#ifdef BROADCOM_DEV
tTDAL_DMD_Error TDAL_DMD_GetMappedRF(tTDAL_DMD_FE eFeID, tTDAL_DMD_RF *rf);
tTDAL_DMD_Error TDAL_DMD_SetMappedRF(tTDAL_DMD_FE eFeID, tTDAL_DMD_RF rf);
#endif

tTDAL_DMD_Error TDAL_DMD_RegisterUnicableFct(tTDAL_DMD_FE               eFeID,
                                            tTDAL_DMD_NotifyStatusFct   pNotifyStatusFct);

tTDAL_DMD_Error TDAL_DMD_Prep_CollisionDetect(tTDAL_DMD_FE eFeID);
tTDAL_DMD_Error TDAL_DMD_Check_Collision(tTDAL_DMD_FE eFeID);

tTDAL_DMD_Error TDAL_DMD_MonitorUBxSignal(tTDAL_DMD_FE eFeID, uint32_t uiFrequency, uint32_t *puiLevel);



tTDAL_DMD_Error TDAL_DMD_ResumeRead         (tTDAL_DMD_FE                   eFeID);


tTDAL_DMD_Error TDAL_DMD_CaptureTables      (tTDAL_DMD_FE                   eFeID,
                                             void                           **tableData,
                                             uint32_t                       *size,
                                             uint32_t                       *tablePackingFlags);

tTDAL_DMD_Error TDAL_DMD_ReleaseTables      (void                           **tableData);

#ifdef USE_MEDIUM_IP
tTDAL_DMD_Error TDAL_DMD_PreTune(tTDAL_DMD_FE eFeID, tTDAL_DMD_TunData *psTunData);
tTDAL_DMD_Error TDAL_DMD_GetDuration(tTDAL_DMD_FE eFeID, double* durationSec);
tTDAL_DMD_Error TDAL_DMD_GetPositionRange(tTDAL_DMD_FE eFeID, double* startPosition, double* endPosition);
tTDAL_DMD_Error TDAL_DMD_GetCurrentPosition(tTDAL_DMD_FE eFeID, uint64_t current_pts, double* positionSec);
tTDAL_DMD_Error TDAL_DMD_SetCurrentRepresentation(tTDAL_DMD_FE eFeID, uint32_t trackIndex, int32_t representationIndex);
tTDAL_DMD_Error TDAL_DMD_GetCurrentRepresentation(tTDAL_DMD_FE eFeID, uint32_t trackIndex, int32_t* representationIndex);
tTDAL_DMD_Error TDAL_DMD_GetRepresentationInfo(tTDAL_DMD_FE eFeID, uint32_t trackIndex, uint32_t representationIndex, tTDAL_DMD_Representation* info);

#ifdef USE_IP_SERVICE_INSTALLATION
tTDAL_DMD_Error TDAL_DMD_GetIPServiceInfo (tTDAL_DMD_FE eFeID, tTDAL_DMD_IpServiceInfo *psIpServiceInfo );
#endif /* USE_IP_SERVICE_INSTALLATION */

#endif

tTDAL_DMD_Error TDAL_DMD_GetProtocol(char *protocolName, void *protocol, void *protocolInstance);

typedef void (*tTDAL_IP_DMD_ProtocolCallback) (void *protocol, void *protocolInstance);
tTDAL_DMD_Error TDAL_IP_DMD_RegisterProtocolCallback(tTDAL_IP_DMD_ProtocolCallback callback);
tTDAL_DMD_Error TDAL_IP_DMD_UnregisterProtocolCallback(tTDAL_IP_DMD_ProtocolCallback callback);
int32_t TDAL_DMD_LocateFE(tTDAL_DMD_FE eFeID);

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_DMD_H_*/
