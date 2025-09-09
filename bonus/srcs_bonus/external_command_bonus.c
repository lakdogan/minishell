/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:51:11 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 00:18:14 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

static void	update_expanded_argv_and_flags(t_exec *exec, char **expanded_argv,
		t_minishell *shell)
{
	int		new_argc;
	bool	*new_no_expand;
	bool	*new_quoted;
	int		i;

	new_argc = 0;
	while (expanded_argv[new_argc])
		new_argc++;
	new_no_expand = gc_malloc(shell->gc[GC_MAIN], sizeof(bool) * (new_argc
				+ 1));
	new_quoted = gc_malloc(shell->gc[GC_MAIN], sizeof(bool) * (new_argc + 1));
	i = 0;
	while (i < new_argc)
	{
		new_no_expand[i] = false;
		new_quoted[i] = false;
		i++;
	}
	new_no_expand[new_argc] = false;
	new_quoted[new_argc] = false;
	exec->argv = expanded_argv;
	exec->no_expand_flags = new_no_expand;
	exec->was_quoted_flags = new_quoted;
}

static void	child_external_process(t_exec *exec, t_minishell *shell)
{
	char	**expanded_argv;

	expanded_argv = expand_wildcards(exec->argv, exec->no_expand_flags, shell);
	if (expanded_argv)
		update_expanded_argv_and_flags(exec, expanded_argv, shell);
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
