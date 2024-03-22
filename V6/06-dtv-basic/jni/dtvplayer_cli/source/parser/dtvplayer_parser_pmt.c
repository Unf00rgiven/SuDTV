#include "dtvplayer_parser_section.h"
#include "dtvplayer_parser_pmt.h"
#include "dtvplayer_parser_descriptor.h"
#include "dtvplayer_parser_utils.h"
#include "dtvplayer_demux.h"

/************************************************************************************
                                   DEFINES
************************************************************************************/

/*
 * PCR PID + PROGRAM INFO LENGHT
 */
#define SECTION_EXTENSION_SIZE 4
/*
 * Stream size with out stream descriptor
 */
#define MIN_STREAM_SIZE 5
#define MIN_PMT_SIZE SECTION_SIZE + SECTION_EXTENSION_SIZE + MIN_STREAM_SIZE + CRC_SIZE

/************************************************************************************
                                   FUNCTIONS
************************************************************************************/

/**
 * ! \fn parser_pmt_pcr_pid(uint8_t* buffer)
 *   \brief Return PMT PCR PID of the TS packets which shall contain the PCR fields valid for the
 *          program specified by program number
 *   \param[in] buffer pointer to begin of PMT table
 *   \return PMT PCR PID
 */
uint16_t parser_pmt_pcr_pid(uint8_t* buffer) {
    return ( buffer && ( parser_section_table_id( buffer ) == TID_PMT ) ?
            ((((uint16_t) buffer[8] & 0x1f) << 8 ) | buffer[9] ) : 0xffff );
}

/**
 * ! \fn parser_pmt_program_info_length(uint8_t* buffer)
 *   \brief Return number of bytes of the descriptors
 *   \param[in] buffer pointer to begin of PMT table
 *   \return program info length
 */
uint16_t parser_pmt_program_info_length(uint8_t* buffer) {
    return ( buffer && ( parser_section_table_id( buffer ) == TID_PMT ) ?
            ((((uint16_t) buffer[10] & 0x0f) << 8 ) | buffer[11] ) : 0xffff );
}

/**
 * ! \fn parser_pmt_es_info_lenght(uint8_t* buffer)
 *   \brief Return number of bytes of the descriptors
 *   \param[in] stream pointer to begin of stream
 *   \return es info length
 */
uint16_t parser_pmt_es_info_length(uint8_t* stream) {
    return stream ? ((uint16_t) stream[3] & 0x0f) << 8 | stream[4] : 0xffff;
}

/**
 * ! \fn parser_pmt_stream(uint8_t* buffer, uint8_t* stream)
 *   \brief Returns pointer to stream in N stream loop section
 *   \param[in] buffer pointer to begin of PMT table
 *   \param[in] stream pointer to begin of stream
 *   \return pointer to begin of stream
 */
uint8_t* parser_pmt_stream(uint8_t* buffer, uint8_t* stream) {
    if(!buffer) {
        LOGE("Failed to get PMT stream, buffer is NULL.\n");
        return NULL;
    }

    if(*buffer != TID_PMT) {
        LOGE("Failed to get PMT stream, invalid TID.\n");
        return NULL;
    }

    if(!stream) {
        return parser_section_length( buffer ) + 1 /*first byte in section header*/ >= MIN_PMT_SIZE
                ? buffer + SECTION_SIZE + SECTION_EXTENSION_SIZE
                + parser_pmt_program_info_length(buffer) : NULL;
    } else {
        return stream + MIN_STREAM_SIZE + parser_pmt_es_info_length(stream) < buffer
                + parser_section_length( buffer ) + 1 /*first byte in section header*/ - 4 ?
                  stream + MIN_STREAM_SIZE + parser_pmt_es_info_length(stream) : NULL;
    }
}

/**
 * ! \fn parser_pmt_stream_type(uint8_t* buffer)
 *   \brief Returns type of payload
 *   \param[in] stream pointer to begin of stream
 *   \return stream type
 */
