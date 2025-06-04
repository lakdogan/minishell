/**
 * @file external_executor.c
 * @brief Implementation of external command execution functionality
 *
 * This file contains the implementation for executing external (non-builtin)
 * commands in the shell. It handles resolving command paths, executing
 * the command via execve(),
 *	and proper error handling for various failure cases.
 *
 *
 */

#include "../../../includes/core/minishell.h"

/**
 * @brief Executes an external command in a child process
 *
 * Resolves the absolute path of the command using
 * the PATH environment variable,
 * then attempts to execute it using execve(). Handles various error conditions
 * such as command not found and permission
 *	denied with appropriate error messages
 * and exit codes. This function never
 *  returns if successful as execve() replaces
 * the current process image.
 *
 * @param exec Pointer to the command execution
 * structure containing command and arguments
 * @param minishell Pointer to the shell state structure
 * containing environment variables
 * @note This function terminates the process on both success and failure
 */
void	execute_command(t_exec *exec, t_minishell *minishell)
{
	char	*abs_path;

	abs_path = get_absolute_path(minishell, exec->command, minishell->envp_arr);
	if (!abs_path)
		exit_with_error(minishell, "command not found: ", exec->command, CMD_NOT_FOUND);
	else if (errno == EACCES)
		exit_with_error(minishell, "permission denied: ", exec->command,
			PERMISSION_DENIED);
	execve(abs_path, exec->argv, minishell->envp_arr);
	exit_with_error(minishell, "execve", strerror(errno), CMD_NOT_FOUND);
}
