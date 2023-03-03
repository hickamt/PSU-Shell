// rchaney@pdx.edu
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/stat.h>

#include "cmd_parse.h"

// Global Verbose Print
extern unsigned short isVerbose;

int main(int argc, char *argv[])
{
    int ret = 0;
    char *history[HISTORY_COUNT] = {NULL};
    umask(0);
    memset(history, 0, sizeof(history));
    simple_argv(argc, argv);
    ret = process_user_input_simple(history);
    free_history(history);
    return (ret);
}
