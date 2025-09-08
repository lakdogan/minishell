/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:10:18 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:04:41 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Executes a node based on its type.
void	execute_node_by_type(t_command_tree *node, t_minishell *minishell)
{
	if (node->type == NODE_CMD)
		handle_exec(node, minishell);
	else if (node->type == NODE_PIPE)
		handle_pipe(minishell, node);
	else if (node->type == NODE_SEQUENCE)
	{
		execute_tree(node->left, minishell);
		execute_tree(node->right, minishell);
	}
}

// Waits for all child processes to finish.
void	wait_for_processes(t_minishell *minishell)
{
	int	status;

	if (!minishell->in_nested_pipe)
	{
		while (waitpid(WAIT_ANY, &status, WAIT_BLOCK) > NO_PROCESS)
			continue ;
	}
	minishell->in_nested_pipe = false;
}
