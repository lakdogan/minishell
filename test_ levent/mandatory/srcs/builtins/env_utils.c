/**
 * @file env_utils.c
 * @brief Utility functions for environment variable management
 *
 * This file contains utility functions for getting, setting, and
 * manipulating environment variables in the shell.
 */

#include "../../includes/core/minishell.h"

/**
 * @brief Gets the value of an environment variable
 *
 * Searches the environment for a variable with the specified name
 * and stores its value in the result parameter.
 *
 * @param name Name of the environment variable to get
 * @param minishell Pointer to the shell state structure
 * @param result Pointer to store the result (will point to existing memory,
 *               do not free)
 * @return int 0 if the variable was found, 1 if not
 */
int	get_env_value(const char *name, t_minishell *minishell, char **result)
{
	t_list	*node;
	t_env	*env;

	if (!name || !result)
		return (1);
	node = minishell->envp;
	while (node)
	{
		env = (t_env *)node->content;
		if (ft_strncmp(env->value, name, SIZE_MAX) == 0)
		{
			if (env->content && ft_strchr(env->content, '='))
				*result = ft_strchr(env->content, '=') + 1;
			else
				*result = NULL;
			return (0);
		}
		node = node->next;
	}
	*result = NULL;
	return (1);
}

/**
 * @brief Sets the value of an environment variable
 *
 * Sets or updates an environment variable with the specified name and value.
 * If the variable exists, its value is updated. If not,
	a new variable is created.
 *
 * @param name Name of the environment variable to set
 * @param value Value to set (can be NULL)
 * @param minishell Pointer to the shell state structure
 * @return int 0 on success, 1 on failure
 */
int	set_env_var(const char *name, const char *value, t_minishell *minishell)
{
	t_list *node;
	t_env *env;
	char *key;
	char *val;

	if (!name || !minishell)
		return (1);
	key = gc_strdup(minishell->gc[GC_TEMP], name);
	if (value)
		val = gc_strdup(minishell->gc[GC_TEMP], value);
	else
		val = NULL;
	node = minishell->envp;
	while (node)
	{
		env = (t_env *)node->content;
		if (ft_strncmp(env->value, name, SIZE_MAX) == 0)
		{
			update_existing_env(minishell, env, key, val);
			return (0);
		}
		node = node->next;
	}
	add_new_env(key, val, minishell);
	return (0);
}
