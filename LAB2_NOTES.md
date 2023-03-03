# Lab 2 Instruction Notes

This lab introduces the basic principles of a shell. We are required to implement the following built in commands:

- quit
- cd directoryName
- cwd
- history
- echo

Further, the following requirements for external commands should be satisfied:

- a single command with any number of command line options
- allow for rediction of stdin or stdout < and >

The complete [instructions as html](/notes/lab2_notes/Lab2_Instructions.html) give a more detailed overview of the first stage requirements for this lab.

## Important Library Functions & Definitions

You will need to use several functions from the #include libraries. These links should help explain the functions and definitions of the library to narrow down which you may need to complete this lab.

- [Linux Man Pages Online](https://www.kernel.org/doc/man-pages/)
- [param.h: IMB](https://www.ibm.com/docs/en/aix/7.2?topic=files-paramh-file)
- [unistd.h: IBM](https://www.ibm.com/docs/en/zos/2.3.0?topic=files-unistdh-implementation-specific-functions)
- [stdio.h: Tech On The Net](https://www.techonthenet.com/c_language/standard_library_functions/stdio_h/index.php)
- [string.h: Tech On The Net](https://www.techonthenet.com/c_language/standard_library_functions/string_h/index.php)
- [stdlib.h: Tech On The Net](https://www.techonthenet.com/c_language/standard_library_functions/stdlib_h/index.php)

## First Steps

To begin, copy the files [cmd_pars.h, cmd_parse.c, and psush_example.c] from the instructors PSU directory.

Then, create a makefile that will compile the program (beginning with the files mentioned above) and contains the following make targets:

- all, build all out-of-date programs and prerequisites
- clean, clean up the compiled files and editor chaff
- psush, linking the program, rebuilding the psush.o file, if necessary
  -psush.o, build the psush.c file, if necessary
  if 'psush' is built from ultiple C files, you will need to have additional targets to support the additional C files

## Read the Slides & Watch the Video

Video lectures have been recorded for the first stage this lab. Watch the recording after you are familiar with the files and requirements.

Download the slides from Canvas that include an overview of [psush-pseudo, dup and dup2, pipes, ragged array].

## Run Valgrind on Compiled File

Once your makfile is complete, run Valgrind on the file entering a few command line arguments and then quit. We can run cd ../ and quit as they seem to be functional within the given .c files.

```c
// Basic stack call created with the following
$ valgrind -s --leak-check=full ./psush
// our terminal: command
psush thickam β: cd ../
psush thickam β: quit
```

The array of lists (ragged array) does not free allocated memory yet. In 'cmd_parse.c' there are two function stubs located in the cmd_parse.c file for the implementation but you will probably want to add one more for the struct param_s:

```c
void free_list(cmd_list_t *cmd_list)
void free_cmd(cmd_t *cmd)

// Add another for the deepest pointer list: cmd_list->cmd->param_list
void free_param(param_t *param_list)
```

## Methodology of Madness Thus Far

My method so far is creating the Makefile to compile the given files and test various commands to see what might work and what is still required for implementation. Next, I ran valgrind against the compiled file './psush' and began the implementation for 'free()' on each allocation that would be possible.

To better understand what each pointer is taking in from the command line I built, valgrind tested, and completed the free portion without memory leaks.

- NOTE: use instructors handy 'print_cmd(cmd)' method in cmd_parse.c

```c
void free_list(cmd_list_t *cmd_list)
{
    cmd_t *current = cmd_list->head;
    while (current)
    {
        free_cmd(current);
	// other shit goes here (did you free the head?)
    }
// walked this list, anything left to free here?
}

void free_cmd(cmd_t *cmd)
{
// within the struct cmd_s are the following pointers that may or may not be allocated during the programs execution:
•	raw_cmd
•	cmd
•	input_file_name
•	output_file_name

// and, there is another struct pointer to (param_s) which is why I created another function to free the char* ptr it holds:
	free_param(cmd->param_list)

// nothing left to free after calling free_param(), function stops and free_list() picks up where it left off
}

// Remember to add the correct .h definition to cmd_parse.h
void free_param(param_t *param_list)
{
    param_t *current = param_list;
    while (current)
    {
      free(current->param);
// other shit goes here (like, freeing the current param and moving forward/next?)
    }

// walked this list, anything left to free here?
}
```

## Now, Implement First Built-In Command: Quit

No, don't quit! Implement a way to exit our shell and name the command 'quit'.

Instructor Note:

The **quit** built-in command will exit **psush**. If the user types a **Control-D** at the command line (as the first character on the line), the result will be the same as the **quit** build-in command.

From the Manual Pages:

```c
$ man exit

// Basic Overview
exit: causes the shell to exit, duh
```

In looking over the comd_parse.c file, there is a method that compares the command line string with "quit". If equal, then a **break** is invoked and the system will exit. This has already been implemented.

## Implement ( cd < dir > )

In testing, the **cd** command allows me to move up a directory but not forward given a file name. CD is being implemented, sort of, in cmd_parse.c inside of the exec_commands() function.

```c
// Take a look at the manual
$ man cd
```

Also, [Ubuntu Manpages for (cd)](https://manpages.ubuntu.com/manpages/bionic/en/man1/cd.1posix.html)

From the code in cmd_parse.c, and instructor notes, we can use an already working command ( cwd ) to get the current working directory. If we are moving INTO a directory, then append the given directory string to this path and use the function ( chdir(const char \*\_\_path)) to change paths.

- Use the standard library (stdlib.h) and other libraries included in our header file to look for the functions that will make this easier. Also, environment variables such as "HOME" will be helpful.

```c
// chdir() is part of the <unistd.h> library
extern int chdir (const char *__path) __THROW __nonnull ((1)) __wur;

// getenv() is part of the <stdlib.h> library
extern char *getenv (const char *__name) __THROW __nonnull ((1)) __wur;
```

## CWD, Already Implemented

The ( cwd ) command is already showing my current working directory. Code used:

```c
// Console Out Current Working Directory
else if (0 == strcmp(cmd->cmd, CWD_CMD))
{
    char str[MAXPATHLEN];
    getcwd(str, MAXPATHLEN);
    printf(" " CWD_CMD ": %s\n", str);
}
```

## Implement ECHO

To begin, I used the instructors pre-built 'print_cmd(cmd)' function to understand which pointer holds the 'paramter' string to echo back to console.

## Implement history

Create the code for the built-in command **history**

Take a look at the instructor slides in the history scheme. This is in the psush-pseudo slide available on Canvas.

Note the code implementation in the first slide is pushing each command line (str) to the end of the array.

![History Slide 1](/notes/img/history_1.png)
![History Slide 2](/notes/img/history_2.png)
![History Slide 2](/notes/img/history_3.png)

# External Commands | Part II of Lab2

I **HIGHLY** recommend you take a look through these videos, especially those towards the end of the lecture course.

- [Intro to Systems Programming in C](https://www.youtube.com/playlist?list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO)

External Commands are all of the other commands available in Linux. The following links to the online manual pages for Linix/POSIX are part of the Lab2 **external commands** used for testing your psu shell (psush):

- [cat: Man Pages](https://man7.org/linux/man-pages/man1/cat.1.html)
- [ls: Man Pages](https://man7.org/linux/man-pages/man1/ls.1.html)
- [w: Man Pages](https://man7.org/linux/man-pages/man1/w.1.html)
- [wc: Man Pages](https://man7.org/linux/man-pages/man1/wc.1.html)
- [who: Man Pages](https://man7.org/linux/man-pages/man1/who.1.html)
- [cmp: Man Pages](https://man7.org/linux/man-pages/man1/cmp.1.html)
- [file: Man Pages](https://man7.org/linux/man-pages/man1/file.1.html)
- [grep: Man Pages](https://man7.org/linux/man-pages/man1/grep.1.html)

## Handle External Commands | execvp()

To handle external commands with **redirects** you will probably use a combination of:

- [dup2(): Man Pages](https://www.man7.org/linux/man-pages/man2/dup.2.html)
- [fork(): Man Pages](https://man7.org/linux/man-pages/man2/fork.2.html)
- [execvp(): Man Pages](https://manpages.org/execvp)
- [wait(): Man Pages](https://man7.org/linux/man-pages/man2/wait.2.html)
- [open(2): Man Pages](https://www.man7.org/linux/man-pages/man2/openat.2.html)
- [read(2): Man Pages](https://man7.org/linux/man-pages/man2/read.2.html)
- [write(2): Man Pages](https://man7.org/linux/man-pages/man2/write.2.html)

As a hint, you will use an expansion of the following code along with the implementation of dup2(), fork(), and wait():

```c
char *command[] = {"grep", "root", "/etc/passwd"};
char *bin_file = command[0];
if (execvp(bin_file, command) == -1)
{
fprintf(stderr, "Error executing %s\n", bin_file);
}
```

## Redirects

You will probably want to create a switch statement using the typedef enum{} given in the .h file.

- REDIRECT_NONE = 0
- REDIRECT_FILE = 1
- REDIRECT_PIPE = 2
- BACKGROUND_PROC = 3

As 'c' is so magnificent, it does not have a switch statement that allows for string arguments :<,>

- [Overview of Redirects: STDIN, STDOUT, STDERROR](https://www.linuxscrew.com/redirect-stdin-stdout-stderr-bash)
