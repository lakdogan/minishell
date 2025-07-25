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
 * @return int STRINGS_EQUAL if the command executed successfully,
	non-zero otherwise
 */
int	exec_builtin(t_exec *cmd, t_minishell *minishell)
{
	if (!cmd || !cmd->command || !*cmd->command)
		return (BUILTIN_FAILURE);
	if (ft_strcmp(cmd->command, "echo") == STRINGS_EQUAL)
		return (ft_echo(cmd->argv));
	else if (ft_strcmp(cmd->command, "cd") == STRINGS_EQUAL)
		return (ft_cd(cmd->argv, minishell));
	else if (ft_strcmp(cmd->command, "pwd") == STRINGS_EQUAL)
		return (ft_pwd());
	else if (ft_strcmp(cmd->command, "export") == STRINGS_EQUAL)
		return (ft_export(cmd->argv, minishell));
	else if (ft_strcmp(cmd->command, "unset") == STRINGS_EQUAL)
		return (ft_unset(cmd->argv, minishell));
	else if (ft_strcmp(cmd->command, "env") == STRINGS_EQUAL)
		return (ft_env(minishell));
	else if (ft_strcmp(cmd->command, "exit") == STRINGS_EQUAL)
		return (ft_exit(cmd->argv, minishell));
	return (BUILTIN_NOT_FOUND);
}
