/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modifier.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:13:51 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 19:17:04 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

/* Add a new environment variable */
void	add_new_env(char *key, char *value, t_minishell *minishell)
{
	t_env	*new_env;
	t_list	*new_node;

	if (!key)
		return ;
	new_env = gc_malloc(minishell->gc[GC_MAIN], sizeof(t_env));
	new_env->value = gc_strdup(minishell->gc[GC_MAIN], key);
	if (value)
		new_env->content = gc_strjoin_3(minishell->gc[GC_MAIN], key, "=",
				value);
	else
		new_env->content = NULL;
	new_env->is_export = true;
	new_env->printed = false;
	new_node = gc_lstnew(minishell->gc[GC_MAIN], new_env);
	gc_lstadd_back(minishell->gc[GC_MAIN], &minishell->envp, new_node);
	gc_collect(minishell->gc[GC_TEMP]);
}

/* Update an existing environment variable */
void	update_existing_env(t_minishell *minishell, t_env *env, char *key,
		char *value)
{
	if (value)
	{
		env->value = gc_strdup(minishell->gc[GC_ENV], key);
		env->content = gc_strjoin_3(minishell->gc[GC_ENV], key, "=", value);
	}
	env->is_export = true;
	gc_collect(minishell->gc[GC_TEMP]);
}

/* Update an existing env var or add a new one */
void	update_or_add_env(const char *arg, t_minishell *minishell)
{
	char	*key;
	char	*value;
	t_env	*env;

	key = extract_key(minishell, arg);
	value = extract_value(minishell, arg);
	env = find_env_by_key(minishell, key);
	if (env)
		update_existing_env(minishell, env, key, value);
	else
		add_new_env(key, value, minishell);
	gc_collect(minishell->gc[GC_TEMP]);
	minishell->envp_arr = rebuild_env_array(minishell);
}
