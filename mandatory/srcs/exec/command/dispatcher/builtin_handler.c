/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:40:23 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 19:44:41 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Execute a builtin command with proper redirections */
void	handle_builtin(t_exec *exec, t_minishell *shell)
{
	int	stdin_backup;

	stdin_backup = setup_heredoc_redirection(shell, exec);
	shell->exit_code = exec_builtin(exec, shell);
	restore_stdin(shell, stdin_backup);
}
