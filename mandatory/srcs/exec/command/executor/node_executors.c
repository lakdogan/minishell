/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_executors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 20:02:41 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 21:04:36 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Process an execution node in the command tree */
void	handle_exec(t_command_tree *node, t_minishell *minishell)
{
	t_exec	*exec;

	if (!node)
		return ;
	exec = (t_exec *)node->data;
	if (handle_empty_cmd_with_redir(exec, minishell))
		return ;
	expand_tilde_in_args(exec, minishell);
	if (is_builtin(exec->command))
	{
		execute_builtin_cmd(exec, minishell);
		return ;
	}
	handle_var_expansion_exec(minishell, exec);
	handle_external(exec, minishell);
}
