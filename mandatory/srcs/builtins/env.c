/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:09:26 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 20:59:58 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

/* Print all environment variables */
static int	print_environment(t_minishell *minishell)
{
	t_list	*node;
	t_env	*env;

	node = minishell->envp;
	while (node)
	{
		env = (t_env *)node->content;
		if (env && env->is_export && env->content)
			ft_putendl_fd(env->content, STDOUT_FILENO);
		node = node->next;
	}
	return (BUILTIN_SUCCESS);
}

/* Main env command implementation */
int	ft_env(t_minishell *minishell, char **argv)
{
	int		flags;
	int		cmd_index;
	char	**custom_env;

	if (!argv || !argv[0])
		return (BUILTIN_FAILURE);
	flags = parse_env_args(argv, &cmd_index);
	if (flags < 0)
		return (BUILTIN_FAILURE);
	minishell->env_flags = flags;
	if (argv[cmd_index])
	{
		if (!argv[cmd_index][0])
		{
			ft_putstr_fd("minishell: env: '': No such file or directory\n",
				STDERR_FILENO);
			return (127);
		}
		custom_env = build_env_for_command(minishell, argv, flags, cmd_index);
		return (execute_env_command(argv, cmd_index, custom_env, minishell));
	}
	else if (flags & ENV_FLAG_EMPTY)
		return (BUILTIN_SUCCESS);
	else
		return (print_environment(minishell));
}
