#include "shell.h"

/**
 * interac – It returns true if shell is interactive mode
 * @inf: struct address
 *
 * Return: 1 if interactive mode, else 0
 */
int interac(info_a *inf)
{
	return (isatty(STDIN_FILENO) && inf->rfd <= 2);
}

/**
 * is_delimeter - checking if the character is a delimeter
 * @ch: Is the char to check
 * @delimeter: Is the delimeter string
 * Return: 1 if true, else 0
 */
int is_delimeter(char ch, char *delimeter)
{
	while (*delimeter)
		if (*delimeter++ == ch)
			return (1);
	return (0);
}

/**
 * _isalpha – checking for alphabetic character
 * @ch: The character to input
 * Return: 1 if ch is alphabetic, 0 otherwise
 */

int _isalpha(int ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * _atoi - converts a string to an integer
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 */

int _atoi(char *s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0; s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}

