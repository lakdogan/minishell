/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:00:10 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:05:35 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Executes the command tree.
void	execute_tree(t_command_tree *root, t_minishell *minishell)
{
	if (!root || !validate_command_tree(root))
	{
		handle_invalid_tree(root, minishell);
		return ;
	}
	prepare_tree_execution(root, minishell);
	if (is_assignment_only_command(root))
	{
		handle_assignment_command(root->cmd, minishell);
		return ;
	}
	execute_node_by_type(root, minishell);
	wait_for_processes(minishell);
}
