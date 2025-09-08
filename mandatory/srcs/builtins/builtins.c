/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:36:58 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:19:15 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Executes a builtin command.
int	exec_builtin(t_exec *cmd, t_minishell *minishell)
{
	int	ret;

	if (!cmd || !cmd->command || !*cmd->command)
		return (BUILTIN_FAILURE);
	if (ft_strcmp(cmd->command, "echo") == STRINGS_EQUAL)
		return (ft_echo(cmd->argv, cmd->no_expand_flags,
				cmd->was_quoted_flags));
	else if (ft_strcmp(cmd->command, "cd") == STRINGS_EQUAL)
		return (ft_cd(cmd->argv, minishell));
	else if (ft_strcmp(cmd->command, "pwd") == STRINGS_EQUAL)
		return (ft_pwd());
	else if (ft_strcmp(cmd->command, "export") == STRINGS_EQUAL)
		return (ft_export(cmd->argv, minishell));
	else if (ft_strcmp(cmd->command, "unset") == STRINGS_EQUAL)
		return (ft_unset(cmd->argv, minishell));
	else if (ft_strcmp(cmd->command, "env") == STRINGS_EQUAL)
	{
		ret = ft_env(minishell, cmd->argv);
		minishell->exit_code = ret;
		return (ret);
	}
	else if (ft_strcmp(cmd->command, "exit") == STRINGS_EQUAL)
		return (ft_exit(cmd->argv, minishell));
	return (BUILTIN_NOT_FOUND);
}
