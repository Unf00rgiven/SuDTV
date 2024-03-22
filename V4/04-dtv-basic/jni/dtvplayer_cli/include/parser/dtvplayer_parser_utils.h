#ifndef _DTV_PLAYER_PARSER_UTILS_H_
#define _DTV_PLAYER_PARSER_UTILS_H_

/*************************************************************************************
                                INCLUDE FILES
*************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <android/log.h>

#include "dtvplayer_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
                                   TYPES
************************************************************************************/
typedef enum {
    PARSER_SUCCESS = 0,
    PARSER_ERROR_BAD_ARGUMENT,
    PARSER_ERROR_MEMORY,
    PARSER_ERROR_BAD_TABLE_ID,
    PARSER_ERROR_SECTION_PARSE_FAILED
} tParserStatus;

#ifdef __cplusplus
}
#endif
#endif /* _DTV_PLAYER_PARSER_UTILS_H_ */
