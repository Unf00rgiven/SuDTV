#ifndef _DTV_PLAYER_PARSER_HEADER_H_
#define _DTV_PLAYER_PARSER_HEADER_H_

/*************************************************************************************
                                INCLUDE FILES
*************************************************************************************/

#include "dtvplayer_parser_section.h"
#include "dtvplayer_parser_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
                                   DEFINES
************************************************************************************/
#define PID_PAT           (0x0000)
#define PID_CAT           (0x0001)
#define PID_NULL_PACKET   (0x1fff)

#define TS_HEADER_SIZE 4

/************************************************************************************
                                   TYPES
************************************************************************************/
typedef enum {
    NO_ADAPTATION_FIELD = 0x01,               //no adaptation field; payload only
    ADAPTIVE_FIELD_WITHOUT_PAYLOAD = 0x02,    //adaptation field; no payload
    ADAPTIVE_FIELD_WITH_PAYLOAD = 0x03        //adaptation field followed by payload
} tAdaptationField;

typedef enum {
    NOT_SCRAMBLED = 0x00,
    SCRAMBLED_NOT_DEFINE = 0x01,
    SCRAMBLED_EVEN_KEY = 0x02,
    SCRAMBLED_ODD_KEY = 0x03
} tScramblingControl;

typedef struct {
    uint8_t sync_byte;
    uint8_t transpoer_error_indicator;
    uint8_t payload_unit_start_indicator;
    uint8_t transport_priority;
    uint16_t pid;
    uint8_t transport_scrambling_control;
    uint8_t adaptation_field_control;
    uint8_t continuity_counter;
} tHeader;

/************************************************************************************
                                   FUNCTIONS
************************************************************************************/

/**
 * ! \fn DtvPlayer_ParserAfLength(uint8_t* buffer)
 *   \brief The adaptation_field_length is an 8-bit field specifying the number of bytes in the
 *          adaptation_field immediately following the adaptation_field_length. The value 0 is for
 *          inserting a single stuffing byte in a Transport Stream packet. When the
 *          adaptation_field_control value is '11', the value of the adaptation_field_length shall
 *          be in the range 0 to 182. When the adaptation_field_control value is '10', the value of
 *          the adaptation_field_length shall be 183.
 *   \param[in] buffer pointer to begin of TS package
 *   \return adaptation field length
 */
uint8_t DtvPlayer_ParserAfLength(uint8_t* buffer);

/**
 * ! \fn DtvPlayer_ParserHeader(uint8_t* buffer, tHeader* header)
 *   \param[in] buffer pointer to begin of TS package
 *   \param[out] table parsed TS header
 *   \return status code, \see tParserStatus
 */
tParserStatus DtvPlayer_ParserHeader(uint8_t* buffer, tHeader* header);

/**
 * ! \fn DtvPlayer_ParserDumpHeader(tHeader* header)
 *   \brief Dump TS header to print
 *   \param[in] TS header
 */
void DtvPlayer_ParserDumpHeader(tHeader* header);

#ifdef __cplusplus
}
#endif
#endif /* _DTV_PLAYER_PARSER_HEADER_H_ */
