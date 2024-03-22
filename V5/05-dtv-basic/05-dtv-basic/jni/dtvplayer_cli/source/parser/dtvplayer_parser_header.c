#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dtvplayer_parser_header.h"

/************************************************************************************
                                   DEFINES
************************************************************************************/
#define ULONG_MAX     0xffffffffUL

/************************************************************************************
                                   FUNCTIONS
************************************************************************************/

/**
 * ! \fn parser_ts_transport_error_indicator(uint8_t* buffer)
 *   \brief The transport_error_indicator is a 1-bit flag. When set to '1' it indicates that at
 *          least 1 uncorrectable bit error exists in the associated Transport Stream packet.
 *          This bit may be set to '1' by entities external to the transport layer.
 *          When set to '1' this bit shall not be reset to '0' unless the bit value(s)
 *          in error have been corrected.
 *   \param[in] buffer pointer to begin of TS package
 *   \return transport error indicator
 */
uint8_t parser_ts_transport_error_indicator(uint8_t* buffer) {
    return buffer ? buffer[1] & 0x80 : 0xff;
}

/**
 * ! \fn parser_ts_payload_unit_start_indicator(uint8_t* buffer)
 *   \brief The payload_unit_start_indicator is a 1-bit flag which has normative meaning for
 *          Transport Stream packets that carry PES packets or PSI data.
 *   \param[in] buffer pointer to begin of TS package
 *   \return unit start indicator
 */
uint8_t parser_ts_payload_unit_start_indicator(uint8_t* buffer) {
    return buffer ? ((buffer[1] & 0x40) == 0x40) : 0xff;
}

/**
 * ! \fn parser_ts_transport_priority(uint8_t* buffer)
 *   \brief The transport_priority is a 1-bit indicator. When set to '1' it indicates that the
 *          associated packet is of greater priority than other packets having the same PID which
 *          do not have the bit set to '1'
 *   \param[in] buffer pointer to begin of TS package
 *   \return transport priority
 */
uint8_t parser_ts_transport_priority(uint8_t* buffer) {
    return buffer ? ((buffer[1] & 0x20) == 0x20) : 0xff;
}

/**
 * ! \fn parser_ts_pid(uint8_t* buffer)
 *   \brief The PID is a 13-bit field, indicating the type of the data stored in the packet payload.
 *          PID value 0x0000 is reserved for the Program Association Table. PID value 0x0001 is
 *          reserved for the Conditional Access Table. PID values 0x0002 â€“ 0x000F are reserved.
 *          PID value 0x1FFF is reserved for null packets
 *   \param[in] buffer pointer to begin of TS package
 *   \return PID
 */
uint16_t parser_ts_pid(uint8_t* buffer) {
    return buffer ? ((uint16_t)buffer[1] & 0x1F) << 8 | buffer[2] : 0xffff;
}

/**
 * ! \fn parser_ts_transport_scrambling_control(uint8_t* buffer)
 *   \brief This 2-bit field indicates the scrambling mode of the Transport Stream packet payload.
 *          The Transport Stream packet header, and the adaptation field when present, shall not be
 *          scrambled. In the case of a null packet the value of the transport_scrambling_control
 *          field shall be set to '00
 *              00 - Not scrambled
 *              01 - User-defined
 *              10 - User-defined
 *              11 - User-defined
 *   \param[in] buffer pointer to begin of TS package
 *   \return transport scrambling control
 */
uint8_t parser_ts_transport_scrambling_control(uint8_t* buffer) {
    return buffer ? (buffer[3] & 0xC0) >> 6 : 0xff;
}

/**
 * ! \fn parser_ts_adaption_field_control(uint8_t* buffer)
 *   \brief This 2-bit field indicates whether this Transport Stream packet header is followed by an
 *          adaptation field and/or payload
 *              00 - Reserved for future use by ISO/IEC
 *              01 - No adaptation_field, payload only
 *              10 - Adaptation_field only, no payload
 *              11 - Adaptation_field followed by payload
 *   \param[in] buffer pointer to begin of TS package
 *   \return adaptive field control
 */
uint8_t parser_ts_adaption_field_control(uint8_t* buffer) {
    return buffer ? (buffer[3] & 0x30) >> 4 : 0xff;
}

/**
 * ! \fn parser_ts_continuity_counter(uint8_t* buffer)
 *   \brief The continuity_counter is a 4-bit field incrementing with each Transport Stream packet
 *          with the same PID. The continuity_counter wraps around to 0 after its maximum value.
 *          The continuity_counter shall not be incremented when the adaptation_field_control of
 *          the packet equals '00' or '10'.
 *   \param[in] buffer pointer to begin of TS package
 *   \return continuity counter
 */
uint8_t parser_ts_continuity_counter(uint8_t* buffer) {
    return buffer ? buffer[3] & 0x0f : 0xff;
}

/**
 * ! \fn DtvPlayer_ParserHeader(uint8_t* buffer, tHeader* header)
 *   \param[in] buffer pointer to begin of TS package
 *   \param[out] table parsed TS header
 *   \return status code, \see tParserStatus
 */
tParserStatus DtvPlayer_ParserHeader(uint8_t* buffer, tHeader* header) {
    if(buffer == NULL || header == NULL) {
        LOGE("Failed to parse TS header, buffer is NULL or header is NULL.\n");
        return PARSER_ERROR_BAD_ARGUMENT;
    }
    if(*buffer != 0x47) {
        LOGE("Sync byte error!\n");
        return PARSER_ERROR_BAD_ARGUMENT;
    }
    header->sync_byte = 0x47;
    header->transpoer_error_indicator = parser_ts_transport_error_indicator(buffer);
    header->payload_unit_start_indicator = parser_ts_payload_unit_start_indicator(buffer);
    header->transport_priority = parser_ts_transport_priority(buffer);
    header->pid = parser_ts_pid(buffer);
    header->transport_scrambling_control = parser_ts_transport_scrambling_control(buffer);
    header->adaptation_field_control = parser_ts_adaption_field_control(buffer);
    header->continuity_counter = parser_ts_continuity_counter(buffer);
    return PARSER_SUCCESS;
}

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
uint8_t DtvPlayer_ParserAfLength(uint8_t* buffer) {
    return buffer ? buffer[4] : 0xff;
}

/**
 * ! \fn DtvPlayer_ParserDumpHeader(tHeader* header)
 *   \brief Dump TS header to print
 *   \param[in] TS header
 */
void DtvPlayer_ParserDumpHeader(tHeader* header) {
    if(!header) {
        LOGE("Failed dump TS header. Header is NULL.\n");
        return;
    }
    LOGI("====================   TS HEADER   ==================\n");
    LOGI("sync_byte: 0x%.2x\n", header->sync_byte);
    LOGI("transpoer_error_indicator: 0x%.2x\n",  header->transpoer_error_indicator);
    LOGI("payload_unit_start_indicator: 0x%.2x\n", header->payload_unit_start_indicator);
    LOGI("transport_priority: 0x%.2x\n", header->transport_priority);
    LOGI("pid: 0x%.2x\n", header->pid);
    LOGI("transport_scrambling_control: 0x%.1x\n", header->transport_scrambling_control);
    LOGI("adaptation_field_control: 0x%.2x\n", header->adaptation_field_control);
    LOGI("continuity_counter: 0x%.2x\n", header->continuity_counter);
    LOGI("=====================================================\n");
}
