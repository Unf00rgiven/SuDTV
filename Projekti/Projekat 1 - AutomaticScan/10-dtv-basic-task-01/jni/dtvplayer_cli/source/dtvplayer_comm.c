#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <stdarg.h>
#include <getopt.h>

#include "dtvplayer_comm.h"
#include "dtvplayer.h"

#define MAX_ARG_COUNT 50

void DtvPlayer_Execute_Help(size_t argc, char** argv);
void DtvPlayer_Execute_Exit(size_t argc, char** argv);
void DtvPlayer_Execute_Hello(size_t argc, char** argv);
void DtvPlayer_Execute_Tune(size_t argc, char** argv);
void DtvPlayer_Execute_Untune(size_t argc, char** argv);
void DtvPlayer_Execute_Analyze(size_t argc, char** argv);
void DtvPlayer_Execute_Start(size_t argc, char** argv);
void DtvPlayer_Execute_Stop(size_t argc, char** argv);
void DtvPlayer_Execute_List(size_t argc, char** argv);
void DtvPlayer_Execute_StartChannel(size_t argc, char** argv);
// TODO TASK1: Add new function Scan that will automatically scan predefined frequency range
// NOTE 1: Add it here 1/3
void DtvPlayer_Execute_Scan(size_t argc, char** argv);

tCliCommands gCliCommands[] =
{
    {"help",    DtvPlayer_Execute_Help,         "Use this command to show command list."},
    {"exit",    DtvPlayer_Execute_Exit,         "Use this command to exit applications."},
    {"hello",   DtvPlayer_Execute_Hello,        "Use this command to show greeting text."},
    {"tune",    DtvPlayer_Execute_Tune,         "Use this command to tune specified frequency."},
    {"untune",  DtvPlayer_Execute_Untune,       "Use this command to untune from currently tuned frequency."},
    {"analyze", DtvPlayer_Execute_Analyze,      "Use this command to filter and analize input."},
    {"start",   DtvPlayer_Execute_Start,        "Use this command to start channel playback."},
    {"stop",    DtvPlayer_Execute_Stop,         "Use this command to stop channel playback."},
    {"list",    DtvPlayer_Execute_List,         "Use this command to list available channels."},  
    {"startcn", DtvPlayer_Execute_StartChannel, "Use this command to start channel playback."},
    // TODO TASK1: Add new function Scan that will automatically scan predefined frequency range
    // NOTE 1: Add it here 2/3
    // NOTE 2: name it scan
    {"scan",    DtvPlayer_Execute_Scan,         "Use this command to automatically scan predefined frequency range."},
};

int gCliCommandCount = sizeof(gCliCommands) / sizeof(tCliCommands);

static void DtvPlayer_Comm_Priv_ParseInputArgs(char *buffer, char** args, size_t args_size, size_t *nargs);
static void DtvPlayer_Comm_Priv_ParseInput(char *);

