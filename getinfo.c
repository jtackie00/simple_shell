#include "shell.h"

/**
 * clear_info - initializes info_a struct
 * @inf: struct address
 */
void clear_info(info_a *inf)
{
	inf->argu = NULL;
	inf->arga = NULL;
	inf->path = NULL;
	inf->argpc = 0;
}

/**
 * set_info - initializes info_a struct
 * @inf: struct address
 * @avec: argument vector
 */
void set_info(info_a *inf, char **avec)
{
	int i = 0;

	inf->fn = avec[0];
	if (inf->argu)
	{
		inf->arga = strtow(inf->argu, " \t");
		if (!inf->arga)
		{
			inf->arga = malloc(sizeof(char *) * 2);
			if (inf->arga)
			{
				inf->arga[0] = _strdup(inf->argu);
				inf->arga[1] = NULL;
			}
		}
		for (i = 0; inf->arga && inf->arga[i]; i++)
			;
		inf->argpc = i;

		repl_alias(inf);
		repl_vars(inf);
	}
}

/**
 * free_info - frees info_a struct fields
 * @inf: struct address
 * @all: true if freeing all fields
 */
void free_info(info_a *inf, int all)
{
	ffree(inf->arga);
	inf->arga = NULL;
	inf->path = NULL;
	if (all)
	{
		if (!inf->cmd_buf)
			free(inf->argu);
		if (inf->env)
			free_list(&(inf->env));
		if (inf->history)
			free_list(&(inf->history));
		if (inf->alias)
			free_list(&(inf->alias));
		ffree(inf->environ);
			inf->environ = NULL;
		bfree((void **)inf->cmd_buf);
		if (inf->rfd > 2)
			close(inf->rfd);
		_putchar(BUF_FLUSH);
	}
}

