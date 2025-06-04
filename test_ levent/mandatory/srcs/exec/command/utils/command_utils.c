/**
 * @file command_utils.c
 * @brief Utility functions for command execution
 *
 * This file contains helper functions for command execution in the shell,
 * including process creation, status processing, built-in command detection,
 * and process waiting functionality.
 */

#include "../../../../includes/core/minishell.h"

/**
 * @brief Creates a new child process using fork()
 *
 * Forks a new process and handles potential fork errors by exiting
 * with an appropriate error message. This function abstracts the
 * process creation mechanism.
 *
 * @return pid_t Process ID of the new child process, or parent's process ID
 */
pid_t	create_child_process(t_minishell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == FORK_ERROR)
		exit_with_error(shell, "fork", strerror(errno), EXIT_FAILURE);
	return (pid);
}

/**
 * @brief Processes the exit status of a child process
 *
 * Extracts information from the wait status and sets the shell's exit code
 * accordingly. Handles normal exits, signal terminations, and other cases.
 *
 * @param status The status returned by waitpid()
 * @param minishell Pointer to the shell state structure
 */
void	process_child_status(int status, t_minishell *shell)
{
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_code = SIGNAL_EXIT_BASE + WTERMSIG(status);
	else
		shell->exit_code = GENERAL_ERROR;
}

/**
 * @brief Checks if a command is a shell built-in
 *
 * Determines if the given command name matches any of the shell's
 * built-in commands (echo, cd, pwd, export, unset, env, exit).
 *
 * @param cmd The command name to check
 * @return int TRUE if the command is a built-in, FALSE otherwise
 */
int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (FALSE);
	return (ft_strncmp(cmd, "echo", SIZE_MAX) == STRINGS_EQUAL
		|| ft_strncmp(cmd, "cd", SIZE_MAX) == STRINGS_EQUAL || ft_strncmp(cmd,
			"pwd", SIZE_MAX) == STRINGS_EQUAL || ft_strncmp(cmd, "export",
			SIZE_MAX) == STRINGS_EQUAL || ft_strncmp(cmd, "unset",
			SIZE_MAX) == STRINGS_EQUAL || ft_strncmp(cmd, "env",
			SIZE_MAX) == STRINGS_EQUAL || ft_strncmp(cmd, "exit",
			SIZE_MAX) == STRINGS_EQUAL);
}

/**
 * @brief Waits for a specific child process to complete
 *
 * Blocks until the specified process terminates, then processes
 * its exit status to set the shell's exit code.
 *
 * @param pid Process ID of the child process to wait for
 * @param minishell Pointer to the shell state structure
 */
void	wait_for_process(pid_t pid, t_minishell *shell)
{
	int	status;

	waitpid(pid, &status, WAIT_BLOCK);
	process_child_status(status, shell);
}

void	execute_builtin_or_external_cmd(t_minishell *shell, t_exec *exec)
{
	if (is_builtin(exec->command))
		shell->exit_code = exec_builtin(exec, shell);
	else
		execute_command(exec, shell);
}
