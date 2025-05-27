/**
 * @file export_print.c
 * @brief Functions for printing environment variables in export format
 *
 * This file contains functions to print environment variables in the format
 * used by the export built-in command, including alphabetical sorting and
 * proper quoting of values.
 *
 */

#include "../../includes/core/minishell.h"

/**
 * @brief Counts the number of environment variables marked for export
 *
 * Traverses the environment list and counts only variables that have
 * the is_export flag set.
 *
 * @param envp Linked list of environment variables
 * @return int Number of exportable environment variables
 */
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

/**
 * @brief Sorts an array of environment variables alphabetically by name
 *
 * Uses a simple bubble sort algorithm to arrange environment variables
 * in alphabetical order based on their value (name) field.
 *
 * @param arr Array of environment variable pointers to sort
 * @param size Number of elements in the array
 */
static void	sort_env_array(t_env **arr, int size)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (ft_strncmp(arr[i]->value, arr[j]->value, SIZE_MAX) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

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

/**
 * @brief Prints a single environment variable in export format
 *
 * Formats and prints a single environment variable in the format:
 * declare -x NAME="VALUE" or declare -x NAME if the variable has no value.
 * Values are enclosed in double quotes for proper display.
 *
 * @param env Pointer to the environment variable to print
 */
static void	print_single_export_line(t_env *env)
{
	write(STDOUT_FILENO, "declare -x ", 11);
	write(STDOUT_FILENO, env->value, ft_strlen(env->value));
	if (env->content && ft_strchr(env->content, '='))
	{
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, ft_strchr(env->content, '=') + 1,
			ft_strlen(ft_strchr(env->content, '=') + 1));
		write(STDOUT_FILENO, "\"", 1);
	}
	write(STDOUT_FILENO, "\n", 1);
}

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
