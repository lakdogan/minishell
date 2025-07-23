/**
 * @file fd_operations.c
 * @brief File descriptor operation utilities
 *
 * This file contains utility functions for safely
 * managing file descriptors,
 * including duplication, closing, error checking,
 * and standard I/O redirection
 * with proper backup and restoration capabilities.
 *
 */

#include "../../../includes/core/minishell.h"

/**
 * @brief Safely duplicates a file descriptor
 *
 * Performs a dup2 operation and terminates the program with an error
 * message if the operation fails. This ensures robust error handling
 * for file descriptor duplication throughout the shell.
 *
 * @param oldfd Original file descriptor to duplicate
 * @param newfd Target file descriptor that will become a copy of oldfd
 * @param error_msg Message to display if the duplication fails
 */
void	safe_dup2(t_minishell *shell, int oldfd, int newfd,
		const char *error_msg)
{
	if (dup2(oldfd, newfd) == INVALID_FD)
		exit_with_error(shell, error_msg, strerror(errno), EXIT_FAILURE);
}

/**
 * @brief Safely closes a file descriptor
 *
 * Closes the specified file descriptor only if it's valid, and terminates
 * the program with an error message if the close operation fails.
 *
 * @param fd File descriptor to close
 * @param error_msg Message to display if the close operation fails
 */
void	safe_close(t_minishell *shell, int fd, const char *error_msg)
{
	if (fd != INVALID_FD)
	{
		if (close(fd) == CLOSE_ERROR)
			exit_with_error(shell, error_msg, strerror(errno), EXIT_FAILURE);
	}
}

/**
 * @brief Checks and handles file descriptor errors
 *
 * Verifies if a file descriptor is valid and terminates the program
 * with an appropriate error message if the descriptor is invalid.
 *
 * @param fd File descriptor to check
 * @param filename Name of the file associated with
 * the descriptor (for error messages)
 */
void	check_fd_error(t_minishell *shell, int fd, const char *filename)
{
	if (fd == INVALID_FD)
		exit_with_error(shell, "Error opening", filename, EXIT_FAILURE);
}

/**
 * @brief Redirects standard input and creates a backup
 *
 * Duplicates the current standard input as a backup, then redirects standard
 * input to come from the specified file descriptor.
 *
 * @param new_fd File descriptor to use as the new standard input
 * @param error_msg Message to display if the redirection fails
 * @return int Backup file descriptor for the original standard input,
 *             or INVALID_FD if no redirection was performed
 */
int	redirect_stdin_with_backup(t_minishell *shell, int new_fd,
		const char *error_msg)
{
	int	stdin_backup;

	stdin_backup = INVALID_FD;
	if (new_fd != INVALID_FD)
	{
		stdin_backup = dup(STDIN_FILENO);
		safe_dup2(shell, new_fd, STDIN_FILENO, error_msg);
	}
	return (stdin_backup);
}

/**
 * @brief Restores standard input and output from backups
 *
 * Restores the standard input and output file descriptors to their original
 * state using the provided backup descriptors, then closes the backups.
 *
 * @param stdin_backup Backup file descriptor for standard input
 * @param stdout_backup Backup file descriptor for standard output
 */
void	restore_std_fds(t_minishell *shell, int stdin_backup, int stdout_backup)
{
	safe_dup2(shell, stdin_backup, STDIN_FILENO, "restore stdin");
	safe_dup2(shell, stdout_backup, STDOUT_FILENO, "restore stdout");
	safe_close(shell, stdin_backup, "close stdin backup");
	safe_close(shell, stdout_backup, "close stdout backup");
}
