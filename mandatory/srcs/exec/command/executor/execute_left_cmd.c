/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_left_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:47:25 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:16:18 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Handles standard left commands.
void	handle_standard_left_cmds(t_minishell *shell, t_command_tree *node,
		int *pipefd)
{
	t_exec	*exec;

	exec = node->left->data;
	check_exec_has_valid_data(shell, exec);
	prepare_for_execute(shell, node, exec, pipefd);
	execute_builtin_or_external_cmd(shell, exec);
}

// Sets up default pipe output for left command.
void	setup_default_pipe_output(t_minishell *shell, int *pipefd)
{
	safe_close(shell, pipefd[PIPE_READ_END], "pipe close read end");
	safe_dup2(shell, pipefd[PIPE_WRITE_END], STDOUT_FILENO,
		"pipe left command stdout");
	safe_close(shell, pipefd[PIPE_WRITE_END], "pipe close write end after dup");
}

// Executes the left subtree of a pipe node.
void	execute_left_subtree(t_minishell *shell, t_command_tree *node,
		int *pipefd)
{
	if (!node || !node->left)
		return ;
	if (node->left->type == NODE_CMD)
		handle_standard_left_cmds(shell, node, pipefd);
	else if (node->left->type == NODE_PIPE)
	{
		setup_default_pipe_output(shell, pipefd);
		execute_tree(node->left, shell);
	}
	else
	{
		setup_default_pipe_output(shell, pipefd);
		execute_tree(node->left, shell);
	}
}

// Executes the left node by its type.
static void	execute_left_by_type(t_minishell *shell, t_command_tree *node,
		int *pipefd)
{
	if (node->left->type == NODE_CMD)
		handle_standard_left_cmds(shell, node, pipefd);
	else
		execute_left_subtree(shell, node, pipefd);
}

// Entry point for executing the left command in a pipe.
void	execute_left_cmd(t_command_tree *node, t_minishell *shell, int *pipefd)
{
	setup_child_signals();
	execute_left_by_type(shell, node, pipefd);
	exit(shell->exit_code);
}
