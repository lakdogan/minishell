

/**
 * @file redirection_files_bonus.c
 * @brief File operations for handling shell redirections
 *
 * This file contains utility functions for opening input files and creating
 * temporary files needed for implementing I/O redirections in the shell.
 * It handles file descriptor error checking and
 * proper cleanup of temporary files.
 */

#include "../../../includes_bonus/core_bonus/minishell_bonus.h"

/**
 * @brief Opens a file for input redirection
 *
 * Opens the specified file in read-only mode and checks for potential
 * errors such as file not found or permission denied. If an error occurs,
 * appropriate error handling is performed.
 *
 * @param filename Name of the file to open
 * @return int File descriptor for the opened file
 * @note This function will terminate the program if the file cannot be opened
 */
int	open_infile(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	check_fd_error(fd, filename);
	return (fd);
}

/**
 * @brief Creates a temporary file for heredoc content
 *
 * Generates a unique temporary file in the system's temp directory
 * using a template name. The file is immediately unlinked after creation
 * so it will be automatically deleted when closed, but remains accessible
 * via the returned file descriptor.
 *
 * @return int File descriptor for the temporary file
 * @note This function will terminate the
 * program if temporary file creation fails
 */
int	create_temp_file(void)
{
	char	template[30];
	int		fd;

	ft_strlcpy(template, "/tmp/minishell-heredoc-XXXXXX", 30);
	fd = mkstemp(template);
	if (fd < 0)
		exit_with_error("heredoc", "Failed to create temp file", EXIT_FAILURE);
	unlink(template);
	return (fd);
}
