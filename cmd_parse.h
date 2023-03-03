// rchaney@pdx.edu

#ifndef _CMD_PARSE_H
#define _CMD_PARSE_H

#define MAX_STR_LEN 2000
#define RDWR_PERM 0666
#define HISTORY_COUNT 10
#define CMD_AND_NULL 2
#define STACK_END(__count) (int)(__count + 1)

#define CD_CMD "cd"
#define CWD_CMD "cwd"
#define ECHO_CMD "echo"
#define QUIT_CMD "quit"
#define HISTORY "history"
#define LIST_FILES "ls"
#define CAT_CMD "cat"
#define R_READ "r"
#define W_WRITE "w"

#define PIPE_DELIM "|"
#define SPACE_DELIM " "
#define REDIR_IN "<"
#define REDIR_OUT ">"
#define PROMPT_STR "\x1b[33mpsush\x1b[0m \x1b[32m@\x1b[0m"

// This enumeration is used when determining redirection
// characters (the < and >) were used on a command.
typedef enum
{
    REDIRECT_NONE,
    REDIRECT_FILE,
    REDIRECT_PIPE,
    BACKGROUND_PROC
} redir_t;

// A list of param_t elements.
typedef struct param_s
{
    char *param;
    struct param_s *next;
} param_t;

// A linked list that has a linked list as a member.
typedef struct cmd_s
{
    char *raw_cmd;
    char *cmd;
    int param_count;
    param_t *param_list;
    redir_t input_src;
    redir_t output_dest;
    char *input_file_name;
    char *output_file_name;
    int list_location; // zero based
    struct cmd_s *next;
} cmd_t;

typedef struct cmd_list_s
{
    cmd_t *head;
    cmd_t *tail;
    int count;
} cmd_list_t;

void simple_argv(int argc, char *argv[]);
int process_user_input_simple(char *history[]);
struct cmd_list_s *build_list(char *raw_cmd, struct cmd_list_s *cmd_list);
void parse_commands(struct cmd_list_s *cmd_list);
void exec_commands(struct cmd_list_s *cmds, char *history[]);
void print_list(struct cmd_list_s *);
void print_cmd(struct cmd_s *);
void free_list(struct cmd_list_s *);
void free_cmd(struct cmd_s *);
void free_param(struct param_s *);
void free_history(char *history[]);
void help_menu(void);
// void cat_file(char *argv[], struct cmd_s *command);

#endif // _CMD_PARSE_H
