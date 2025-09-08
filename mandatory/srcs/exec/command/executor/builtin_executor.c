/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 20:04:48 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 20:06:41 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Setup redirections for builtin commands */
static int	setup_builtin_redirections(t_exec *exec, t_minishell *minishell,
		int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
	setup_input_redirections(minishell, exec, false);
	if (exec->redirection_failed)
	{
		dup2(*stdin_backup, STDIN_FILENO);
		dup2(*stdout_backup, STDOUT_FILENO);
		close(*stdin_backup);
		close(*stdout_backup);
		minishell->exit_code = 1;
		return (0);
	}
	setup_output_redirections(minishell, exec);
	return (1);
}

/* Restore file descriptors after builtin execution */
static void	restore_fds_after_builtin(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

/* Execute a builtin command with redirections */
int	execute_builtin_cmd(t_exec *exec, t_minishell *minishell)
{
	int	stdin_backup;
	int	stdout_backup;

	handle_var_expansion_exec(minishell, exec);
	if (!setup_builtin_redirections(exec, minishell, &stdin_backup,
			&stdout_backup))
		return (1);
	handle_builtin(exec, minishell);
	restore_fds_after_builtin(stdin_backup, stdout_backup);
	return (1);
}
