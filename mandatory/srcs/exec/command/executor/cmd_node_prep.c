/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_node_prep.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 20:04:18 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 23:24:08 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Handle empty command with redirections */
int	handle_empty_cmd_with_redir(t_exec *exec, t_minishell *minishell)
{
	const char	*filename;

	if ((!exec->argv || !exec->argv[0]) && (exec->infiles || exec->outfiles))
	{
		filename = NULL;
		if (exec->outfiles && exec->outfiles->name)
			filename = exec->outfiles->name;
		else if (exec->infiles && exec->infiles->name)
			filename = exec->infiles->name;
		if (filename)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd((char *)filename, STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			minishell->exit_code = 127;
		}
		else
			minishell->exit_code = 127;
		return (1);
	}
	return (0);
}

/* Expand tilde in command arguments */
void	expand_tilde_in_args(t_exec *exec, t_minishell *minishell)
{
	int		i;
	char	*tmp;

	if (exec && exec->argv)
	{
		i = 1;
		while (exec->argv[i])
		{
			if (!exec->no_expand_flags || !exec->no_expand_flags[i])
			{
				tmp = expand_tilde(exec->argv[i], minishell);
				exec->argv[i] = tmp;
			}
			i++;
		}
	}
}
