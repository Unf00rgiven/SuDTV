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
// TODO: Verify how tune and untune functions are defined as possible command
void DtvPlayer_Execute_Tune(size_t argc, char** argv);
void DtvPlayer_Execute_Untune(size_t argc, char** argv);

tCliCommands gCliCommands[] =
{
    {"help",    DtvPlayer_Execute_Help,         "Use this command to show command list."},
    {"exit",    DtvPlayer_Execute_Exit,         "Use this command to exit applications."},
    {"hello",   DtvPlayer_Execute_Hello,        "Use this command to show greeting text."},
    // TODO: Verify how tune and untune functions are defined as possible command
    {"tune",    DtvPlayer_Execute_Tune,         "Use this command to tune specified frequency."},
    {"untune",  DtvPlayer_Execute_Untune,       "Use this command to untune from currently tuned frequency."},
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
        if ((*cp != '\0') && (++cp >= &buf_args[args_size]))
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

// TODO: Verify how tune and untune functions are defined as possible command
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

// TODO: Verify how tune and untune functions are defined as possible command
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
                DtvPlayer_Comm_Print("untune # no params needed");
                return;
            default:
                break;
        }
    }
    LOGD("untune");
    DtvPlayer_Untune();
}