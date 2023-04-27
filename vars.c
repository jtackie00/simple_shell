#include "shell.h"

/**
 * is_chain - test if current char in buffer is a chain delimeter
 * @inf: the parameter struct
 * @buff: the char buffer
 * @p: address of current position in buff
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int is_chain(info_a *inf, char *buff, size_t *p)
{
	size_t j = *p;

	if (buff[j] == '|' && buff[j + 1] == '|')
	{
		buff[j] = 0;
		j++;
		inf->cmd_buf_type = CMD_OR;
	}
	else if (buff[j] == '&' && buff[j + 1] == '&')
	{
		buff[j] = 0;
		j++;
		inf->cmd_buf_type = CMD_AND;
	}
	else if (buff[j] == ';') /* found end of this command */
	{
		buff[j] = 0; /* replace semicolon with null */
		inf->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * ch_chain – checking if we should continue chaining based on last status
 * @inf: the parameter struct
 * @buff: the char buffer
 * @p: address of current position in buff
 * @i: starting position in buff
 * @len: length of buff
 *
 * Return: Void
 */
void ch_chain(info_a *inf, char *buff, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (inf->cmd_buf_type == CMD_AND)
	{
		if (inf->status)
		{
			buff[i] = 0;
			j = len;
		}
	}
	if (inf->cmd_buf_type == CMD_OR)
	{
		if (!inf->status)
		{
			buff[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * repl_alias – It replaces an aliases in the tokenized string
 * @inf: It holds the parameter struct
 *
 * Return: 1 if replaced, else 0
 */
int repl_alias(info_a *inf)
{
	int i;
	list_a *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(inf->alias, inf->arga[0], '=');
		if (!node)
			return (0);
		free(inf->arga[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		inf->arga[0] = p;
	}
	return (1);
}

/**
 * repl_vars – It replaces vars in the tokenized string
 * @inf: It is the parameter struct
 *
 * Return: 1 if replaced, else 0 
 */
int repl_vars(info_a *inf)
{
	int i = 0;
	list_a *node;

	for (i = 0; inf->arga[i]; i++)
	{
		if (inf->arga[i][0] != '$' || !inf->arga[i][1])
			continue;

		if (!_strcmp(inf->arga[i], "$?"))
		{
			repl_str(&(inf->arga[i]),
					_strdup(convert_number(inf->status, 10, 0)));
			continue;
		}
		if (!_strcmp(inf->arga[i], "$$"))
		{
			repl_str(&(inf->arga[i]),
					_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(inf->env, &inf->arga[i][1], '=');
		if (node)
		{
			repl_str(&(inf->arga[i]),
					_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		repl_str(&inf->arga[i], _strdup(""));

	}
	return (0);
}

/**
 * repl_str – It replaces string
 * @old_one: It shows the address of old string
 * @new_one: It is a new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int repl_str(char **old_one, char *new_one)
{
	free(*old_one);
	*old_one = new_one;
	return (1);
}
