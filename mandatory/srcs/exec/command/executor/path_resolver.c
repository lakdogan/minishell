/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:57:06 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 22:05:23 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Tries to execute a command from the local directory.
static void	try_local_execution(t_exec *exec, t_minishell *minishell)
{
	char	local_path[PATH_MAX];

	getcwd(local_path, sizeof(local_path));
	ft_strlcat(local_path, "/", sizeof(local_path));
	ft_strlcat(local_path, exec->command, sizeof(local_path));
	if (access(local_path, F_OK) == 0)
	{
		if (access(local_path, X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(exec->command, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			exit(126);
		}
		execve(local_path, exec->argv, minishell->envp_arr);
		exit(126);
	}
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(exec->command, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	exit(127);
}

// Checks if the resolved path is a directory.
static void	check_if_directory(t_minishell *minishell, t_exec *exec,
		char *abs_path)
{
	struct stat		path_stat;
	t_error_context	err_ctx;

	if (stat(abs_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		err_ctx.shell = minishell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = exec->command;
		err_ctx.message = ": is a directory";
		err_ctx.exit_code = 126;
		exit_with_error(err_ctx);
	}
}

// Executes the command with the provided absolute path.
static void	execute_command_pr(t_minishell *minishell, t_exec *exec,
		char *abs_path)
{
	t_error_context	err_ctx;

	execve(abs_path, exec->argv, minishell->envp_arr);
	err_ctx.shell = minishell;
	err_ctx.prefix = "minishell: ";
	err_ctx.subject = exec->command;
	err_ctx.message = ": execve error";
	err_ctx.exit_code = 127;
	exit_with_error(err_ctx);
}

// Executes the command with a resolved path.
static void	execute_with_resolved_path(t_minishell *minishell, t_exec *exec)
{
	char			*abs_path;
	t_error_context	err_ctx;

	abs_path = get_absolute_path(minishell, exec->command, minishell->envp_arr);
	if (!abs_path)
	{
		err_ctx.shell = minishell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = exec->command;
		err_ctx.message = ": command not found";
		err_ctx.exit_code = 127;
		exit_with_error(err_ctx);
	}
	check_if_directory(minishell, exec, abs_path);
	execute_command_pr(minishell, exec, abs_path);
}

// Executes a command, using PATH lookup or local execution.
void	execute_with_path_lookup(t_exec *exec, t_minishell *minishell)
{
	char	*path_env;

	path_env = find_env_value(minishell, "PATH");
	if (!path_env || !*path_env)
	{
		try_local_execution(exec, minishell);
		return ;
	}
	execute_with_resolved_path(minishell, exec);
}
