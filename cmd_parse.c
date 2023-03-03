// rchaney@pdx.edu

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#include "cmd_parse.h"
#include "exec_cmd.h"
#include "pipe_cmd.h"
#include "cmd_test.h"

unsigned short isVerbose = 0;

void help_menu(void)
{
  printf("\n\x1b[32mSimple Built-In Shell Commands: others handled by execvp()\x1b[0m:"
         "\n> \x1b[94mabout\x1b[0m\t\t\t[ check out the README.md on GitHub for Lab2 & Lab3 ]"
         "\n> \x1b[94mcd <dir>\x1b[0m\t\t[ change the current directory ]"
         "\n> \x1b[94mcwd\x1b[0m\t\t\t[ console out current working directory (similar to pwd) ]"
         "\n> \x1b[94mecho <string(s)>\x1b[0m\t[ console back the string(s) ]"
         "\n> \x1b[94mhelp\t\t\x1b[0m\t[ You're reading it ]"
         "\n> \x1b[94mhistory\x1b[0m\t\t[ get the last 10 commands entered ]"
         "\n> \x1b[94mquit\x1b[0m\t\t\t[ exit the shell ]"
         "\n> \x1b[94m[CTRL + d]\x1b[0m\t\t[ end of file, similar to exit ]"
         "\n\t- Helpful for ending a child process of the shell."
         "\n\t- Try: $ bash [ENTER], then to exit [CTRL + d]"
         "\n\n\x1b[32mMakefile Commands\x1b[0m:"
         "\n$ \x1b[94mmake\x1b[0m\t\t\t[ creates the executable (psush) and .o ]"
         "\n$ \x1b[94mmake all\x1b[0m\t\t[ same as make, no other program besides psush in this directory ]"
         "\n$ \x1b[94mmake clean\x1b[0m\t\t[ removes *.o, *.txt, and psush.exe ]"
         "\n$ \x1b[94mmake cls\x1b[0m\t\t[ short for $ make clean, for lazy coders :> ]"
         "\n$ \x1b[94mmake canvas\x1b[0m\t\t[ cleans directory & builds the username-directory-execname.tar.gz ]"
         "\n$ \x1b[94mmake ci\x1b[0m\t\t[ builds RCS revisioning directory if !found, lazy save ]"
         "\n\n\x1b[32mShell Exec Command Examples\x1b[0m:"
         "\n> \x1b[94mls -l\x1b[0m\t\t\t\t[ list of sub-folders and/or files in the current working directory ]"
         "\n> \x1b[94mcat -n /etc/passwd > JUNK\x1b[0m\t[ write out the /etc/passwd to JUNK ]"
         "\n> \x1b[94mcat JUNK\x1b[0m\t\t\t[ see what the previous command wrote to JUNK ]"
         "\n> \x1b[94mls -la -F -h -t -r | cat -n | wc -l -w -c -L\x1b[0m\t\t\n\n");
}

void simple_argv(int argc, char *argv[])
{
  int opt;
  while ((opt = getopt(argc, argv, "hvt")) != -1)
  {
    switch (opt)
    {
    case 'h':
      help_menu();
      exit(EXIT_SUCCESS);
      break;
    // run: $ ./psush -v [ verbose mode: to see how list parses the commands, params, redirects, and pipes ]
    case 'v':
      ++isVerbose;
      if (isVerbose)
      {
        fprintf(stderr, "verbose mode is on: %d\n", isVerbose);
      }
      break;
    case 't':
      test_commands();
      exit(EXIT_SUCCESS);
      break;
    case '?':
      fprintf(stderr, "*** Unknown option used, ignoring. ***\n");
      break;
    default:
      fprintf(stderr, "*** Oops, something strange happened <%c> ... ignoring ...***\n", opt);
      break;
    }
  }
}

int process_user_input_simple(char *history[])
{
  char str[MAX_STR_LEN];
  char *ret_val;
  char *raw_cmd;
  cmd_list_t *cmd_list = NULL;
  char prompt[55];

  for (;;)
  {
    sprintf(prompt, PROMPT_STR " \x1b[94m%s\x1b[0m \x1b[32mβ\x1b[0m > ", getcwd(NULL, 0));
    fputs(prompt, stdout);
    memset(str, 0, MAX_STR_LEN);
    ret_val = fgets(str, MAX_STR_LEN, stdin);
    // [CTRL + d] EOF, or end of commands
    if (NULL == ret_val)
    {
      break;
    }
    // If Command Line Is Empty, continue
    if (strlen(str) == 0)
    {
      continue;
    }
    // STOMP on the pesky trailing newline returned from fgets().
    if (str[strlen(str) - 1] == '\n')
    {
      str[strlen(str) - 1] = '\0';
    }
    // If Exit Command "quit", then break out of shell
    if (strcmp(str, QUIT_CMD) == 0)
    {
      break;
    }
    // COMMAND LINE HISTORY ALLOCATION
    // if (hist->history[HISTORY_COUNT - 1])
    free(history[HISTORY_COUNT - 1]);
    for (int index = (HISTORY_COUNT - 2); index >= 0; --index)
      history[index + 1] = history[index];
    history[0] = strdup(ret_val);
    // END OF HISTORY ALLOCATION
    // *** Basic commands are pipe delimited.
    // *** This is really for Stage 2.
    raw_cmd = strtok(str, PIPE_DELIM);
    cmd_list = (cmd_list_t *)calloc(1, sizeof(cmd_list_t));
    // Build Linked List
    cmd_list = build_list(raw_cmd, cmd_list);
    // Now that I have a linked list of the pipe delimited commands,
    // go through each individual command.
    parse_commands(cmd_list);
    // This is a really good place to call a function to exec the
    // the commands just parsed from the user's command line.
    // exec_commands(cmd_list);
    exec_commands(cmd_list, history);
    // We (that includes you) need to free up all the stuff we just
    // allocated from the heap. That linked list of linked lists looks
    // like it will be nasty to free up, but just follow the memory.
    free_list(cmd_list);
    cmd_list = NULL;
  }

  return (EXIT_SUCCESS);
}

