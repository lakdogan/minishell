/**
 * @file export.c
 * @brief Implementation of the export built-in command
 *
 * This file contains the implementation of the
 * export built-in command which
 * displays all environment variables in
 * sorted order when called without arguments,
 * or adds/updates environment variables
 * when provided with arguments in the format
 * KEY=VALUE or KEY.
 */

#include "../../includes/core/minishell.h"

/**
 * @brief Validates if a string is a valid environment variable key
 *
 * Checks if the provided string meets the requirements for a valid shell
 * environment variable name:
 * - Must not be empty
 * - First character must be a letter or underscore (not a digit)
 * - All characters must be alphanumeric or underscore
 *
 * @param argv The string to validate
 * @return int 1 if the string is a valid key, 0 otherwise
 */
static int	is_valid_export_key(const char *argv)
{
	int	i;

	i = 0;
	if (!argv || !argv[0] || ft_isdigit(argv[0]))
		return (0);
	while (argv[i] && argv[i] != '=')
	{
		if (!ft_isalnum(argv[i]) && argv[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Implements the export built-in command
 *
 * When called without arguments, prints all environment
 * variables in alphabetical
 * order with "declare -x" prefix and proper quoting.
 * When called with arguments, for each valid argument:
 * - If the variable doesn't exist, adds it to the environment
 * - If the variable exists, updates its value
 * - If the argument doesn't contain '=', marks the variable for export
 * Invalid arguments produce an error message and are skipped.
 *
 * @param argv Array of command arguments (argv[0] is "export")
 * @param minishell Pointer to the shell state structure
 * @return int 0 on success, 1 if any argument was invalid
 */
int	ft_export(char **argv, t_minishell *minishell)
{
	int	i;

	i = 1;
	if (!argv[1])
	{
		print_export(minishell, minishell->envp);
		return (0);
	}
	while (argv[i])
	{
		if (!is_valid_export_key(argv[i]))
		{
			write(2, "export: `", 9);
			write(2, argv[i], ft_strlen(argv[i]));
			write(2, "': not a valid identifier\n", 27);
		}
		else
			update_or_add_env(argv[i], minishell);
		i++;
	}
    minishell->envp_arr = rebuild_env_array(minishell);
	return (0);
}
