/**
 * @file exit.c
 * @brief Implementation of the exit built-in command
 *
 * This file contains the implementation of the exit built-in command which
 * terminates the shell with the specified exit code. It handles various
 * error conditions such as non-numeric arguments and too many arguments.
 *
 * @author lakdogan <lakdogan@student.42.fr>
 * @date 2025-05-19
 */

#include "../../includes/core/minishell.h"

/**
 * @brief Checks if a string represents a valid numeric value
 *
 * Determines if the given string contains only digits, possibly with a
 * leading plus or minus sign. This is used to validate that the exit
 * argument is a valid numeric exit code.
 *
 * @param str The string to check
 * @return bool true if the string is numeric, false otherwise
 */
static bool	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

void    cleanup_memory(t_minishell *minishell)
{
    int i;

    i = 0;
    while(i < GC_COUNT)
    {
        gc_cleanup(&minishell->gc[i]);
        i++;
    }
}


/**
 * @brief Implements the exit built-in command
 *
 * Terminates the shell with the specified exit code. The behavior is:
 * - If no argument is provided, exit with the current exit code
 * - If a non-numeric argument is provided, display an error and exit with 255
 * - If too many arguments are provided,
	display an error and return 1 (do not exit)
 * - Otherwise, exit with the specified code (truncated to unsigned char)
 *
 * @param argv Array of command arguments (argv[0] is "exit",
	argv[1] is the exit code)
 * @param minishell Pointer to the shell state containing the current exit code
 * @return int 1 if too many arguments are provided, otherwise does not return
 */
int	ft_exit(char **argv, t_minishell *minishell)
{
	long	code;

	write(STDERR_FILENO, "exit\n", 5);
	if (!argv[1])
    {
        cleanup_memory(minishell);
		exit(minishell->exit_code);
    }
	if (!is_numeric(argv[1]))
	{
		write(STDERR_FILENO, "minishell: exit: ", 18);
		write(STDERR_FILENO, argv[1], ft_strlen(argv[1]));
		write(STDERR_FILENO, ": numeric argument required\n", 29);
        cleanup_memory(minishell);
		exit(255);
	}
	if (argv[2])
	{
		write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	code = ft_atol(argv[1]);
    cleanup_memory(minishell);
	exit((unsigned char)code);
}
