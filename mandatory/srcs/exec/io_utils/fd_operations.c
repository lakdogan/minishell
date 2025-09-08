/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_operations.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:35:47 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:58:41 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Safely duplicates a file descriptor.
void	safe_dup2(t_minishell *shell, int oldfd, int newfd,
		const char *error_msg)
{
	t_error_context	err_ctx;

	if (dup2(oldfd, newfd) == INVALID_FD)
	{
		err_ctx.shell = shell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = error_msg;
		err_ctx.message = strerror(errno);
		err_ctx.exit_code = EXIT_FAILURE;
		exit_with_error(err_ctx);
	}
}

// Safely closes a file descriptor.
void	safe_close(t_minishell *shell, int fd, const char *error_msg)
{
	t_error_context	err_ctx;

	if (fd != INVALID_FD)
	{
		if (close(fd) == CLOSE_ERROR)
		{
			err_ctx.shell = shell;
			err_ctx.prefix = "minishell: ";
			err_ctx.subject = error_msg;
			err_ctx.message = strerror(errno);
			err_ctx.exit_code = EXIT_FAILURE;
			exit_with_error(err_ctx);
		}
	}
}

// Checks and handles file descriptor errors.
void	check_fd_error(t_minishell *shell, int fd, const char *filename)
{
	t_error_context	err_ctx;

	if (fd == INVALID_FD)
	{
		err_ctx.shell = shell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = "Error opening";
		err_ctx.message = filename;
		err_ctx.exit_code = EXIT_FAILURE;
		exit_with_error(err_ctx);
	}
}

// Redirects stdin with backup.
int	redirect_stdin_with_backup(t_minishell *shell, int new_fd,
		const char *error_msg)
{
	int	stdin_backup;

	stdin_backup = INVALID_FD;
	if (new_fd != INVALID_FD)
	{
		stdin_backup = dup(STDIN_FILENO);
		safe_dup2(shell, new_fd, STDIN_FILENO, error_msg);
	}
	return (stdin_backup);
}

// Restores standard file descriptors from backups.
void	restore_std_fds(t_minishell *shell, int stdin_backup, int stdout_backup,
		int stderr_backup)
{
	safe_dup2(shell, stdin_backup, STDIN_FILENO, "restore stdin");
	safe_dup2(shell, stdout_backup, STDOUT_FILENO, "restore stdout");
	safe_dup2(shell, stderr_backup, STDERR_FILENO, "restore stderr");
	safe_close(shell, stdin_backup, "close stdin backup");
	safe_close(shell, stdout_backup, "close stdout backup");
	safe_close(shell, stderr_backup, "close stderr backup");
}
