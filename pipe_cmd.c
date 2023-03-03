#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "pipe_cmd.h"
#include "exec_cmd.h"

void pipe_commands(cmd_list_t *cmd, cmd_t *command)
{
  pid_t pid = NEG_FAIL;
  int pipes[2];
  int fd_in = STDIN_FILENO;
  char **argv = NULL;

  while (command)
  {
    // Open pipes, if not the last command
    if (command != cmd->tail)
    {
      pipe(pipes);
    }
    // Create Child Process
    if ((pid = fork()) < CHILD_PROC)
    {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    // Execute Commands/Redirects
    if (pid == CHILD_PROC)
    {
      // only the child process will require allocation, and is freed upon execvp()
      argv = (char **)calloc(command->param_count + CMD_AND_NULL, sizeof(char *));
      argv = build_argv_stack(argv, command);
      // dup2() stream only if command is not the first
      if (command != cmd->head)
      {
        dup2(fd_in, STDIN_FILENO);
      }
      // Handle File_In Redirect
      if (command->input_src == REDIRECT_FILE)
      {
        input_file(command);
      }
      // Handle File_Out Redirect
      if (command->output_dest == REDIRECT_FILE)
      {
        output_file(command);
      }
      // Handle Standard Out for Next Command IFF it exists
      if (command->next)
      {
        dup2(pipes[STDOUT_FILENO], STDOUT_FILENO);
        close(pipes[STDIN_FILENO]);
        close(pipes[STDOUT_FILENO]);
      }
      // Does not return on success, if() condition is just verbose
      if (execvp(argv[0], argv) < SUCC_ZERO)
      {
        perror("execvp");
        exit(EXIT_FAILURE);
      }
    }
    // close STDIN if NOT the first command
    if (command != cmd->head)
    {
      close(fd_in);
    }
    // close both STDIN && STDOUT while NOT the last command
    if (command != cmd->tail)
    {
      close(pipes[STDOUT_FILENO]);
      fd_in = pipes[STDIN_FILENO];
    }
    // get next command
    command = command->next;
  }
  // Parent needs to wait for ALL children
  // if wait() is called and no child_PID processes the return is neg 1
  // else, return is zero or greater as (whole number)
  while ((wait(NULL)) >= CHILD_PROC)
    ;
}
