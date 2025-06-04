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

static int	update_existing_env_var(t_env *env, const char *name,
		const char *value, t_minishell *minishell)
{
	char	*full_var;

	if (value)
	{
		full_var = gc_strjoin_3(minishell->gc[GC_ENV], name, "=", value);
		if (!full_var)
			return (1);
		env->content = full_var;
	}
	env->is_export = 1;
	return (0);
}

static int	create_new_env_var(const char *name, const char *value,
		t_minishell *minishell)
{
	t_env	*env;
	char	*full_var;

	env = gc_alloc_struct(minishell->gc[GC_ENV], sizeof(t_env));
	if (!env)
		return (1);
	env->value = gc_strdup(minishell->gc[GC_ENV], name);
	if (!env->value)
		return (1);
	env->content = NULL;
	if (value)
	{
		full_var = gc_strjoin_3(minishell->gc[GC_ENV], name, "=", value);
		if (!full_var)
			return (1);
		env->content = full_var;
	}
	env->is_export = 1;
	gc_lstadd_back(minishell->gc[GC_ENV], &(minishell->envp), env);
	return (0);
}

/**
 * @brief Finds an environment variable by name
 *
 * @param name The name to search for
 * @param minishell The shell state structure
 * @return t_env* The found environment variable, or NULL if not found
 */
static t_env	*find_env_var(const char *name, t_minishell *minishell)
{
	t_list	*node;
	t_env	*env;

	node = minishell->envp;
	while (node)
	{
		env = (t_env *)node->content;
		if (ft_strcmp(env->value, name) == 0)
			return (env);
		node = node->next;
	}
	return (NULL);
}

int	set_env_var(const char *name, const char *value, t_minishell *minishell)
{
	t_env	*existing_env;

	if (!name || !minishell)
		return (1);
	existing_env = find_env_var(name, minishell);
	if (existing_env)
		return (update_existing_env_var(existing_env, name, value, minishell));
	else
		return (create_new_env_var(name, value, minishell));
}