uint8_t parser_pmt_stream_type(uint8_t* stream) {
    return stream ? *stream : 0xff;
}

/**
 * ! \fn parser_pmt_elementary_pid(uint8_t* buffer)
 *   \brief Returns PID of the TS packet which carry the associated program element
 *   \param[in] stream pointer to begin of stream
 *   \return elementary PID
 */
uint16_t parser_pmt_elementary_pid(uint8_t* stream) {
    return stream ? ((uint16_t) stream[1] & 0x1f) << 8 | stream[2] : 0xffff;
}

/**
 * ! \fn DtvPlayer_ParserPMTParse(uint8_t* buffer, int size, tPMTTable** table)
 *   \brief Parse PMT table
 *   \param[in] buffer pointer to begin of PMT table
 *   \param[out] table parsed PMT table
 *   \return status code, \see tParserStatus
 */
tParserStatus DtvPlayer_ParserPMTParse(uint8_t* buffer, tPMTTable* table) {
    if (!buffer || !table) {
        LOGE("Failed to parse pat table, buffer size is 0 or buffer is NULL.\n");
        return PARSER_ERROR_BAD_ARGUMENT;
    }

    if(*buffer != TID_PMT) {
        LOGE("Failed to parse pmt table. TID is: 0x%.2x\n", *buffer);
        return PARSER_ERROR_BAD_TABLE_ID;
    }

    if(DtvPlayer_ParserSectionParse(buffer, &table->section)) {
        LOGE("Failed to parse PMT section!\n");
        return PARSER_ERROR_SECTION_PARSE_FAILED;
    }

    // IMPLEMENTATION
    table->pcr_pid = parser_pmt_pcr_pid(buffer);
    table->program_info_lenght = parser_pmt_program_info_length(buffer);
    //////////////////////////////////////////////////

    // Parsing program info
    tDescriptor pi_descriptors[PS_MAX_DESCRIPTOR_NUMBER];
    int j = 0;
    int offset = 0;
    if(table->program_info_lenght) {
        while(offset < table->program_info_lenght) {
            pi_descriptors[j].tag = DtvPlayer_ParserDescriptorTag(buffer + 12 + offset);
            pi_descriptors[j].lenght = DtvPlayer_ParserDescriptorLenght(buffer + 12 + offset);
            if(pi_descriptors[j].tag == DESC_TAG_CA) {
                pi_descriptors[j].ca_descriptor.ca_pid = DtvPlayer_ParserDescriptorCAPid(buffer + 12
                        + offset);
                pi_descriptors[j].ca_descriptor.ca_system_id = DtvPlayer_ParserDescriptorCASystemId(buffer
                        + 12 + offset);
            }
            offset = offset + pi_descriptors[j].lenght + 2;
            j++;
        }
    }
    table->pi_descriptor_number = j;
    memcpy(table->pi_descriptors, pi_descriptors, table->pi_descriptor_number * sizeof(tDescriptor));


    // IMPLEMENTATION
    uint8_t* stream = NULL;
    tPMTStream streams[PS_MAX_STREAM_NUMBER];
    int index = 0;
    stream = parser_pmt_stream(buffer, stream);
    while(stream) {
        streams[index].stream_type = parser_pmt_stream_type(stream);
        streams[index].elementary_pid = parser_pmt_elementary_pid(stream);
        streams[index].es_info_lenght = parser_pmt_es_info_length(stream);
        if(streams[index].es_info_lenght) {
            uint8_t* stream_descriptor = DtvPlayer_ParserDescriptor(stream);
            streams[index].es_descriptor.tag = DtvPlayer_ParserDescriptorTag(stream_descriptor);
            streams[index].es_descriptor.lenght = DtvPlayer_ParserDescriptorLenght(stream_descriptor);
            if(streams[index].es_descriptor.tag == DESC_TAG_CA) {
                streams[index].es_descriptor.ca_descriptor.ca_pid =
                        DtvPlayer_ParserDescriptorCAPid(stream_descriptor);
                streams[index].es_descriptor.ca_descriptor.ca_system_id =
                        DtvPlayer_ParserDescriptorCASystemId(stream_descriptor);
            }
        }
        index++;
        stream = parser_pmt_stream(buffer, stream);
    }
    table->streams_number = index;

    memcpy(table->streams, streams, table->streams_number * sizeof(tPMTStream));
    //////////////////////////////////////////////////

    return PARSER_SUCCESS;
}

