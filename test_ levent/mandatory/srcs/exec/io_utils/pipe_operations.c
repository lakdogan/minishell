/**
 * @file pipe_operations.c
 * @brief Pipe operation utilities for shell command execution
 *
 * This file contains utility functions for
 * creating, managing, and reading from
 * pipes used in command execution. It provides
 * safe pipe creation and closure,
 * along with buffered reading functionality for
 * interprocess communication.
 */

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
int	create_pipe(int *pipefd)
{
	if (pipe(pipefd) == SYSCALL_ERROR)
	{
		exit_with_error("pipe", strerror(errno), EXIT_FAILURE);
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
void	close_pipe(int *pipefd)
{
	safe_close(pipefd[0], "close pipe read end");
	safe_close(pipefd[1], "close pipe write end");
}

/**
 * @brief Reads all available data from a pipe into a string
 *
 * Reads data from the specified pipe file descriptor in chunks until EOF
 * or an error is encountered. Dynamically allocates memory to store the
 * complete content read from the pipe.
 *
 * @param pipe_fd File descriptor to read from
 * (typically the read end of a pipe)
 * @return char* Dynamically allocated string containing
 * 				all data read from the pipe,
 *               or NULL if no data was read (caller must free)
 * @note Terminates the program with an
 * error message if a read error occurs
 */
char	*read_from_pipe(int pipe_fd)
{
	char	buffer[BUFFER_SIZE_TEE];
	char	*content;
	char	*new_content;
	ssize_t	bytes;

	content = NULL;
	bytes = read(pipe_fd, buffer, BUFFER_SIZE_TEE - 1);
	while (bytes > 0)
	{
		buffer[bytes] = NULL_TERMINATOR;
		new_content = ft_safe_strjoin(content, buffer);
		if (content)
			free(content);
		content = new_content;
		bytes = read(pipe_fd, buffer, BUFFER_SIZE_TEE - 1);
	}
	if (bytes < 0)
		exit_with_error("read_from_pipe", strerror(errno), EXIT_FAILURE);
	return (content);
}
