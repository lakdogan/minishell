/**
 * @file command_dispatcher.c
 * @brief Handles the execution flow of shell commands
 *
 * This file contains functions for dispatching commands to either
 * built-in handlers or external processes. It manages process creation,
 * I/O redirection setup, and signal handling during command execution.
 */

#include "../../../../includes/core/minishell.h"
int count_args(char **argv)
{
    int count = 0;
    if (!argv)
        return 0;
    
    while (argv[count])
        count++;
    
    return count;
}
void preprocess_command(t_exec *exec, t_minishell *shell)
{
    // Early detection of cd $PWD before any expansion happens
    if (exec && exec->command && exec->argv && 
        ft_strcmp(exec->command, "cd") == STRINGS_EQUAL &&
        exec->argv[COMMAND_ARGS_START])
    {
        // Check for exact match of "$PWD" (not expanded yet)
        if (ft_strcmp(exec->argv[COMMAND_ARGS_START], "$PWD") == STRINGS_EQUAL)
        {
            // Mark this command to skip normal expansion
            exec->no_expand_flags = gc_malloc(shell->gc[GC_COMMAND], 
                                            sizeof(int) * (count_args(exec->argv) + 1));
            if (exec->no_expand_flags)
                exec->no_expand_flags[COMMAND_ARGS_START] = 1;
        }
    }
}

/**
 * @brief Prepares the environment for command execution
 *
 * Sets up signal handlers for the child process and configures
 * both input and output redirections for the command execution.
 *
 * @param exec Pointer to the command execution structure
 */
void	prepare_command_execution(t_minishell *shell, t_exec *exec)
{
	preprocess_command(exec, shell);
	setup_child_signals();
	setup_input_redirections(shell, exec, true);
	setup_output_redirections(shell, exec);
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
void	handle_external(t_exec *exec, t_minishell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		prepare_command_execution(shell, exec);
		execute_command(exec, shell);
	}
	else if (pid == FORK_ERROR)
	{
		perror("fork");
		shell->exit_code = EXIT_FAILURE;
	}
	else
		wait_for_process(pid, shell);
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
static int	setup_heredoc_redirection(t_minishell *shell, t_exec *exec)
{
	int	stdin_backup;

	stdin_backup = INVALID_FD;

	if (exec && exec->heredoc_fd > 0 && exec->heredoc_fd != INVALID_FD)
	{
		stdin_backup = redirect_stdin_with_backup(shell, exec->heredoc_fd,
				"heredoc stdin redirection");
		safe_close(shell, exec->heredoc_fd, "close heredoc fd after backup");
	}
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
static void	restore_stdin(t_minishell *shell, int stdin_backup)
{
	if (stdin_backup != INVALID_FD)
	{
		safe_dup2(shell, stdin_backup, STDIN_FILENO, "stdin restoration");
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
void	handle_builtin(t_exec *exec, t_minishell *shell)
{
	int	stdin_backup;

	stdin_backup = setup_heredoc_redirection(shell, exec);
	shell->exit_code = exec_builtin(exec, shell);
	restore_stdin(shell, stdin_backup);
}
