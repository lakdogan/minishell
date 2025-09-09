/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_with_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:20:41 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 00:22:31 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

// Executes a command using the given path.
static void	update_expanded_argv_and_flags(t_exec *exec, char **expanded_argv,
		t_minishell *minishell)
{
	int		new_argc;
	bool	*new_no_expand;
	bool	*new_quoted;
	int		i;

	new_argc = 0;
	while (expanded_argv[new_argc])
		new_argc++;
	new_no_expand = gc_malloc(minishell->gc[GC_MAIN], sizeof(bool) * (new_argc
				+ 1));
	new_quoted = gc_malloc(minishell->gc[GC_MAIN], sizeof(bool) * (new_argc
				+ 1));
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

static void	handle_expansion_and_flags(t_exec *exec, t_minishell *minishell)
{
	char	**expanded_argv;

	expanded_argv = expand_wildcards(exec->argv, exec->no_expand_flags,
			minishell);
	if (expanded_argv)
		update_expanded_argv_and_flags(exec, expanded_argv, minishell);
}

void	execute_with_path(char *path, t_exec *exec, t_minishell *minishell)
{
	t_error_context	err_ctx;

	handle_expansion_and_flags(exec, minishell);
	check_if_directory(path, minishell);
	check_path_accessibility(path, minishell);
	execve(path, exec->argv, minishell->envp_arr);
	err_ctx.shell = minishell;
	err_ctx.prefix = "minishell: ";
	err_ctx.subject = exec->command;
	err_ctx.message = ": execve error";
	err_ctx.exit_code = 127;
	exit_with_error(err_ctx);
}
