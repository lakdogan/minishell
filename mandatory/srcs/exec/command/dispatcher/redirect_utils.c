/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:39:38 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 19:44:52 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Setup heredoc redirection and return backup fd */
int	setup_heredoc_redirection(t_minishell *shell, t_exec *exec)
{
	int	stdin_backup;

	stdin_backup = INVALID_FD;
	if (exec && exec->heredoc_fd > 0 && exec->heredoc_fd != INVALID_FD)
	{
		stdin_backup = redirect_stdin_with_backup(shell, exec->heredoc_fd,
				"heredoc stdin redirection");
		safe_close(shell, exec->heredoc_fd, "close heredoc fd after backup");
	}
	return (stdin_backup);
}

/* Restore stdin from backup fd */
void	restore_stdin(t_minishell *shell, int stdin_backup)
{
	if (stdin_backup != INVALID_FD)
	{
		safe_dup2(shell, stdin_backup, STDIN_FILENO, "stdin restoration");
		close(stdin_backup);
	}
}
