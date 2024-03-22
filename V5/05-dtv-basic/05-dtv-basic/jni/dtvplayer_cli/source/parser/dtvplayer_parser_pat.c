#include "dtvplayer_parser_section.h"
#include "dtvplayer_parser_pat.h"
#include "dtvplayer_parser_utils.h"
#include "dtvplayer_demux.h"

/************************************************************************************
                                   DEFINES
************************************************************************************/

#define PROGRAM_SIZE 4
#define MIN_PAT_SIZE SECTION_SIZE + PROGRAM_SIZE /*one program*/ + CRC_SIZE /*CRC*/

/************************************************************************************
                                   FUNCTIONS
************************************************************************************/

/**
 * ! \fn parser_pat_program(uint8_t* buffer, uint8_t* program)
 *   \brief Returns pointer to program in N program loop section
 *   \param[in] buffer pointer to begin of PAT table
 *   \param[in] program pointer to begin of program section
 *   \return program section, or NULL if input buffer is NULL, TID is different then TID_PAT,
 *           size of table smaller then MIN_PAT_SIZE
 */
uint8_t* parser_pat_program(uint8_t* buffer, uint8_t* program) {
    if(!buffer) {
        LOGE("Failed to get PAT program, buffer is NULL.\n");
        return NULL;
    }

    if(*buffer != TID_PAT) {
        return NULL;
    }

    if(!program) {
        return parser_section_length( buffer ) + 3 /*first 3 byte in section header*/
                >= MIN_PAT_SIZE ? buffer + SECTION_SIZE : NULL;
    } else {
        return program + PROGRAM_SIZE < buffer + parser_section_length( buffer )
                + 3 /*first 3 byte in section header*/ - CRC_SIZE ? program + PROGRAM_SIZE : NULL;
    }
}

/**
 * ! \fn parser_pat_program_number(uint8_t* program)
 *   \brief Returns program number. It specifies the program to which the program map PID is
 *          applicable. When value is 0x0000, then the following PID reference shall be the
 *          network PID.
 *   \param[in] program pointer to begin of program section
 *   \return program number, or NULL if program is NULL
 */
uint16_t parser_pat_program_number(uint8_t* program) {
    return (( program ) ? ((((uint16_t) program[0] ) << 8 ) | program[1] ) : 0xff );
}

/**
 * ! \fn parser_pat_program_map_pid(uint8_t* program)
 *   \brief Returns program map PID. The PID of the TS packets which shall contain the
 *          program map section applicable for the program as specified by the program number.
 *   \param[in] program pointer to begin of program section
 *   \return program map PID, or 0xffff if program is NULL
 */
uint16_t parser_pat_program_map_pid(uint8_t* program) {
    return (( program ) ? (((( uint16_t ) program[2] & 0x1f ) << 8 ) | program[3] ) : 0xffff );
}

/**
 * ! \fn DtvPlayer_ParserPATParse(uint8_t* buffer, int size, tPATTable** table)
 *   \brief Parse PAT table
 *   \param[in] buffer pointer to begin of PAT table
 *   \param[out] table parsed PAT table
 *   \return status code, \see tParserStatus
 */
tParserStatus DtvPlayer_ParserPATParse(uint8_t* buffer, tPATTable* table) {
    if(!buffer || !table) {
        LOGE("Failed to parse pat table, buffer size is 0 or buffer is NULL.\n");
        return PARSER_ERROR_BAD_ARGUMENT;
    }

    if(*buffer != TID_PAT) {
        LOGE("Failed to parse PAT table. TID is: %x\n", *buffer);
        return PARSER_ERROR_BAD_TABLE_ID;
    }

    if(DtvPlayer_ParserSectionParse(buffer, &table->section)) {
        LOGE("Failed to parse PAT section!\n");
        return PARSER_ERROR_SECTION_PARSE_FAILED;
    }

    size_t pat_lenght = parser_section_length(buffer) + 3 /*first 3 byte in section header*/;
    table->program_size = (pat_lenght - SECTION_SIZE - CRC_SIZE) / PROGRAM_SIZE;
    if(table->program_size > PS_MAX_PROGRAM_NUMBER) {
        LOGE("Maximum program size reached!\n");
        return PARSER_ERROR_SECTION_PARSE_FAILED;
    }
    uint8_t* program = NULL;
    size_t program_index = 0;
    for(; program_index < table->program_size; program_index++) {
        program = parser_pat_program(buffer, program);
        table->programs[program_index].program_number = parser_pat_program_number(program);
        table->programs[program_index].program_map_pid = parser_pat_program_map_pid(program);
    }

    return PARSER_SUCCESS;
}

/**
 * ! \fn DtvPlayer_ParserPATCopy(tPATTable** to, tPATTable* from)
 *   \brief Function copies PAT table from the object from into the object to
 *   \param[in] from pointer to PAT table
 *   \param[in] to pointer to PAT table
 *   \return status code, \see tParserStatus
 */
tParserStatus DtvPlayer_ParserPATCopy(tPATTable* to, tPATTable* from) {
    if(from == NULL || to == NULL) {
        return PARSER_ERROR_BAD_ARGUMENT;
    }
    memcpy(to, from, sizeof(tPATTable));
    return PARSER_SUCCESS;
}

/**
 * ! \fn DtvPlayer_ParserPATFree(tPATTable* table)
 *   \brief Free allocate memory for PAT table
 *   \param[in] PAT table
 */
void DtvPlayer_ParserPATFree(tPATTable* table) {
}

/**
 * ! \fn DtvPlayer_ParserPATDump(tPATTable* table)
 *   \brief Dump PAT table to print
 *   \param[in] PAT table
 */
void DtvPlayer_ParserPATDump(tPATTable* table) {
    if(!table) {
        printf("Failed dump PAT table. PAT table is NULL.\n");
        return;
    }
    printf("\n");
    printf("===================   PAT TABLE   ===================\n");
    DtvPlayer_ParserSectionDump(&table->section);
    printf("------------------    PROGRAMS   --------------------\n");
    int program_index = 0;
    for(; program_index < table->program_size; program_index++) {
        printf("\tProgram number: 0x%.2x\n", table->programs[program_index].program_number);
        printf("\tProgram map PID: 0x%.2x\n", table->programs[program_index].program_map_pid);
        printf("\n");
    }
    printf("=====================================================\n");
}