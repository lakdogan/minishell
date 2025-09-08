/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:39:05 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 20:24:39 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Execute external command in forked process */
void	handle_external(t_exec *exec, t_minishell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		setup_child_signals();
		prepare_command_execution(shell, exec);
		if (exec->redirection_failed)
			exit(shell->exit_code);
		execute_command(exec, shell);
	}
	else if (pid == FORK_ERROR)
	{
		perror("fork");
		shell->exit_code = EXIT_FAILURE;
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		wait_for_process(pid, shell);
		setup_parent_signals();
	}
}
