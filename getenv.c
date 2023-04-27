#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @inf: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_environ(info_a *inf)
{
	if (!inf->environ || inf->env_changed)
	{
		inf->environ = list_to_strings(inf->env);
		inf->env_changed = 0;
	}

	return (inf->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @inf: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _unsetenv(info_a *inf, char *var)
{
	list_a *node = inf->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			inf->env_changed = delete_node_at_index(&(inf->env), i);
			i = 0;
			node = inf->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (inf->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @inf: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _setenv(info_a *inf, char *var, char *value)
{
	char *buff = NULL;
	list_a *node;
	char *p;

	if (!var || !value)
		return (0);

	buff = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buff)
		return (1);
	_strcpy(buff, var);
	_strcat(buff, "=");
	_strcat(buff, value);
	node = inf->env;
	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buff;
			inf->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(inf->env), buff, 0);
	free(buff);
	inf->env_changed = 1;
	return (0);
}

