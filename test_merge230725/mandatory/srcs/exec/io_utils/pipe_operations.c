#include "../../../includes/core/minishell.h"

/**
 * @brief Creates a new pipe with error handling
 *
 * Creates a new pipe and handles potential pipe
 * creation errors by printing
 * an error message and terminating the program.
 * Uses the pipe() system call
 * to create a unidirectional communication channel.
 *
 * @param pipefd Array to store the two pipe
 * file descriptors (read end and write end)
 * @return int OPERATION_SUCCESS if pipe creation
 * succeeds, never returns on failure
 */
int	create_pipe(t_minishell *shell, int *pipefd)
{
    if (pipe(pipefd) == SYSCALL_ERROR)
    {
        exit_with_error(shell, "minishell: ", "pipe", strerror(errno), EXIT_FAILURE);
        return (OPERATION_FAILURE);
    }
    return (OPERATION_SUCCESS);
}

/**
 * @brief Safely closes both ends of a pipe
 *
 * Closes both the read and write ends of a pipe, handling any errors that
 * might occur during the close operation with appropriate error messages.
 *
 * @param pipefd Array containing the two pipe file descriptors to close
 */
void	close_pipe(t_minishell *shell, int *pipefd)
{
	safe_close(shell, pipefd[PIPE_READ_END], "close pipe read end");
	safe_close(shell, pipefd[PIPE_WRITE_END], "close pipe write end");
}
