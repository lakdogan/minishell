/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:14:47 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:10:11 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Closes pipe file descriptors and restores stdout.
void	close_file_descriptors(t_minishell *shell, t_pipe_state *state)
{
	close_pipe(shell, state->pipefd);
	close(state->stdout_backup);
}

// Waits for both child processes to finish.
void	wait_for_children(t_minishell *shell, pid_t left_pid, pid_t right_pid)
{
	wait_for_process(left_pid, shell);
	wait_for_process(right_pid, shell);
}

// Restores stdout from backup.
void	restore_stdout(t_pipe_state *state)
{
	if (state->stdout_backup != INVALID_FD)
	{
		dup2(state->stdout_backup, STDOUT_FILENO);
		close(state->stdout_backup);
	}
}

// Cleans up pipe resources and waits for children.
void	cleanup_pipe(t_minishell *shell, t_pipe_state *state)
{
	close_pipe(shell, state->pipefd);
	wait_for_children(shell, state->left_pid, state->right_pid);
	restore_stdout(state);
}
