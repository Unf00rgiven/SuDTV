#ifndef _DTV_PLAYER_PARSER_DESCRIPTOR_H_
#define _DTV_PLAYER_PARSER_DESCRIPTOR_H_
/*************************************************************************************
                                INCLUDE FILES
*************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
                                   DEFINES
************************************************************************************/

/************************************************************************************
                                   TYPES
************************************************************************************/
typedef enum {
    DESC_TAG_RESERVED_0                   =  0x00,
    DESC_TAG_RESERVED_1                   =  0x01,
    DESC_TAG_VIDEO_STREAM                 =  0x02,
    DESC_TAG_AUDIO_STREAM                 =  0x03,
    DESC_TAG_HIERARCHY                    =  0x04,
    DESC_TAG_REGISTRATION                 =  0x05,
    DESC_TAG_DATA_STREAM_ALIGNMENT        =  0x06,
    DESC_TAG_TARGET_BACKGROUND_GRID       =  0x07,
    DESC_TAG_VIDEO_WINDOW                 =  0x08,
    DESC_TAG_CA                           =  0x09,
    DESC_TAG_ISO_639_LANGUAGE             =  0x0A,
    DESC_TAG_SYSTEM_CLOCK                 =  0x0B,
    DESC_TAG_MULTIPLEX_BUFFER_UTILIZATION =  0x0C,
    DESC_TAG_COPYRIGHT                    =  0x0D,
    DESC_TAG_MAXIMUM_BITRATE              =  0x0E,
    DESC_TAG_PRIVATE_DATA_INDICATOR       =  0x0F,
    DESC_TAG_SMOOTHING_BUFFER             =  0x10,
    DESC_TAG_STD                          =  0x11,
    DESC_TAG_CAROUSEL_IDENTIFIER          =  0x12
} tDescriptorTag;

typedef struct {
    uint16_t ca_system_id;
    uint16_t ca_pid;
    uint8_t* private_data_bytes;
} tCADescriptor;

/************************************************************************************
                                   FUNCTIONS
************************************************************************************/
/**
 * ! \fn DtvPlayer_ParserDescriptor(uint8_t* buffer)
 *   \brief Returns pointer to first descriptor in N loop descriptors
 *   \param[in] buffer pointer to descriptors section
 *   \return pointer to descriptor, or NULL if input buffer is NULL
 */
uint8_t* DtvPlayer_ParserDescriptor(uint8_t* buffer);

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
uint8_t DtvPlayer_ParserDescriptorTag(uint8_t* buffer);

/**
 * ! \fn DtvPlayer_ParserDescriptorLenght(uint8_t* buffer)
 *   \brief Returns descriptor length
 *   \param[in] buffer pointer to descriptors section
 *   \return descriptor length, or 0xff if input buffer is NULL
 */
uint8_t DtvPlayer_ParserDescriptorLenght(uint8_t* buffer);

/************************************************************************************
                               CA DESCRIPTOR
************************************************************************************/

/**
 * ! \fn DtvPlayer_ParserDescriptorCASystemId(uint8_t* buffer)
 *   \brief Returns CA system identifier
 *   \param[in] buffer pointer to descriptors section
 *   \return CA system ID, or 0xffff if input buffer is NULL
 */
uint16_t DtvPlayer_ParserDescriptorCASystemId(uint8_t* buffer);

/**
 * ! \fn DtvPlayer_ParserDescriptorCAPid(uint8_t* buffer)
 *   \brief Returns CA table PID
 *   \param[in] buffer pointer to descriptors section
 *   \return CA table PID, or 0xffff if input buffer is NULL
 *           CA descriptor founded in PMT table contains ECM table PID.
 *           CA descriptor founded in CAT table contains EMM table PID.
 */
uint16_t DtvPlayer_ParserDescriptorCAPid(uint8_t* buffer);

#ifdef __cplusplus
}
#endif
#endif /* _DTV_PLAYER_PARSER_DESCRIPTOR_H_ */
