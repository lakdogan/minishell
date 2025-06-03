

/**
 * @file command_dispatcher_bonus.c
 * @brief Handles the execution flow of shell commands
 *
 * This file contains functions for dispatching commands to either
 * built-in handlers or external processes. It manages process creation,
 * I/O redirection setup, and signal handling during command execution.
 */

#include "../../../includes_bonus/core_bonus/minishell_bonus.h"

/**
 * @brief Prepares the environment for command execution
 *
 * Sets up signal handlers for the child process and configures
 * both input and output redirections for the command execution.
 *
 * @param exec Pointer to the command execution structure
 */
static void	prepare_command_execution(t_exec *exec)
{
	setup_child_signals();
	setup_input_redirections(exec);
	setup_output_redirections(exec);
}

/**
 * @brief Handles execution of external (non-builtin) commands
 *
 * Creates a child process to execute the external command.
 * The parent process waits for the child to complete and
 * captures its exit status.
 *
 * @param exec Pointer to the command execution structure
 * @param minishell Pointer to the shell state structure
 */
void	handle_external(t_exec *exec, t_minishell *minishell)
{
	pid_t	pid;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		prepare_command_execution(exec);
		execute_command(exec, minishell);
	}
	else if (pid == FORK_ERROR)
	{
		perror("fork");
		minishell->exit_code = EXIT_FAILURE;
	}
	else
		wait_for_process(pid, minishell);
}

/**
 * @brief Sets up heredoc input redirection
 *
 * Redirects the standard input to read from the heredoc file descriptor.
 * Creates a backup of the original stdin for later restoration.
 *
 * @param exec Pointer to the command execution structure
 * @return int File descriptor of the backed up stdin, or INVALID_FD on error
 */
static int	setup_heredoc_redirection(t_exec *exec)
{
	int	stdin_backup;

	stdin_backup = redirect_stdin_with_backup(exec->heredoc_fd,
			"heredoc stdin redirection");
	if (exec->heredoc_fd != INVALID_FD)
		safe_close(exec->heredoc_fd, "close heredoc fd after backup");
	return (stdin_backup);
}

/**
 * @brief Restores the standard input to its original state
 *
 * Uses the backed up stdin file descriptor to restore stdin after
 * command execution with redirected input has completed.
 *
 * @param stdin_backup File descriptor of the backed up stdin
 */
static void	restore_stdin(int stdin_backup)
{
	if (stdin_backup != INVALID_FD)
	{
		safe_dup2(stdin_backup, STDIN_FILENO, "stdin restoration");
		close(stdin_backup);
	}
}

/**
 * @brief Handles execution of built-in commands
 *
 * Sets up input redirection for heredocs, executes the built-in command,
 * and restores the standard input. Unlike external commands, built-ins
 * are executed in the current process.
 *
 * @param exec Pointer to the command execution structure
 * @param minishell Pointer to the shell state structure
 */
void	handle_builtin(t_exec *exec, t_minishell *minishell)
{
	int	stdin_backup;

	stdin_backup = setup_heredoc_redirection(exec);
	minishell->exit_code = exec_builtin(exec, minishell);
	restore_stdin(stdin_backup);
}
