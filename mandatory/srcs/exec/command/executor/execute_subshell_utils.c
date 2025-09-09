/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 21:08:11 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 21:34:51 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

void	child_subshell(t_command_tree *node, t_minishell *minishell,
		int pipe_fd[2])
{
	close(pipe_fd[0]);
	minishell->in_subshell = true;
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(pipe_fd[1]);
	execute_node_by_type(node->left, minishell);
	exit(minishell->exit_code);
}
