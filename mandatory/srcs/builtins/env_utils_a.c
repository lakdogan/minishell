/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_a.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:02:53 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 22:50:29 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Updates an existing environment variable.
static int	update_existing_env_var(t_env *env, const char *name,
		const char *value, t_minishell *minishell)
{
	char	*full_var;

	if (value)
	{
		full_var = gc_strjoin_3(minishell->gc[GC_ENV], name, EQUALS_SIGN_STR,
				value);
		if (!full_var)
			return (BUILTIN_FAILURE);
		env->content = full_var;
	}
	env->is_export = true;
	return (BUILTIN_SUCCESS);
}

// Creates a new environment variable.
static int	create_new_env_var(const char *name, const char *value,
		t_minishell *minishell)
{
	t_env	*env;
	char	*full_var;

	if (!name || !*name)
		return (BUILTIN_FAILURE);
	env = gc_alloc_struct(minishell->gc[GC_ENV], sizeof(t_env));
	if (!env)
		return (BUILTIN_FAILURE);
	env->value = gc_strdup(minishell->gc[GC_ENV], name);
	if (!env->value)
		return (BUILTIN_FAILURE);
	if (value)
		full_var = gc_strjoin_3(minishell->gc[GC_ENV], name, EQUALS_SIGN_STR,
				value);
	else
		full_var = gc_strjoin_3(minishell->gc[GC_ENV], name, EQUALS_SIGN_STR,
				"");
	if (!full_var)
		return (BUILTIN_FAILURE);
	env->content = full_var;
	env->is_export = true;
	gc_lstadd_back(minishell->gc[GC_ENV], &(minishell->envp), env);
	return (BUILTIN_SUCCESS);
}

// Finds an environment variable by name.
t_env	*find_env_var(const char *name, t_minishell *minishell)
{
	t_list	*node;
	t_env	*env;

	node = minishell->envp;
	while (node)
	{
		env = (t_env *)node->content;
		if (env && env->value && ft_strcmp(env->value, name) == STRINGS_EQUAL)
			return (env);
		node = node->next;
	}
	return (NULL);
}

// Sets or updates an environment variable.
int	set_env_var(const char *name, const char *value, t_minishell *minishell)
{
	t_env	*existing_env;
	char	*clean_value;

	clean_value = remove_all_quotes(minishell, value);
	if (!name || !minishell)
		return (BUILTIN_FAILURE);
	existing_env = find_env_var(name, minishell);
	if (existing_env)
		return (update_existing_env_var(existing_env, name, clean_value,
				minishell));
	else
		return (create_new_env_var(name, clean_value, minishell));
}
