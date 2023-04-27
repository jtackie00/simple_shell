#include "shell.h"

/**
 * main - entry point
 * @ac: argu count
 * @avec: argu vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **avec)
{
	info_a inf[] = { INFO_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
			"add $3, %0"
			: "=r" (fd)
			: "r" (fd));

	if (ac == 2)
	{
		fd = open(avec[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(avec[0]);
				_eputs(": 0: Can't open ");
				_eputs(avec[1]);
				_eputchar('\n');
				_eputchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		inf->rfd = fd;
	}
	populate_env_list(inf);
	read_history(inf);
	hsh(inf, avec);
	return (EXIT_SUCCESS);
}

