/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:09:43 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 18:42:04 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

/* Handle child process execution */
static int	execute_child_process(char **argv, int cmd_index, char **env)
{
	if (execve(argv[cmd_index], &argv[cmd_index], env) == -1)
	{
		ft_putstr_fd("env: ", STDERR_FILENO);
		ft_putstr_fd(argv[cmd_index], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		exit(126);
	}
	return (BUILTIN_FAILURE);
}

/* Handle parent process waiting for command completion */
static int	wait_for_child(pid_t pid, t_env_context *ctx)
{
	int	status;
	int	exit_code;
	int	var_count;
	int	is_dynamic;

	var_count = 0;
	if (ctx->custom_env)
		while (ctx->custom_env[var_count])
			var_count++;
	is_dynamic = (ctx->custom_env != ctx->minishell->envp_arr);
	if (waitpid(pid, &status, 0) == -1)
	{
		ft_putstr_fd("minishell: env: waitpid error\n", STDERR_FILENO);
		return (BUILTIN_FAILURE);
	}
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else
		exit_code = BUILTIN_FAILURE;
	if (is_dynamic && ctx->custom_env)
		free_env_array(ctx->custom_env, var_count);
	return (exit_code);
}

/* Setup environment and execute command in child process */
static int	setup_and_execute(t_env_context *ctx)
{
	char	**env_to_use;

	if (ctx->custom_env)
		env_to_use = ctx->custom_env;
	else if (ctx->flags & ENV_FLAG_EMPTY)
		env_to_use = NULL;
	else
		env_to_use = ctx->argv;
	return (execute_child_process(ctx->argv, ctx->cmd_index, env_to_use));
}

/* Prepare environment context for command execution */
static t_env_context	prepare_env_context(char **argv, int cmd_index,
		char **custom_env, t_minishell *minishell)
{
	t_env_context	ctx;

	ctx.argv = argv;
	ctx.cmd_index = cmd_index;
	ctx.custom_env = custom_env;
	ctx.flags = minishell->env_flags;
	ctx.minishell = minishell;
	return (ctx);
}

/* Execute a command with a custom environment */
int	execute_env_command(char **argv, int cmd_index, char **custom_env,
		t_minishell *minishell)
{
	pid_t			pid;
	t_env_context	ctx;
	int				result;
	int				var_count;
	int				is_dynamic;

	var_count = 0;
	ctx = prepare_env_context(argv, cmd_index, custom_env, minishell);
	is_dynamic = (custom_env != minishell->envp_arr);
	if (custom_env)
		while (custom_env[var_count])
			var_count++;
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: env: fork error\n", STDERR_FILENO);
		if (is_dynamic && custom_env)
			free_env_array(custom_env, var_count);
		return (BUILTIN_FAILURE);
	}
	else if (pid == 0)
		result = setup_and_execute(&ctx);
	else
		result = wait_for_child(pid, &ctx);
	return (result);
}
