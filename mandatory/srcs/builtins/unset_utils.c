/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:22:09 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 23:02:13 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Counts the number of exportable environment variables.
static int	count_exportable_env_vars(t_list *envp)
{
	t_list	*env_node;
	t_env	*env_var;
	int		count;

	count = 0;
	env_node = envp;
	while (env_node)
	{
		env_var = (t_env *)env_node->content;
		if (env_var->is_export)
			count++;
		env_node = env_node->next;
	}
	return (count);
}

// Populates the environment array with exportable variables.
static char	**populate_env_array(t_minishell *minishell, char **env_array)
{
	t_list	*env_node;
	t_env	*env_var;
	int		i;

	env_node = minishell->envp;
	i = 0;
	while (env_node)
	{
		env_var = (t_env *)env_node->content;
		if (env_var->is_export)
		{
			env_array[i] = gc_strdup(minishell->gc[GC_ENV], env_var->content);
			if (!env_array[i])
				return (NULL);
			i++;
		}
		env_node = env_node->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

// Rebuilds the environment array from the exportable variables.
char	**rebuild_env_array(t_minishell *minishell)
{
	char	**new_env;
	int		env_count;

	env_count = count_exportable_env_vars(minishell->envp);
	new_env = gc_str_array_create(minishell->gc[GC_ENV], env_count
			+ ARRAY_NULL_TERMINATOR_SIZE);
	if (!new_env)
		return (NULL);
	return (populate_env_array(minishell, new_env));
}
