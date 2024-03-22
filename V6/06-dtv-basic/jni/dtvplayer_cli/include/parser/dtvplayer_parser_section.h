#ifndef _DTV_PLAYER_PARSER_SECTION_H_
#define _DTV_PLAYER_PARSER_SECTION_H_

/*************************************************************************************
                                INCLUDE FILES
*************************************************************************************/
#include "dtvplayer_parser_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
                                   DEFINES
************************************************************************************/
#define SECTION_SIZE 8
#define CRC_SIZE 4

/************************************************************************************
                                   TYPES
************************************************************************************/

typedef struct {
    uint8_t table_id;
    uint16_t section_length;
    uint16_t transport_stream_id;
    uint8_t section_version;
    uint8_t section_number;
    uint8_t last_section_number;
} tSectionHeader;

/************************************************************************************
                                   FUNCTIONS
************************************************************************************/
/**
 * ! \fn DtvPlayer_ParserSectionParse(uint8_t* buffer, tSectionHeader** section)
 *   \brief Parse section
 *   \param[in] buffer pointer to begin of table section
 *   \param[out] section parsed table section
 *   \return status code, \see tParserStatus
 */
tParserStatus DtvPlayer_ParserSectionParse(uint8_t* buffer, tSectionHeader* section);

/**
 * ! \fn DtvPlayer_ParserSectionDump(tSectionHeader* section)
 *   \brief Dump table section to print
 *   \param[in] table section
 */
void DtvPlayer_ParserSectionDump(tSectionHeader* section);

uint16_t parser_section_length(uint8_t* section);

uint8_t parser_section_table_id(uint8_t* section);

#ifdef __cplusplus
}
#endif
#endif /* _DTV_PLAYER_PARSER_SECTION_H_ */
