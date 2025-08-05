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

	i = FIRST_CHAR;
	if (!argv || !argv[i] || !(ft_isalpha(argv[i]) || argv[i] == UNDERSCORE))
		return (IDENTIFIER_INVALID);
	i++;
	while (argv[i] && argv[i] != EQUALS_SIGN)
	{
		if (!ft_isalnum(argv[i]) && argv[i] != UNDERSCORE)
			return (IDENTIFIER_INVALID);
		i++;
	}
	return (IDENTIFIER_VALID);
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
	int	status;

	status = BUILTIN_SUCCESS;
	i = COMMAND_ARGS_START;
	if (!argv[i])
	{
		print_export(minishell, minishell->envp);
		return (BUILTIN_SUCCESS);
	}
	while (argv[i])
	{
		if (!is_valid_export_key(argv[i]))
		{
			write(STDERR_FILENO, "export: `", 9);
			write(STDERR_FILENO, argv[i], ft_strlen(argv[i]));
			write(STDERR_FILENO, "': not a valid identifier\n", 27);
			status = BUILTIN_FAILURE;
		}
		else
			update_or_add_env(argv[i], minishell);
		i++;
	}
	minishell->envp_arr = rebuild_env_array(minishell);
	return (status);
}
