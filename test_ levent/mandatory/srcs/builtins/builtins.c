/**
 * @file builtins.c
 * @brief Implementation of shell built-in commands
 *
 * This file contains the implementation of all built-in commands for the
 * minishell project, including echo, cd, pwd, export, unset, env, and exit.
 * Built-in commands are executed directly within the shell process rather
 * than spawning a new process.
 */

#include "../../includes/core/minishell.h"

/**
 * @brief Executes the appropriate built-in command
 *
 * This function determines which built-in command to execute based on the
 * command name and routes the execution to the appropriate handler function.
 * Supported built-ins include: echo, cd, pwd, export, unset, env, and exit.
 *
 * @param cmd The command structure containing the command name and arguments
 * @param minishell Pointer to the shell state containing environment variables
 * @return int 0 if the command executed successfully, non-zero otherwise
 */
int	exec_builtin(t_exec *cmd, t_minishell *minishell)
{
	if (ft_strncmp(cmd->command, "echo", SIZE_MAX) == 0)
		return (ft_echo(cmd->argv));
	else if (ft_strncmp(cmd->command, "cd", SIZE_MAX) == 0)
		return (ft_cd(cmd->argv, minishell));
	else if (ft_strncmp(cmd->command, "pwd", SIZE_MAX) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd->command, "export", SIZE_MAX) == 0)
		return (ft_export(cmd->argv, minishell));
	else if (ft_strncmp(cmd->command, "unset", SIZE_MAX) == 0)
		return (ft_unset(cmd->argv, minishell));
	else if (ft_strncmp(cmd->command, "env", SIZE_MAX) == 0)
		return (ft_env(minishell));
	else if (ft_strncmp(cmd->command, "exit", SIZE_MAX) == 0)
		return (ft_exit(cmd->argv, minishell));
	return (1);
}
