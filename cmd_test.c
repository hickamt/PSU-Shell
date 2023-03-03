#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_parse.h"
#include "cmd_test.h"

int test_commands(void)
{
  char **history = NULL;
  char str[MAX_STR_LEN];
  char *ret_val;
  char *raw_cmd;
  cmd_list_t *cmd_list = NULL;
  char *commands[] = {
      "ls -la > test.txt\n",
      "ls -l | wc > test.txt\n",
      "cat test.txt\n",
      "cat -n /etc/passwd > test.txt\n"};

  for (int i = 0; i < 4; ++i)
  {
    fputs(commands[i], stdout);
    memset(str, 0, MAX_STR_LEN);
    ret_val = (char *)memcpy(str, commands[i], strlen(commands[i]));
    if (NULL == ret_val)
    {
      break;
    }
    if (strlen(str) == 0)
    {
      continue;
    }
    if (str[strlen(str) - 1] == '\n')
    {
      str[strlen(str) - 1] = '\0';
    }
    if (strcmp(str, QUIT_CMD) == 0)
    {
      break;
    }
    // free(history[HISTORY_COUNT - 1]);
    // for (int index = (HISTORY_COUNT - 2); index >= 0; --index)
    //   history[index + 1] = history[index];
    // history[0] = strdup(ret_val);
    raw_cmd = strtok(str, PIPE_DELIM);
    cmd_list = (cmd_list_t *)calloc(1, sizeof(cmd_list_t));
    cmd_list = build_list(raw_cmd, cmd_list);
    parse_commands(cmd_list);
    exec_commands(cmd_list, history);
    free_list(cmd_list);
    cmd_list = NULL;
  }

  return (EXIT_SUCCESS);
}
