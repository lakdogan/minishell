/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_left_cmd_prepare.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:47:12 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:16:45 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Checks exec outfiles and prepares pipes.
void	check_exec_outfile_and_prepare_pipes(t_minishell *shell, t_exec *exec,
		int *pipefd)
{
	safe_close(shell, pipefd[PIPE_READ_END], "pipe close read end");
	if (!exec->outfiles)
		safe_dup2(shell, pipefd[PIPE_WRITE_END], STDOUT_FILENO,
			"pipe left command stdout");
	safe_close(shell, pipefd[PIPE_WRITE_END], "pipe close write end after dup");
}

// Checks if right command has infile.
void	check_if_right_cmd_has_infile(t_command_tree *node, t_exec **right_exec,
		bool *right_has_input)
{
	*right_exec = node->right->data;
	*right_has_input = (*right_exec && (*right_exec)->infiles != NULL);
}

// Prepares for left command execution.
void	prepare_for_execute(t_minishell *shell, t_command_tree *node,
		t_exec *exec, int *pipefd)
{
	t_exec	*right_exec;
	bool	right_has_input;

	right_has_input = false;
	if (node->right->type == NODE_CMD)
		check_if_right_cmd_has_infile(node, &right_exec, &right_has_input);
	setup_in_and_output_redirections(shell, exec);
	if (right_has_input)
		close_pipe(shell, pipefd);
	else
		check_exec_outfile_and_prepare_pipes(shell, exec, pipefd);
}
