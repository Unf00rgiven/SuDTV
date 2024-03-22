#ifndef __DTV_PLAYER_COMM_H__
#define __DTV_PLAYER_COMM_H__

#include "dtvplayer_utils.h"

typedef struct _cli_cmd
{
  char* name;
  void (*execute)(size_t, char **);
  char* brief;
} tCliCommands;

tErrorCode DtvPlayer_Comm_Read();

#endif // __DTV_PLAYER_COMM_H__
