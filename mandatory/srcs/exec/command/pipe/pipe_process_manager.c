/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_process_manager.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:30:10 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 21:14:01 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Setup pipe redirections for a child process */
void	setup_pipe_redirections(int i, int num_cmds, int prev_fd, int *pipefd)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (i < num_cmds - 1)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}

/* Update parent process pipe file descriptors */
void	update_parent_pipes(int *prev_fd, int *pipefd, int is_last_cmd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (!is_last_cmd)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
}

/* Wait for all child processes and set exit code */
void	wait_for_pipe_children(pid_t *pids, int num_cmds, t_minishell *shell)
{
	int	i;
	int	status;
	int	last_status;

	status = 0;
	last_status = 0;
	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (i == num_cmds - 1)
			last_status = status;
		i++;
	}
	free(pids);
	if (WIFEXITED(last_status))
		shell->exit_code = WEXITSTATUS(last_status);
	else
		shell->exit_code = 1;
}
