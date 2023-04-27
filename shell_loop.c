#include "shell.h"

/**
 * hsh – It is the main shell loop
 * @inf: It is the parameter & return info struct
 * @avec: It is the argument vector from main()
 *
 * Return: 0 if successful, 1 if error occurs, or error code
 */
int hsh(info_a *inf, char **avec)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info(inf);
		if (interac(inf))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		r = get_input(inf);
		if (r != -1)
		{
			set_info(inf, avec);
			builtin_ret = find_builtin(inf);
			if (builtin_ret == -1)
				find_cmd(inf);
		}
		else if (interac(inf))
			_putchar('\n');
		free_info(inf, 0);
	}
	write_history(inf);
	free_info(inf, 1);
	if (!interac(inf) && inf->status)
		exit(inf->status);
	if (builtin_ret == -2)
	{
		if (inf->err_number == -1)
			exit(inf->status);
		exit(inf->err_number);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @inf: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 * 	0 if builtin executed successfully,
 * 	1 if builtin found but not successful,
 * 	2 if builtin signals exit()
 */
int find_builtin(info_a *inf)
{
	int i, built_in_ret = -1;
	built_t builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].tp; i++)
		if (_strcmp(inf->arga[0], builtintbl[i].tp) == 0)
		{
			inf->line_ch++;
			built_in_ret = builtintbl[i].fun(inf);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * @inf: the parameter & return info struct
 *
 * Return: void
 */
void find_cmd(info_a *inf)
{
	char *path = NULL;
	int i, k;

	inf->path = inf->arga[0];
	if (inf->linec_flag == 1)
	{
		inf->line_ch++;
		inf->linec_flag = 0;
	}
	for (i = 0, k = 0; inf->argu[i]; i++)
		if (!is_delimeter(inf->argu[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(inf, _getenv(inf, "PATH="), inf->arga[0]);
	if (path)
	{
		inf->path = path;
		fork_cmd(inf);
	}
	else
	{
		if ((interac(inf) || _getenv(inf, "PATH=")
					|| inf->arga[0][0] == '/') && is_cmd(inf, inf->arga[0]))
			fork_cmd(inf);
		else if (*(inf->argu) != '\n')
		{
			inf->status = 127;
			print_error(inf, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @inf: the parameter & return info struct
 *
 * Return: void
 */
void fork_cmd(info_a *inf)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(inf->path, inf->arga, get_environ(inf)) == -1)
		{
			free_info(inf, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(inf->status));
		if (WIFEXITED(inf->status))
		{
			inf->status = WEXITSTATUS(inf->status);
			if (inf->status == 126)
				print_error(inf, "Permission denied\n");
		}
	}
}
