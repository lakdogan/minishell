/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:36:00 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:57:33 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Creates a pipe and handles errors.
int	create_pipe(t_minishell *shell, int *pipefd)
{
	t_error_context	err_ctx;

	if (pipe(pipefd) == SYSCALL_ERROR)
	{
		err_ctx.shell = shell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = "pipe";
		err_ctx.message = strerror(errno);
		err_ctx.exit_code = EXIT_FAILURE;
		exit_with_error(err_ctx);
		return (OPERATION_FAILURE);
	}
	return (OPERATION_SUCCESS);
}

// Closes both ends of a pipe safely.
void	close_pipe(t_minishell *shell, int *pipefd)
{
	safe_close(shell, pipefd[PIPE_READ_END], "close pipe read end");
	safe_close(shell, pipefd[PIPE_WRITE_END], "close pipe write end");
}
