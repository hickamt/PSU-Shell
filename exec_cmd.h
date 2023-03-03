#ifndef _EXEC_CMD_H
#define _EXEC_CMD_H

#define FILE_EXEC_SIZE 3
#define NEG_FAIL -1
#define SUCC_ZERO 0
#define CHILD_PROC 0

#include "cmd_parse.h"

void executive_commands(struct cmd_s *cmds);
char **build_argv_stack(char *argv[], struct cmd_s *command);
void input_file(struct cmd_s *command);
void output_file(struct cmd_s *command);

#endif // _EXEC_CMD_H