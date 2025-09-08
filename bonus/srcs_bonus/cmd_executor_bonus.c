/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_executor_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:56:26 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:28:05 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

/* Handle special dot command */
static void	handle_dot_command(t_exec *exec)
{
	if (!exec->argv[1])
	{
		ft_putstr_fd("minishell: .: filename argument required\n",
			STDERR_FILENO);
		ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO);
		exit(127);
	}
}

/* Handle special cases before main execution */
void	handle_special_cases(t_exec *exec, t_minishell *minishell)
{
	if (!exec->argv[0] || !*exec->argv[0])
		exit(0);
	if (exec->command && ft_strcmp(exec->command, ".") == 0)
		handle_dot_command(exec);
	if (exec->command && ft_strcmp(exec->command, "..") == 0)
	{
		ft_putstr_fd("minishell: ..: command not found\n", STDERR_FILENO);
		exit(127);
	}
	handle_var_expansion_exec(minishell, exec);
	if (!exec->command || !*exec->command
		|| ft_is_whitespace_str(exec->command))
		exit(127);
}

// Checks if the given path is a directory.
static void	check_if_directory(char *path, t_minishell *minishell)
{
	struct stat		path_stat;
	t_error_context	err_ctx;

	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		err_ctx.shell = minishell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = path;
		err_ctx.message = ": is a directory";
		err_ctx.exit_code = 126;
		exit_with_error(err_ctx);
	}
}

// Checks if the path is accessible and executable.
static void	check_path_accessibility(char *path, t_minishell *minishell)
{
	struct stat		path_stat;
	t_error_context	err_ctx;

	if (stat(path, &path_stat) != 0)
	{
		err_ctx.shell = minishell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = path;
		err_ctx.message = ": No such file or directory";
		err_ctx.exit_code = 127;
		exit_with_error(err_ctx);
	}
	if (access(path, X_OK) != 0)
	{
		err_ctx.shell = minishell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = path;
		err_ctx.message = ": Permission denied";
		err_ctx.exit_code = 126;
		exit_with_error(err_ctx);
	}
}

// Executes a command using the given path.
void	execute_with_path(char *path, t_exec *exec, t_minishell *minishell)
{
	t_error_context	err_ctx;
	char			**expanded_argv;

	expanded_argv = expand_wildcards(exec->argv, exec->no_expand_flags);
	if (expanded_argv)
		exec->argv = expanded_argv;
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