tErrorCode DtvPlayer_Comm_Print(const char * format, ... )
{
    char output[MESSAGE_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
  
    vprintf(format, args);

    return ePLAYER_ERROR_OK;
}

tErrorCode DtvPlayer_Comm_Read()
{
    char input[MESSAGE_BUFFER_SIZE];
    do{
        printf("\nDTV->");
        if(fgets(input, sizeof(input), stdin))
        {
            input[strcspn(input, "\n")] = '\0'; // discard end of line
        } else
        {
            continue;
        }
    } while (input[0] == 0 || input [0] == '\n');

    if(strcmp(input, "exit") == 0)
    {
        LOGD("Exit requested");
        return ePLAYER_ERROR_GENERAL;
    } else
    {
        DtvPlayer_Comm_Priv_ParseInput(input);
    }
    return ePLAYER_ERROR_OK;
}

static void DtvPlayer_Comm_Priv_ParseInputArgs(char *buffer, char** args, size_t args_size, size_t *nargs)
{
    char *buf_args[args_size]; /* You need C99 */
    char **cp;
    char *wbuf;
    size_t i, j;

    wbuf = buffer;
    buf_args[0] = buffer;
    args[0] = buffer;

    for(cp = buf_args; (*cp = strsep(&wbuf, " \n\t")) != NULL ;)
    {
        if (((int)(*cp) != '\0') && (++cp >= &buf_args[args_size]))
        {
            break;
        }
    }

    for (j=i= 0; buf_args[i] != NULL; i++)
    {
        if(strlen(buf_args[i]) > 0)
        {
            args[j++] = buf_args[i];
        }
    }

    *nargs=j;
    args[j]=NULL;
}

static void DtvPlayer_Comm_Priv_ParseInput(char *input)
{
    LOGD("Received: [%s]", input);
    char *args[MAX_ARG_COUNT];
    size_t nargs;
    int i, j;

    DtvPlayer_Comm_Priv_ParseInputArgs(input, args, MAX_ARG_COUNT, &nargs);

    uint8_t isFound = 0;
    for(i = 0; i < gCliCommandCount; i++)
    {
        if(args[0] == 0)
        {
            continue;
        }
        if ( !strcmp(args[0], gCliCommands[i].name) && gCliCommands[i].execute)
        {
            gCliCommands[i].execute(nargs, args);
            isFound = 1;
        }
    }

    if(isFound == 0)
    {
        printf("Command [%s] not found!\n", input);
    }
}

/// TODO: ADD COMMANDS HERE:
void DtvPlayer_Execute_Help(size_t argc, char** argv)
{
    int i;
    char out[MESSAGE_BUFFER_SIZE];
    out[0] = '\n';
    out[1] = 0;

    for(i = 0; i < (int)gCliCommandCount; i++)
    {
        sprintf(out, "%s%-40s %s\n", out, gCliCommands[i].name, gCliCommands[i].brief);
    }
    printf("Usage:\n%-40s %s\n%s", "command", "description", out);
}

void DtvPlayer_Execute_Exit(size_t argc, char** argv)
{
    static const char *optString = "h?";

    static const struct option longOpts[] =
    {
        { "help", no_argument, NULL, 'h' },
        { NULL, no_argument, NULL, 0 }
    };

    int opt = 0;
    int longIndex; 
    optind = 1;
    
    int apid = 0;
    int vpid = 0;

    while (opt != -1)
    {
        opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

        switch (opt)
        {
            case 'h':
            case '?':
                printf("exit # no params needed\n");
                return;
            default:
                break;
        }
    }
    LOGD("exit");
}

void DtvPlayer_Execute_Hello(size_t argc, char** argv)
{
    printf("Hello, this is DTV Exercise 1!\n");
}

void DtvPlayer_Execute_Tune(size_t argc, char** argv)
{
    static const char *optString = "f:h?";

    static const struct option longOpts[] =
    {
        { "frequency", required_argument, NULL, 'f' },
        { "help", no_argument, NULL, 'h' },
        { NULL, no_argument, NULL, 0 }
    };

    int opt = 0;
    int frequency = 0;
    int longIndex; 
    optind = 1;
    
    while (opt != -1)
    {
        opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

        switch (opt)
        {
            case 'f':
                frequency = atoi(optarg);
                break;
            case 'h':
            case '?':
                printf("tune -f <frequency> # frequency is required parameter");
                return;
            default:
                break;
        }
    }
    if(frequency == 0){
        printf("[tune] wrong arguments.");
        return;
    }
    LOGD("tune -f %d", frequency);

    DtvPlayer_Tune(frequency);
}

void DtvPlayer_Execute_Untune(size_t argc, char** argv)
{
    static const char *optString = "h?";

    static const struct option longOpts[] =
    {
        { "help", no_argument, NULL, 'h' },
        { NULL, no_argument, NULL, 0 }
    };

    int opt = 0;
    int longIndex; 
    optind = 1;
    
    while (opt != -1)
    {
        opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

        switch (opt)
        {
            case 'h':
            case '?':
                printf("untune # no params needed");
                return;
            default:
                break;
        }
    }
    LOGD("untune");
    DtvPlayer_Untune();
}

void DtvPlayer_Execute_Analyze(size_t argc, char** argv)
{
    static const char *optString = "h?";

    static const struct option longOpts[] =
    {
        { "help", no_argument, NULL, 'h' },
        { NULL, no_argument, NULL, 0 }
    };

    int opt = 0;
    int longIndex; 
    optind = 1;
    
    while (opt != -1)
    {
        opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

        switch (opt)
        {
            case 'h':
            case '?':
                printf("analyze # no params needed");
                return;
            default:
                break;
        }
    }
    LOGD("analyze");
    DtvPlayer_Analyze();
}

void DtvPlayer_Execute_Start(size_t argc, char** argv)
{
    static const char *optString = "a:v:c:d:h?";

    static const struct option longOpts[] =
    {
        { "apid", required_argument, NULL, 'a' },
        { "vpid", required_argument, NULL, 'v' },
        { "help", no_argument, NULL, 'h' },
        { NULL, no_argument, NULL, 0 }
    };

    int opt = 0;
    int longIndex; 
    optind = 1;
    
    int apid = 0;
    int vpid = 0;
    int acodec = 0;
    int vcodec = 0;

    while (opt != -1)
    {
        opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

        switch (opt)
        {
            case 'a':
                apid = atoi(optarg);
                break;
            case 'v':
                vpid = atoi(optarg);
                break;
            case 'c':
                vcodec = atoi(optarg);
                break;
            case 'd':
                acodec = atoi(optarg);
                break;
            case 'h':
            case '?':
                printf("start -a <apid> -v <vpid> -c <vcodec> -d <acodec># Audio and Video PIDS and codecs are required parametes");
                return;
            default:
                break;
        }
    }
    if(apid == 0 || vpid == 0 || acodec == 0 || vcodec == 0){
        printf("[start] wrong arguments.");
        return;
    }
    LOGD("start");
    DtvPlayer_Start(vpid, apid, vcodec, acodec);
}

void DtvPlayer_Execute_Stop(size_t argc, char** argv)
{
        static const char *optString = "h?";

    static const struct option longOpts[] =
    {
        { "help", no_argument, NULL, 'h' },
        { NULL, no_argument, NULL, 0 }
    };

    int opt = 0;
    int longIndex; 
    optind = 1;
    
    int apid = 0;
    int vpid = 0;

    while (opt != -1)
    {
        opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

        switch (opt)
        {
            case 'h':
            case '?':
                printf("stop # no params needed");
                return;
            default:
                break;
        }
    }
    LOGD("stop");
    DtvPlayer_Stop();
}

void DtvPlayer_Execute_List(size_t argc, char** argv)
{
    static const char *optString = "h?";

    static const struct option longOpts[] =
    {
        { "help", no_argument, NULL, 'h' },
        { NULL, no_argument, NULL, 0 }
    };

    int opt = 0;
    int longIndex; 
    optind = 1;
    
    int apid = 0;
    int vpid = 0;

    while (opt != -1)
    {
        opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

        switch (opt)
        {
            case 'h':
            case '?':
                DtvPlayer_Comm_Print("list # no params needed");
                return;
            default:
                break;
        }
    }
    LOGD("list");
    DtvPlayer_List();
}

void DtvPlayer_Execute_StartChannel(size_t argc, char** argv)
{
    static const char *optString = "n:h?";

    static const struct option longOpts[] =
    {
        { "number", required_argument, NULL, 'n' },
        { "help", no_argument, NULL, 'h' },
        { NULL, no_argument, NULL, 0 }
    };

    int opt = 0;
    int longIndex; 
    optind = 1;
    
    int channelNumber = -1;

    while (opt != -1)
    {
        opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

        switch (opt)
        {
            case 'n':
                channelNumber = atoi(optarg);
                break;
            case 'h':
            case '?':
                DtvPlayer_Comm_Print("startcn -n <number> # Channel number in list is required parameter");
                return;
            default:
                break;
        }
    }
    if(channelNumber <0)
    {
        DtvPlayer_Comm_Print("[start] wrong arguments.");
        return;
    }
    LOGD("start");
    DtvPlayer_StartChannel(channelNumber);
}

// TODO TASK1: Add new function Scan that will automatically scan predefined frequency range
// NOTE 1: Add it here 3/3
void DtvPlayer_Execute_Scan(size_t argc, char** argv) {

    static const char *optString = "h?";
    static const struct option longOpts[] =
    {
        { "help", no_argument, NULL, 'h' },
        { NULL, no_argument, NULL, 0 }
    };

    int opt = 0;
    int longIndex; 
    
    while (opt != -1)
    {
        opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

        switch (opt)
        {
            case 'h':
            case '?':
                DtvPlayer_Comm_Print("No arguments needed");
                return;
            default:
                break;
        }
    }
    DtvPlayer_Scan();
    DtvPlayer_Comm_Print("Scan complete.\n");
}