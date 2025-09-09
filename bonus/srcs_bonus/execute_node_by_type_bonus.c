/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_by_type_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:53:24 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 21:30:15 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

// Executes the node based on its type.
void	execute_node_by_type_bonus(t_command_tree *node, t_minishell *minishell)
{
	if (!node)
		return ;
	if (node->type == NODE_CMD)
		handle_exec(node, minishell);
	else if (node->type == NODE_PIPE)
	{
		if (minishell->in_subshell == true)
			handle_pipe_in_subshell(minishell, node);
		else
			handle_pipe(minishell, node);
	}
	else if (node->type == NODE_SEQUENCE)
	{
		execute_tree(node->left, minishell);
		execute_tree(node->right, minishell);
	}
	else if (node->type == NODE_ANDIF)
		handle_and_operator(minishell, node);
	else if (node->type == NODE_OR)
		handle_or_operator(minishell, node);
	else if (node->type == NODE_SUBSHELL)
		execute_subshell(node, minishell);
}
