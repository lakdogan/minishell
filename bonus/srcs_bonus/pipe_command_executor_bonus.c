/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command_executor_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:08:55 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:32:02 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

// Handles empty command with only redirections.
static void	handle_empty_command_with_redirections(t_exec *exec,
		t_minishell *shell)
{
	setup_input_redirections(shell, exec, true);
	setup_output_redirections(shell, exec);
	if (!exec->redirection_failed)
		exit(0);
	else
		exit(1);
}

// Handles redirections or exits if failed.
static void	handle_redirections_or_exit(t_exec *exec, t_minishell *shell)
{
	setup_input_redirections(shell, exec, true);
	setup_output_redirections(shell, exec);
	if (exec->redirection_failed)
		exit(shell->exit_code);
}

// Executes a builtin or external command.
static void	exec_builtin_or_external(t_exec *exec, t_minishell *shell)
{
	if (is_builtin(exec->command))
	{
		exec_builtin(exec, shell);
		exit(shell->exit_code);
	}
	execvp(exec->command, exec->argv);
	perror("execvp");
	exit(127);
}

// Executes a command from a pipe.
void	execute_command_from_pipe(t_exec *exec, t_minishell *shell)
{
	char	**expanded_argv;

	expanded_argv = expand_wildcards(exec->argv, exec->no_expand_flags);
	if (expanded_argv)
		exec->argv = expanded_argv;
	if ((!exec->argv || !exec->argv[0]) && (exec->infiles || exec->outfiles))
		handle_empty_command_with_redirections(exec, shell);
	handle_redirections_or_exit(exec, shell);
	exec_builtin_or_external(exec, shell);
}
