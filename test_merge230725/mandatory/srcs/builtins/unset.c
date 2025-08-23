/**
 * @file unset.c
 * @brief Implementation of the unset built-in command
 *
 * This file contains the implementation of the unset built-in command which
 * removes environment variables from the shell environment. It handles
 * validation of variable names and proper memory cleanup during removal.
 */

#include "../../includes/core/minishell.h"

/**
 * @brief Validates if a string is a valid environment variable identifier
 *
 * Checks if the provided string meets the requirements for a valid shell
 * variable name:
 * - Must not be empty
 * - First character must be a letter or underscore (not a digit)
 * - All characters must be alphanumeric or underscore
 *
 * @param str The string to validate
 * @return int 1 if the string is a valid identifier, 0 otherwise
 */
static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[FIRST_CHAR] || ft_isdigit(str[FIRST_CHAR]))
		return (IDENTIFIER_INVALID);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != UNDERSCORE)
			return (IDENTIFIER_INVALID);
		i++;
	}
	return (IDENTIFIER_VALID);
}

static void	remove_env_var(const char *name, t_minishell *minishell)
{
	t_list	*prev;
	t_list	*node;
	t_list	*to_remove;
	t_env	*env;

	prev = NULL;
	node = minishell->envp;
	while (node)
	{
		env = (t_env *)node->content;
		if (env && ft_strcmp(env->value, name) == STRINGS_EQUAL)
		{
			to_remove = node;
			if (prev)
				prev->next = node->next;
			else
				minishell->envp = node->next;
			node = node->next;
			// Note: We don't free here since it's managed by GC
			return; // Found and removed, exit
		}
		prev = node;
		node = node->next;
	}
}

/**
 * @brief Implements the unset built-in command
 *
 * Removes one or more environment variables from the shell environment.
 * For each argument, validates that it's a valid identifier and then
 * attempts to remove it from the environment. Displays an error message
 * for invalid identifiers.
 *
 * @param argv Array of command arguments (argv[0] is "unset")
 * @param minishell Pointer to the shell state structure
 * @return int Always returns 0 (success)
 */
int	ft_unset(char **argv, t_minishell *minishell)
{
	int	i;

	if (!argv || !argv[1])  // Wenn keine Argumente gegeben sind, ist das OK
		return (BUILTIN_SUCCESS);

	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			write(STDERR_FILENO, "unset: `", 8);
			write(STDERR_FILENO, argv[i], ft_strlen(argv[i]));
			write(STDERR_FILENO, "': not a valid identifier\n", 27);
			// Nicht exit(1) hier - das würde die ganze Shell beenden
			// return (BUILTIN_FAILURE);  // Stattdessen Failure zurückgeben
		}
		else
			remove_env_var(argv[i], minishell);
		i++;
	}
	minishell->envp_arr = rebuild_env_array(minishell);
	return (BUILTIN_SUCCESS);
}
