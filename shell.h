#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>

/* for reading and writing buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for chaining the command */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* 1 if you are using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct strlists - singly linked list
 * @number: contains the number field
 * @str: Is a string
 * @pc: moves to the next
 */
typedef struct strlists
{
	int number;
	char *str;
	struct strlists *pc;
} list_a;

/**
 * struct passdata - containing pseudo-arguements to pass data into a function,
 * which will allow uniform prototype for function pointer struct
 * @argu: It is a string generated from getline containing arguements
 * @arga: Is an array of strings generated from argu
 * @path: It is a string path for the current command
 * @argpc: It is the argument program count
 * @line_c: It is the error count
 * @err_number: It is the error code for exit()s
 * @linec_flag: if on, count this line of input
 * @fn: It is the program filename
 * @env: It is the linked list local copy of environ
 * @environ: It is the custom modified copy of environ from LL env
 * @history: It is the history node
 * @alias: Is the alias node
 * @env_changed: on, if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, on if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @rfd: It is the fd from which to read line input
 * @hcount: It is the history line number count
 */
typedef struct passdata
{
	char *argu;
	char **arga;
	char *path;
	int argpc;
	unsigned int line_c;
	int err_number;
	int linec_flag;
	char *fn;
	list_a *env;
	list_a *history;
	list_a *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int rfd;
	int hcount;
} info_a;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
		0, 0, 0}

/**
 * struct built_in – It contains a builtin string and related function
 * @tp: It is the builtin command flag
 * @fun: It is the function
 */
typedef struct built_in
{
	char *tp;
	int (*fun)(info_a *);
} built_t;


/* toem_shloop.c */
int hsh(info_a *, char **);
int find_builtin(info_a *);
void find_cmd(info_a *);
void fork_cmd(info_a *);

/* toem_parser.c */
int is_cmd(info_a *, char *);
char *dup_chars(char *, int, int);
char *find_path(info_a *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* toem_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int bfree(void **);

/* toem_atoi.c */
int interac(info_a *);
int is_delimeter(char, char *);
int _isalpha(int);
int _atoi(char *);

/* toem_errors1.c */
int _erratoi(char *);
void print_error(info_a *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* toem_builtin.c */
int _myexit(info_a *);
int _mycd(info_a *);
int _myhelp(info_a *);

/* toem_builtin1.c */
int _myhistory(info_a *);
int _myalias(info_a *);

/*toem_getline.c */
ssize_t get_input(info_a *);
int _getline(info_a *, char **, size_t *);
void sigintHandler(int);

/* toem_getinfo.c */
void clear_info(info_a *);
void set_info(info_a *, char **);
void free_info(info_a *, int);

/* toem_environ.c */
char *_getenv(info_a *, const char *);
int _myenv(info_a *);
int _mysetenv(info_a *);
int _myunsetenv(info_a *);
int populate_env_list(info_a *);

/* toem_getenv.c */
char **get_environ(info_a *);
int _unsetenv(info_a *, char *);
int _setenv(info_a *, char *, char *);

/* toem_history.c */
char *get_history_file(info_a *inf);
int write_history(info_a *inf);
int read_history(info_a *inf);
int build_history_list(info_a *inf, char *buf, int linecount);
int renumber_history(info_a *inf);

/* toem_lists.c */
list_a *add_node(list_a **, const char *, int);
list_a *add_node_end(list_a **, const char *, int);
size_t print_list_str(const list_a *);
int delete_node_at_index(list_a **, unsigned int);
void free_list(list_a **);

/* toem_lists1.c */
size_t list_len(const list_a *);
char **list_to_strings(list_a *);
size_t print_list(const list_a *);
list_a *node_starts_with(list_a *, char *, char);
ssize_t get_node_index(list_a *, list_a *);

/* toem_vars.c */
int is_chain(info_a *, char *, size_t *);
void ch_chain(info_a *, char *, size_t *, size_t, size_t);
int repl_alias(info_a *);
int repl_vars(info_a *);
int repl_str(char **, char *);

#endif

