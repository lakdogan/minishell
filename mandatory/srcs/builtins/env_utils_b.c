/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_b.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:09:16 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 19:29:51 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

/* Parse env command arguments to determine flags and command position */
int	parse_env_args(char **argv, int *cmd_index)
{
	int	i;
	int	flags;

	flags = 0;
	i = 1;
	if (argv[i] && ft_strcmp(argv[i], "-i") == 0)
	{
		flags |= ENV_FLAG_EMPTY;
		i++;
	}
	while (argv[i] && ft_strchr(argv[i], '='))
		i++;
	*cmd_index = i;
	return (flags);
}

/* Free a dynamically allocated environment array */
void	free_env_array(char **env_array, int count)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (i < count)
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

/* Determine if we need to create a new environment array */
static char	**get_default_env(t_minishell *minishell, int flags, int var_count)
{
	if (var_count <= 0)
	{
		if (flags & ENV_FLAG_EMPTY)
			return (NULL);
		else
			return (minishell->envp_arr);
	}
	return (NULL);
}

/* Create the custom environment array with command variables */
static char	**create_env_array(char **argv, int start_idx, int var_count)
{
	char	**new_env;
	int		i;

	new_env = malloc((var_count + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < var_count)
	{
		new_env[i] = ft_strdup(argv[start_idx + i]);
		i++;
	}
	new_env[var_count] = NULL;
	return (new_env);
}

/* Build custom environment array for command execution */
char	**build_env_for_command(t_minishell *minishell, char **argv, int flags,
		int cmd_index)
{
	int		var_count;
	int		start_idx;
	char	**default_env;

	if (flags & ENV_FLAG_EMPTY)
		start_idx = 2;
	else
		start_idx = 1;
	var_count = cmd_index - start_idx;
	default_env = get_default_env(minishell, flags, var_count);
	if (default_env != NULL)
		return (default_env);
	return (create_env_array(argv, start_idx, var_count));
}
