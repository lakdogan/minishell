/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command_executor_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:08:55 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 00:16:44 by lakdogan         ###   ########.fr       */
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
	char	**envp_arr;

	if (is_builtin(exec->command))
	{
		exec_builtin(exec, shell);
		exit(shell->exit_code);
	}
	envp_arr = list_to_envp_array(shell->gc[GC_MAIN], shell->envp);
	ft_execvp(exec->command, exec->argv, envp_arr);
	perror("execvp");
	exit(127);
}

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

void	execute_command_from_pipe(t_exec *exec, t_minishell *shell)
{
	char	**expanded_argv;

	expanded_argv = expand_wildcards(exec->argv, exec->no_expand_flags, shell);
	if (expanded_argv)
		update_expanded_argv_and_flags(exec, expanded_argv, shell);
	if ((!exec->argv || !exec->argv[0]) && (exec->infiles || exec->outfiles))
		handle_empty_command_with_redirections(exec, shell);
	handle_redirections_or_exit(exec, shell);
	exec_builtin_or_external(exec, shell);
}
