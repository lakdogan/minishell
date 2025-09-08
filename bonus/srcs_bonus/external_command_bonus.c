/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:51:11 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:30:22 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

// Handles the child process for external commands.
static void	child_external_process(t_exec *exec, t_minishell *shell)
{
	char	**expanded_argv;

	expanded_argv = expand_wildcards(exec->argv, exec->no_expand_flags);
	if (expanded_argv)
		exec->argv = expanded_argv;
	setup_child_signals();
	prepare_command_execution(shell, exec);
	if (exec->redirection_failed)
		exit(shell->exit_code);
	execute_command(exec, shell);
}

// Handles the parent process for external commands.
static void	parent_external_process(pid_t pid, t_minishell *shell)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	wait_for_process(pid, shell);
	setup_parent_signals();
}

// Handles fork errors for external commands.
static void	handle_fork_error(t_minishell *shell)
{
	perror("fork");
	shell->exit_code = EXIT_FAILURE;
}

// Handles execution of external commands.
void	handle_external(t_exec *exec, t_minishell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == CHILD_PROCESS)
		child_external_process(exec, shell);
	else if (pid == FORK_ERROR)
		handle_fork_error(shell);
	else
		parent_external_process(pid, shell);
}