cmd_list_t *build_list(char *raw_cmd, cmd_list_t *cmd_list)
{
  int cmd_count = 0;
  while (raw_cmd != NULL)
  {
    cmd_t *cmd = (cmd_t *)calloc(1, sizeof(cmd_t));
    cmd->raw_cmd = strdup(raw_cmd);
    cmd->list_location = (cmd_count++);
    if (cmd_list->head == NULL)
    {
      cmd_list->tail = cmd_list->head = cmd;
    }
    else
    {
      cmd_list->tail->next = cmd;
      cmd_list->tail = cmd;
    }
    cmd_list->count++;
    raw_cmd = strtok(NULL, PIPE_DELIM);
  }
  return cmd_list;
}

// Instructor Crazy Code
// alloca: allocates on stack
#define stralloca(_R, _S)          \
  {                                \
    (_R) = alloca(strlen(_S) + 1); \
    strcpy(_R, _S);                \
  }

// Instructors Parse List
void parse_commands(cmd_list_t *cmd_list)
{
  cmd_t *cmd = cmd_list->head;
  char *arg;
  char *raw;
  while (cmd)
  {
    stralloca(raw, cmd->raw_cmd);
    arg = strtok(raw, SPACE_DELIM);
    if (NULL == arg)
    {
      cmd = cmd->next;
      continue;
    }
    if (arg[0] == '\'')
    {
      arg++;
    }
    if (arg[strlen(arg) - 1] == '\'')
    {
      arg[strlen(arg) - 1] = '\0';
    }
    cmd->cmd = strdup(arg);
    // Give input and output source type of 'redir_t'
    cmd->input_src = REDIRECT_NONE;
    cmd->output_dest = REDIRECT_NONE;
    // the arg = strtok(NULL, " ") will parse each argument with a NULL termination character
    while ((arg = strtok(NULL, SPACE_DELIM)) != NULL)
    {
      if (strcmp(arg, REDIR_IN) == 0)
      {
        // Directive: STDIN_FILENO (0)
        cmd->input_file_name = strdup(strtok(NULL, SPACE_DELIM));
        cmd->input_src = REDIRECT_FILE;
      }
      else if (strcmp(arg, REDIR_OUT) == 0)
      {
        // Directive: STDOUT_FILENO (1)
        cmd->output_file_name = strdup(strtok(NULL, SPACE_DELIM));
        cmd->output_dest = REDIRECT_FILE;
      }
      else
      {
        // add next param
        param_t *param = (param_t *)calloc(1, sizeof(param_t));
        param_t *cparam = cmd->param_list;
        cmd->param_count++;
        // Put something in here to strip out the single quotes if
        // they are the first/last characters in arg.
        if (arg[0] == '\'')
        {
          arg++;
        }
        if (arg[strlen(arg) - 1] == '\'')
        {
          arg[strlen(arg) - 1] = '\0';
        }
        param->param = strdup(arg);
        if (NULL == cparam)
        {
          cmd->param_list = param;
        }
        else
        {
          // I should put a tail pointer on this.
          while (cparam->next != NULL)
          {
            cparam = cparam->next;
          }
          cparam->next = param;
        }
      }
    }
    // This could overwite some bogus file redirection.
    if (cmd->list_location > 0)
    {
      cmd->input_src = REDIRECT_PIPE;
    }
    if (cmd->list_location < (cmd_list->count - 1))
    {
      cmd->output_dest = REDIRECT_PIPE;
    }
    // No need to free when using alloca memory.
    // free(raw);
    cmd = cmd->next;
  }
  if (isVerbose > 0)
  {
    print_list(cmd_list);
  }
}

