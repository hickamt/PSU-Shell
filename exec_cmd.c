#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "exec_cmd.h"

void executive_commands(cmd_t *command)
{
  pid_t pid = NEG_FAIL;
  int status;
  char **argv = NULL;

  if ((pid = fork()) == NEG_FAIL)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if (pid == CHILD_PROC)
  {
    // Allocation will be NULL terminated with calloc(), no need to add argv[0] = NULL;
    argv = (char **)calloc(command->param_count + CMD_AND_NULL, sizeof(char *));
    argv = build_argv_stack(argv, command);
    if (command->input_src == REDIRECT_FILE)
    {
      input_file(command);
    }
    if (command->output_dest == REDIRECT_FILE)
    {
      output_file(command);
    }
    if (execvp(argv[0], argv) < SUCC_ZERO)
    {
      perror("execvp");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    pid = wait(&status);
  }
}

// Build: *argv[cmd, param_1, ..., param_n, NULL]
char **build_argv_stack(char *argv[], cmd_t *command)
{
  param_t *current = command->param_list;
  argv[0] = command->cmd;
  for (int i = 1; i <= command->param_count; current = current->next, ++i)
  {
    argv[i] = current->param;
  }
  return argv;
}

// Read STDIN
void input_file(cmd_t *command)
{
  int fd_in = NEG_FAIL;
  if ((fd_in = open(command->input_file_name, O_RDONLY)) < SUCC_ZERO)
  {
    perror("open");
    exit(EXIT_FAILURE);
  }
  dup2(fd_in, STDIN_FILENO);
  close(fd_in);
}

// Write STDOUT
void output_file(cmd_t *command)
{
  int fd_out = NEG_FAIL;
  if ((fd_out = open(command->output_file_name, O_APPEND | O_CREAT | O_WRONLY, S_IRWXU)) < SUCC_ZERO)
  {
    perror("open");
    exit(EXIT_FAILURE);
  }
  dup2(fd_out, STDOUT_FILENO);
  close(fd_out);
}
