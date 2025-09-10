/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 00:49:02 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 19:54:57 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Creates a pipe for subshell communication.
static int	create_subshell_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

// Forks a new subshell process.
static pid_t	fork_subshell(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		perror("fork");
	return (pid);
}

// Handles the parent process logic for the subshell.
static void	parent_subshell(pid_t pid, int pipe_fd[2], t_minishell *minishell)
{
	char	buffer[4096];
	ssize_t	bytes_read;
	int		status;

	close(pipe_fd[1]);
	bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		write(STDOUT_FILENO, buffer, bytes_read);
		bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
	}
	close(pipe_fd[0]);
	waitpid(pid, &status, 0);
	minishell->in_subshell = false;
	if (WIFEXITED(status))
		minishell->exit_code = WEXITSTATUS(status);
	else
		minishell->exit_code = 1;
}

// Executes a subshell node.
void	execute_subshell(t_command_tree *node, t_minishell *minishell)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (create_subshell_pipe(pipe_fd) == -1)
		return ;
	pid = fork_subshell();
	if (pid < 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return ;
	}
	if (pid == 0)
		child_subshell(node, minishell, pipe_fd);
	else
		parent_subshell(pid, pipe_fd, minishell);
}