void exec_commands(cmd_list_t *cmd, char *history[])
{
  cmd_t *command = cmd->head;
  if (1 == cmd->count)
  {
    char path[MAXPATHLEN];
    if (!command->cmd)
    {
      return;
    }
    // CHANGE DIRECTORY: $ CD <FILE>
    else if (0 == strcmp(command->cmd, CD_CMD))
    {
      if (0 == command->param_count)
      {
        chdir(getenv("HOME"));
      }
      else
      {
        if (0 != chdir(command->param_list->param))
        {
          printf("\n(%s) is not a valid directory\n", command->param_list->param);
        }
      }
    }
    // CURRENT WORKING DIRECTORY: $ CWD
    else if (0 == strcmp(command->cmd, CWD_CMD))
    {
      printf(" " CWD_CMD ": %s\n", getcwd(path, MAXPATHLEN));
    }
    // This Echo Function is unecessary with execvp() working
    else if (0 == strcmp(command->cmd, ECHO_CMD))
    {
      if (!command->input_src && !command->output_dest && command->param_count > 0)
      {
        param_t *current = NULL;
        current = command->param_list;
        while (current)
        {
          printf("%s ", current->param);
          current = current->next;
        }
        printf("\n");
      }
      else
      {
        executive_commands(command);
      }
    }
    // DISPLAY HISTORY DETAILS: $ HISTORY
    else if (0 == strcmp(command->cmd, HISTORY))
    {
      int count = 0;
      printf("CLI HISTORY:\n");
      for (int index = (HISTORY_COUNT - 1); index >= 0; --index)
      {
        if (history[index])
        {
          printf("(%d)\t %s\n", ++count, history[index]);
        }
      }
      printf("\n");
    }
    // Once Command with 0 or more redirects
    else if (0 == strcmp(command->cmd, "help") || 0 == strcmp(command->cmd, "-h"))
    {
      help_menu();
    }
    else if (0 == strcmp(command->cmd, "about"))
    {
      pid_t pid = NEG_FAIL;
      int status;
      if ((pid = fork()) == NEG_FAIL)
      {
        perror("fork");
        exit(EXIT_FAILURE);
      }
      if (pid == CHILD_PROC)
      {
        char *argv[] = {"open", "https://github.com/hickamt/CS333-Labs"};
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
    else
    {
      executive_commands(command);
    }
  }
  // Pipe Seperated Commands
  else
  {
    pipe_commands(cmd, command);
  }
}

void print_list(cmd_list_t *cmd_list)
{
  cmd_t *cmd = cmd_list->head;
  while (NULL != cmd)
  {
    print_cmd(cmd);
    cmd = cmd->next;
  }
}

void print_cmd(cmd_t *cmd)
{
  param_t *param = NULL;
  int pcount = 1;
  fprintf(stderr, "raw text: +%s+\n", cmd->raw_cmd);
  fprintf(stderr, "\tbase command: +%s+\n", cmd->cmd);
  fprintf(stderr, "\tparam count: %d\n", cmd->param_count);
  param = cmd->param_list;
  while (NULL != param)
  {
    fprintf(stderr, "\t\tparam %d: %s\n", pcount, param->param);
    param = param->next;
    pcount++;
  }
  fprintf(stderr, "\tinput source: %s\n", (cmd->input_src == REDIRECT_FILE ? "redirect file" : (cmd->input_src == REDIRECT_PIPE ? "redirect pipe" : "redirect none")));
  fprintf(stderr, "\toutput dest:  %s\n", (cmd->output_dest == REDIRECT_FILE ? "redirect file" : (cmd->output_dest == REDIRECT_PIPE ? "redirect pipe" : "redirect none")));
  fprintf(stderr, "\tinput file name:  %s\n", (NULL == cmd->input_file_name ? "<na>" : cmd->input_file_name));
  fprintf(stderr, "\toutput file name: %s\n", (NULL == cmd->output_file_name ? "<na>" : cmd->output_file_name));
  fprintf(stderr, "\tlocation in list of commands: %d\n", cmd->list_location);
  fprintf(stderr, "\n");
}

void free_list(cmd_list_t *cmd_list)
{
  cmd_t *current = cmd_list->head;
  while (current)
  {
    free_cmd(current);
    current = current->next;
    free(cmd_list->head);
    cmd_list->head = current;
  }
  cmd_list->head = cmd_list->tail = NULL;
  if (cmd_list)
    free(cmd_list);
}

void free_cmd(cmd_t *cmd)
{
  if (cmd->raw_cmd)
    free(cmd->raw_cmd);
  if (cmd->cmd)
    free(cmd->cmd);
  if (cmd->input_file_name)
    free(cmd->input_file_name);
  if (cmd->output_file_name)
    free(cmd->output_file_name);
  if (cmd->param_list)
    free_param(cmd->param_list);
}

void free_param(param_t *param_list)
{
  param_t *current = param_list;
  while (current)
  {
    if (current->param)
      free(current->param);
    current = current->next;
    free(param_list);
    param_list = current;
  }
}

void free_history(char *history[])
{
  for (int i = 0; i < HISTORY_COUNT; ++i)
  {
    if (history[i])
    {
      free(history[i]);
    }
  }
}
