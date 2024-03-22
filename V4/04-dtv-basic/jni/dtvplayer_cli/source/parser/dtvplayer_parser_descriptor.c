#include "dtvplayer_parser_utils.h"
#include "dtvplayer_parser_descriptor.h"

/************************************************************************************
                                   FUNCTIONS
************************************************************************************/

/**
 * ! \fn DtvPlayer_ParserDescriptor(uint8_t* buffer)
 *   \brief Returns pointer to first descriptor in N loop descriptors
 *   \param[in] buffer pointer to descriptors section
 *   \return pointer to descriptor, or NULL if input buffer is NULL
 */
uint8_t* DtvPlayer_ParserDescriptor(uint8_t* buffer) {
    return buffer ? buffer + 5 : NULL;
}

/**
 * ! \fn DtvPlayer_ParserDescriptorTag(uint8_t* buffer)
 *   \brief  Returns descriptor tag value
 *   \param[in] buffer buffer pointer to descriptors section
 *   \return descriptor tag value, or 0xff if input buffer is NULL
 *           Descriptor tags possible values:
 *               0x00 - reserved
 *               0x01 - reserved
 *               0x02 - video stream descriptor
 *               0x03 - audio stream descriptor
 *               0x04 - hierarchy descriptor
 *               0x05 - registration descriptor
 *               0x06 - data stream alignment descriptor
 *               0x07 - target background grid descriptor
 *               0x08 - video window descriptor
 *               0x09 - CA descriptor
 *               0x0A - ISO 639 language descriptor
 */
uint8_t DtvPlayer_ParserDescriptorTag(uint8_t* buffer) {
    return buffer ? *buffer : 0xff;
}

/**
 * ! \fn DtvPlayer_ParserDescriptorLenght(uint8_t* buffer)
 *   \brief Returns descriptor length
 *   \param[in] buffer pointer to descriptors section
 *   \return descriptor length, or 0xff if input buffer is NULL
 */
uint8_t DtvPlayer_ParserDescriptorLenght(uint8_t* buffer) {
    return buffer ? buffer[1] : 0xff;
}

/************************************************************************************
                               CA DESCRIPTOR
************************************************************************************/

/**
 * ! \fn DtvPlayer_ParserDescriptorCASystemId(uint8_t* buffer)
 *   \brief Returns CA system identifier
 *   \param[in] buffer pointer to descriptors section
 *   \return CA system ID, or 0xffff if input buffer is NULL
 */
uint16_t DtvPlayer_ParserDescriptorCASystemId(uint8_t* buffer) {
    return buffer ? ((uint16_t) buffer[2]) << 8 | buffer[3] : 0xffff;
}

/**
 * ! \fn DtvPlayer_ParserDescriptorCAPid(uint8_t* buffer)
 *   \brief Returns CA table PID
 *   \param[in] buffer pointer to descriptors section
 *   \return CA table PID, or 0xffff if input buffer is NULL
 *           CA descriptor founded in PMT table contains ECM table PID.
 *           CA descriptor founded in CAT table contains EMM table PID.
 */
uint16_t DtvPlayer_ParserDescriptorCAPid(uint8_t* buffer) {
    return buffer ? ((uint16_t) buffer[4] & 0x1f) << 8 | buffer[5] : 0xffff;
}

