#include "shell.h"

/**
 * get_history_file - gets the history file
 * @inf: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_history_file(info_a *inf)
{
	char *buff, *dir;

	dir = _getenv(inf, "HOME=");
	if (!dir)
		return (NULL);
	buff = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buff)
		return (NULL);
	buff[0] = 0;
	_strcpy(buff, dir);
	_strcat(buff, "/");
	_strcat(buff, HIST_FILE);
	return (buff);
}

/**
 * write_history - creates a file, or appends to an existing file
 * @inf: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history(info_a *inf)
{
	ssize_t fd;
	char *filename = get_history_file(inf);
	list_a *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = inf->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history - reads history from file
 * @inf: the parameter struct
 *
 * Return: hcount on success, 0 otherwise
 */
int read_history(info_a *inf)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buff = NULL, *filename = get_history_file(inf);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buff = malloc(sizeof(char) * (fsize + 1));
	if (!buff)
		return (0);
	rdlen = read(fd, buff, fsize);
	buff[fsize] = 0;
	if (rdlen <= 0)
		return (free(buff), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buff[i] == '\n')
		{
			buff[i] = 0;
			build_history_list(inf, buff + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_history_list(inf, buff + last, linecount++);
	free(buff);
	inf->hcount = linecount;
	while (inf->hcount-- >= HIST_MAX)
		delete_node_at_index(&(inf->history), 0);
	renumber_history(inf);
	return (inf->hcount);
}

/**
 * build_history_list - adds entry to a history linked list
 * @inf: Structure containing potential arguments. Used to maintain
 * @buff: buffer
 * @linecount: the history linecount, hcount
 *
 * Return: Always 0
 */
int build_history_list(info_a *inf, char *buff, int linecount)
{
	list_a *node = NULL;

	if (inf->history)
		node = inf->history;
	add_node_end(&node, buff, linecount);

	if (!inf->history)
		inf->history = node;
	return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @inf: Structure containing potential arguments. Used to maintain
 *
 * Return: the new hcount
 */
int renumber_history(info_a *inf)
{
	list_a *node = inf->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (inf->hcount = i);
}