/**
 * ! \fn DtvPlayer_ParserPMTCopy(tPMTTable** to, tPMTTable* from)
 *   \brief Function copies PMT table from the object from into the object to
 *   \param[in] from pointer to PMT table
 *   \param[in] to pointer to PMT table
 *   \return status code, \see tParserStatus
 */
tParserStatus DtvPlayer_ParserPMTCopy(tPMTTable* to, tPMTTable* from) {
    if(!from || !to) {
        return PARSER_ERROR_BAD_ARGUMENT;
    }
    memcpy(to, from, sizeof(tPMTTable));
    return PARSER_SUCCESS;
}

/**
 * ! \fn DtvPlayer_ParserPMTFree(tPMTTable* table)
 *   \brief Free allocate memory for PMT table
 *   \param[in] PMT table
 */
void DtvPlayer_ParserPMTFree(tPMTTable* table) {
}

/**
 * ! \fn DtvPlayer_ParserPMTDump(tPMTTable* table)
 *   \brief Dump PMT table to print
 *   \param[in] PMT table
 */
void DtvPlayer_ParserPMTDump(tPMTTable* table) {
    if(!table) {
        LOGE("Failed dump PMT table. PMT table is NULL.\n");
        return;
    }
    printf("\n");
    printf("===================   PMT TABLE   ===================\n");
    DtvPlayer_ParserSectionDump(&table->section);
    printf("pcr pid: 0x%.2x\n", table->pcr_pid);
    printf("program info length: 0x%.2x\n", table->program_info_lenght);
    printf("-----------------------------------------------------\n");
    if(table->pi_descriptor_number > 0) {
        printf("\t----    PI DESCRIPTORS    ----\n");
        int pi_descriptor_index = 0;
        for(;pi_descriptor_index < table->pi_descriptor_number; pi_descriptor_index++) {
            printf("\tDescriptor tag: 0x%.2x\n", table->pi_descriptors[pi_descriptor_index].tag);
            printf("\tDescriptor length: 0x%.2x\n", table->pi_descriptors[pi_descriptor_index].lenght);
            if(table->pi_descriptors[pi_descriptor_index].tag == DESC_TAG_CA) {
                printf("\t\tCA pid: 0x%.2x\n", table->pi_descriptors[pi_descriptor_index].ca_descriptor.ca_pid);
                printf("\t\tCA system id: 0x%.2x\n",table->pi_descriptors[pi_descriptor_index].ca_descriptor.ca_system_id);
            }
            printf("\n");
        }
        printf("-----------------------------------------------------\n");
    }
    printf("-------------------    STREAMS    -------------------\n");
    int stream_index = 0;
    for(; stream_index < table->streams_number; stream_index++) {
        printf("\tstream type: 0x%.2x\n", table->streams[stream_index].stream_type);
        printf("\telementary PID: 0x%.2x\n", table->streams[stream_index].elementary_pid);
        printf("\tes info length: 0x%.2x\n", table->streams[stream_index].es_info_lenght);
        if(table->streams[stream_index].es_info_lenght) {
            printf("\t\t----    ES INFO    ----\n");
            printf("\t\tdescriptor tag: 0x%.2x\n", table->streams[stream_index].es_descriptor.tag);
            printf("\t\tdescriptor type: 0x%.2x\n", table->streams[stream_index].es_descriptor.lenght);
        }
        printf("\n");
    }
    printf("=====================================================\n");
}
