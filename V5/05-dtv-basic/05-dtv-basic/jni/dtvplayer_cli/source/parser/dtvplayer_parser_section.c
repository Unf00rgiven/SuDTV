#include "dtvplayer_parser_section.h"
#include "dtvplayer_parser_utils.h"

/**************************************************************************************************
 *                PAT                   CAT(ECM/EMM)              PMT
 *  table_id                     8   |      =/=       |                         =/=
 *  section syntax indicator     1   |      =/=       |                         =/=
 *  0                            1   |      =/=       |                         =/=
 *  reserved                     2   |      =/=       |                         =/=
 *  section length               12  |      =/=       |                         =/=
 *  transport stream id          16  |  \             |                         =/=
 *  reserved                     2   |  /   18        |                         =/=
 *  version number               5   |      =/=       |                         =/=
 *  current next indicator       1   |      =/=       |                         =/=
 *  section number               8   |      =/=       |                         =/=
 *  last section number          8   |      =/=       |                         =/=
 *                                                    |  PCR_PID                 13
 *                                                    |  reserved                4
 *                                                    |  program info length     16
 **************************************************************************************************/


/************************************************************************************
                                   DEFINES
************************************************************************************/


/************************************************************************************
                                   FUNCTIONS
************************************************************************************/

/**
 * ! \fn parser_section_table_id(uint8_t* section)
 *   \brief Return table ID
 *   \param[in]  section pointer to begin of table section
 *   \return table ID
 */
uint8_t parser_section_table_id(uint8_t* section) {
    return (( section ) ? section[0] : 0xff );
}

/**
 * ! \fn parser_section_length(uint8_t* section)
 *   \brief The lower 10 bits specify the number of bytes of the section.
 *   \param[in] section pointer to begin of table section
 *   \return section length
 */
uint16_t parser_section_length(uint8_t* section) {
    return (( section ) ? (((((uint16_t) section[1] & 0x0f ) << 8 ) | section[2] )) : 0xffff );
}

/**
 * ! \fn parser_section_transport_stream_id(uint8_t* section)
 *   \brief Transport stream identifier for identify TS from other multiplex within a network.
 *          User define value.
 *   \param[in] section pointer to begin of table section
 *   \return transport stream ID
 */
uint16_t parser_section_transport_stream_id(uint8_t* section) {
    return (( section ) ? ((((uint16_t) section[3] ) << 8 ) | section[4] ) : 0xffff );
}

/**
 * ! \fn parser_section_version(uint8_t* section)
 *   \brief The version number of the whole table.
 *   \param[in] section pointer to begin of table section
 *   \return section version
 */
uint8_t parser_section_version(uint8_t* section) {
    return (( section ) ?  (( section[5] >> 1 ) & 0x1f ): 0xff );
}

/**
 * ! \fn parser_section_number(uint8_t* section)
 *   \brief The section number tells what section this happens to be. It is set to zero in this
 *          case, i.e. the table consists of only one section.
 *   \param[in] section pointer to begin of table section
 *   \return section number
 */
uint8_t parser_section_number(uint8_t* section) {
    return (( section ) ? (((uint8_t) section[6] )) : 0xff );
}

/**
 * ! \fn parser_section_last_section_number(uint8_t* section)
 *   \brief The last section informs about the number of the last section of a table.
 *   \param[in] section pointer to begin of table section
 *   \return last section number
 */
uint8_t parser_section_last_section_number(uint8_t* section) {
    return (( section ) ? (((uint8_t) section[7] )) : 0xff );
}

/**
 * ! \fn DtvPlayer_ParserSectionParse(uint8_t* buffer, tSectionHeader** section)
 *   \brief Parse section
 *   \param[in] buffer pointer to begin of table section
 *   \param[out] section parsed table section
 *   \return status code, \see tParserStatus
 */
tParserStatus DtvPlayer_ParserSectionParse(uint8_t* buffer, tSectionHeader* section) {
    if(buffer == NULL || section == NULL) {
        LOGE("Failed to parse table section, buffer is NULL.\n");
        return PARSER_ERROR_BAD_ARGUMENT;
    }
    section->table_id = parser_section_table_id(buffer);
    section->section_length = parser_section_length(buffer);
    // In case of ECM table there is no transport_stream_id field. It is part of reserved field.
    section->transport_stream_id = parser_section_transport_stream_id(buffer);
    section->section_version = parser_section_version(buffer);
    section->section_number = parser_section_number(buffer);
    section->last_section_number = parser_section_last_section_number(buffer);
    return PARSER_SUCCESS;
}

/**
 * ! \fn DtvPlayer_ParserSectionDump(tSectionHeader* section)
 *   \brief Dump table section to print
 *   \param[in] table section
 */
void DtvPlayer_ParserSectionDump(tSectionHeader* section) {
    if(!section) {
        LOGE("Failed dump section. Section is NULL.\n");
        return;
    }
    printf("-------------------    SECTION   --------------------\n");
    printf("Table id: 0x%.2x\n", section->table_id);
    printf("Section length: 0x%.2x\n", section->section_length);
    printf("Transport stream id: 0x%.2x\n", section->transport_stream_id);
    printf("Section version: 0x%.2x\n", section->section_version);
    printf("Section number: 0x%.2x\n", section->section_number);
    printf("Last section number: 0x%.2x\n", section->last_section_number);
    printf("-----------------------------------------------------\n");
}
