/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:50:31 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:54:32 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Prepares heredocs for all nodes in the command tree.
void	prepare_heredocs(t_minishell *shell, t_command_tree *node)
{
	t_exec	*exec;

	if (!node)
		return ;
	if (node->type == NODE_CMD)
	{
		exec = node->data;
		if (exec)
			save_heredoc_contents(shell, exec);
	}
	if (node->type == NODE_PIPE)
	{
		if (node->left)
			prepare_heredocs(shell, node->left);
		if (node->right)
			prepare_heredocs(shell, node->right);
	}
}

// Creates a pipe for heredoc usage, exits on failure.
static void	create_heredoc_pipe(t_minishell *shell, int pipefd[PIPE_FD_COUNT])
{
	t_error_context	err_ctx;

	if (!create_pipe(shell, pipefd))
	{
		err_ctx.shell = shell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = "heredoc";
		err_ctx.message = strerror(errno);
		err_ctx.exit_code = EXIT_FAILURE;
		exit_with_error(err_ctx);
	}
}

// Writes heredoc content to the pipe, exits on failure.
static void	write_to_heredoc_pipe(t_minishell *shell, int pipe_write_fd,
		char *content)
{
	ssize_t			write_ret;
	t_error_context	err_ctx;

	write_ret = write(pipe_write_fd, content, ft_strlen(content));
	if (write_ret == WRITE_ERROR)
	{
		err_ctx.shell = shell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = "heredoc";
		err_ctx.message = "Failed to write to pipe";
		err_ctx.exit_code = EXIT_FAILURE;
		exit_with_error(err_ctx);
	}
}

// Writes heredoc content to a file descriptor for exec.
void	write_heredoc_to_fd(t_minishell *shell, t_exec *exec, char *content)
{
	int	pipefd[PIPE_FD_COUNT];

	if (!content)
		return ;
	create_heredoc_pipe(shell, pipefd);
	write_to_heredoc_pipe(shell, pipefd[PIPE_WRITE_END], content);
	close(pipefd[PIPE_WRITE_END]);
	exec->heredoc_fd = pipefd[PIPE_READ_END];
}
