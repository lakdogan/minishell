/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:10:41 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:24:58 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Returns the number of exportable environment variables.
static int	env_list_size(t_list *envp)
{
	int	count;

	count = 0;
	while (envp)
	{
		if (((t_env *)envp->content)->is_export)
			count++;
		envp = envp->next;
	}
	return (count);
}

// Sorts the environment variable array alphabetically.
static void	sort_env_array(t_env **env_array, int array_size)
{
	int		current_index;
	int		comparison_index;
	t_env	*temp_env;

	current_index = FIRST_CHAR;
	while (current_index < array_size - ARRAY_LAST_ELEMENT_OFFSET)
	{
		comparison_index = current_index + NEXT_ELEMENT_INDEX;
		while (comparison_index < array_size)
		{
			if (ft_strcmp(env_array[current_index]->value,
					env_array[comparison_index]->value)
				> STRING_COMPARISON_GREATER)
			{
				temp_env = env_array[current_index];
				env_array[current_index] = env_array[comparison_index];
				env_array[comparison_index] = temp_env;
			}
			comparison_index++;
		}
		current_index++;
	}
}

// Builds a sorted array of exportable environment variables.
static t_env	**build_sorted_env_array(t_minishell *minishell, t_list *envp,
		int *size)
{
	t_env	**env_arr;
	t_list	*node;
	t_env	*env;
	int		i;

	*size = env_list_size(envp);
	env_arr = gc_alloc_struct_array(minishell->gc[GC_TEMP], (*size),
			sizeof(t_env *));
	if (!env_arr)
		return (NULL);
	node = envp;
	i = 0;
	while (node)
	{
		env = (t_env *)node->content;
		if (env->is_export)
			env_arr[i++] = env;
		node = node->next;
	}
	sort_env_array(env_arr, *size);
	return (env_arr);
}

// Prints a single export line.
static void	print_single_export_line(t_env *env)
{
	write(STDOUT_FILENO, "declare -x ", 11);
	write(STDOUT_FILENO, env->value, ft_strlen(env->value));
	if (env->content && ft_strchr(env->content, EQUALS_SIGN))
	{
		write(STDOUT_FILENO, EXPORT_EQUALS_QUOTE, 2);
		write(STDOUT_FILENO, ft_strchr(env->content, EQUALS_SIGN)
			+ SKIP_EQUALS_SIGN, ft_strlen(ft_strchr(env->content, EQUALS_SIGN)
				+ SKIP_EQUALS_SIGN));
		write(STDOUT_FILENO, EXPORT_CLOSING_QUOTE, 1);
	}
	write(STDOUT_FILENO, "\n", 1);
}

// Prints all exportable environment variables.
void	print_export(t_minishell *minishell, t_list *envp)
{
	int		size;
	int		i;
	t_env	**env_arr;

	env_arr = build_sorted_env_array(minishell, envp, &size);
	if (!env_arr)
		return ;
	i = 0;
	while (i < size)
		print_single_export_line(env_arr[i++]);
	gc_collect(minishell->gc[GC_TEMP]);
}
