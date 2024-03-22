#ifndef _DTV_PLAYER_PARSER_PMT_H_
#define _DTV_PLAYER_PARSER_PMT_H_

/*************************************************************************************
                                INCLUDE FILES
*************************************************************************************/
#include "dtvplayer_parser_section.h"
#include "dtvplayer_parser_descriptor.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
                                   DEFINES
************************************************************************************/

#define PS_MAX_STREAM_NUMBER 10
#define PS_MAX_DESCRIPTOR_NUMBER 10

/************************************************************************************
                                   TYPES
************************************************************************************/
typedef enum {
    STREAM_TYPE_RESERVED                                   = 0x00, // ITU-T/ISO/IEC reserved
    STREAM_TYPE_ISO_IEC_11172_MPEG_1_VIDEO                 = 0x01, // VIDEO
    STREAM_TYPE_ITU_T_H262_ISO_IEC_13818_2_MPEG_2_VIDEO    = 0x02, // VIDEO
    STREAM_TYPE_ISO_IEC_11172_MPEG_1_AUDIO                 = 0x03,
    STREAM_TYPE_ISO_IEC_13818_2_MPEG_2_AUDIO               = 0x04, // AUDIO
    STREAM_TYPE_ITU_T_H222_ISO_IEC_13818_1_PRIVATE_SECTION = 0x05,
    STREAM_TYPE_ITU_T_H222_ISO_IEC_13818_1_PEIVATE_DATA    = 0x06,
    STREAM_TYPE_ISO_IEC_13522_MHEG                         = 0x07,
    STREAM_TYPE_ITU_T_H222_ISO_IEC_13818_1                 = 0x08,
    STREAM_TYPE_ITU_T_H222_1                               = 0x09,
    STREAM_TYPE_ISO_IEC_13818_6_DSM_CC_TYPE_A              = 0x0A,
    STREAM_TYPE_ISO_IEC_13818_6_DSM_CC_TYPE_B              = 0x0B,
    STREAM_TYPE_ISO_IEC_13818_6_DSM_CC_TYPE_C              = 0x0C,
    STREAM_TYPE_ISO_IEC_13818_6_DSM_CC_type_D              = 0x0D,
    STREAM_TYPE_ISO_IEC_13818_1_AUXILIARY                  = 0x0E,
    STREAM_TYPE_ISO_IEC_13818_7_AAC                        = 0x0F, // AUDIO
    STREAM_TYPE_ISO_IEC_14496_10_H264                      = 0x1B  // VIDEO
} tPMTStreamType;

typedef struct {
    uint8_t tag;
    uint8_t lenght;
    union {
        tCADescriptor ca_descriptor;
    };
} tDescriptor;

typedef struct {
    uint8_t stream_type;
    uint16_t elementary_pid;
    uint16_t es_info_lenght;
    tDescriptor es_descriptor;
} tPMTStream;

typedef struct {
    tSectionHeader section;
    uint16_t pcr_pid;
    uint16_t program_info_lenght;
    uint16_t pi_descriptor_number;
    tDescriptor pi_descriptors[PS_MAX_DESCRIPTOR_NUMBER];
    uint16_t streams_number;
    tPMTStream streams[PS_MAX_STREAM_NUMBER];
} tPMTTable;

/************************************************************************************
                                   FUNCTIONS
************************************************************************************/
/**
 * ! \fn DtvPlayer_ParserPMTParse(uint8_t* buffer, int size, tPMTTable** table)
 *   \brief Parse PMT table
 *   \param[in] buffer pointer to begin of PMT table
 *   \param[out] table parsed PMT table
 *   \return status code, \see tParserStatus
 */
tParserStatus DtvPlayer_ParserPMTParse(uint8_t* buffer, tPMTTable* table);

/**
 * ! \fn DtvPlayer_ParserPMTCopy(tPMTTable** to, tPMTTable* from)
 *   \brief Function copies PMT table from the object from into the object to
 *   \param[in] from pointer to PMT table
 *   \param[in] to pointer to PMT table
 *   \return status code, \see tParserStatus
 */
tParserStatus DtvPlayer_ParserPMTCopy(tPMTTable* to, tPMTTable* from);

/**
 * ! \fn DtvPlayer_ParserPMTFree(tPMTTable* table)
 *   \brief Free allocate memory for PMT table
 *   \param[in] PMT table
 */
void DtvPlayer_ParserPMTFree(tPMTTable* table);

/**
 * ! \fn DtvPlayer_ParserPMTDump(tPMTTable* table)
 *   \brief Dump PMT table to print
 *   \param[in] PMT table
 */
void DtvPlayer_ParserPMTDump(tPMTTable* table);

#ifdef __cplusplus
}
#endif
#endif /* _DTV_PLAYER_PARSER_PMT_H_ */
