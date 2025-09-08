/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command_executor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:08:55 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:07:14 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Executes a command in a pipe context.
void	execute_command_from_pipe(t_exec *exec, t_minishell *shell)
{
	if ((!exec->argv || !exec->argv[0]) && (exec->infiles || exec->outfiles))
	{
		setup_input_redirections(shell, exec, true);
		setup_output_redirections(shell, exec);
		if (!exec->redirection_failed)
			exit(0);
		else
			exit(1);
	}
	setup_input_redirections(shell, exec, true);
	setup_output_redirections(shell, exec);
	if (exec->redirection_failed)
		exit(shell->exit_code);
	if (is_builtin(exec->command))
	{
		exec_builtin(exec, shell);
		exit(shell->exit_code);
	}
	execvp(exec->command, exec->argv);
	perror("execvp");
	exit(127);
}
