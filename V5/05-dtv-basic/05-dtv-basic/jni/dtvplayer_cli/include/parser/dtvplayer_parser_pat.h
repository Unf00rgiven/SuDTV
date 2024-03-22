#ifndef _DTV_PLAYER_PARSER_PAT_H_
#define _DTV_PLAYER_PARSER_PAT_H_

/*************************************************************************************
                                INCLUDE FILES
*************************************************************************************/
#include "dtvplayer_parser_section.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
                                   DEFINES
************************************************************************************/
#define PS_MAX_PROGRAM_NUMBER 10

/************************************************************************************
                                   TYPES
************************************************************************************/
typedef struct {
    uint16_t program_number;
    uint16_t program_map_pid;
} tPATProgram;

typedef struct {
    tSectionHeader section;
    tPATProgram programs[PS_MAX_PROGRAM_NUMBER];
    uint16_t program_size;
} tPATTable;

/************************************************************************************
                                   FUNCTIONS
************************************************************************************/
/**
 * ! \fn DtvPlayer_ParserPATParse(uint8_t* buffer, int size, tPATTable** table)
 *   \brief Parse PAT table
 *   \param[in] buffer pointer to begin of PAT table
 *   \param[out] table parsed PAT table
 *   \return status code, \see tParserStatus
 */
tParserStatus DtvPlayer_ParserPATParse(uint8_t* buffer, tPATTable* table);

/**
 * ! \fn DtvPlayer_ParserPATCopy(tPATTable** to, tPATTable* from)
 *   \brief Function copies PAT table from the object from into the object to
 *   \param[in] from pointer to PAT table
 *   \param[in] to pointer to PAT table
 *   \return status code, \see tParserStatus
 */
tParserStatus DtvPlayer_ParserPATCopy(tPATTable* to, tPATTable* from);

/**
 * ! \fn DtvPlayer_ParserPATFree(tPATTable* table)
 *   \brief Free allocate memory for PAT table
 *   \param[in] PAT table
 */
void DtvPlayer_ParserPATFree(tPATTable* table);

/**
 * ! \fn DtvPlayer_ParserPATDump(tPATTable* table)
 *   \brief Dump PAT table to print
 *   \param[in] PAT table
 */
void DtvPlayer_ParserPATDump(tPATTable* table);

#ifdef __cplusplus
}
#endif
#endif /* _DTV_PLAYER_PARSER_PAT_H_ */
