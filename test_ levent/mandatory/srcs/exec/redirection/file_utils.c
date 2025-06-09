#include "../../../includes/core/minishell.h"

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
int	open_infile(t_minishell *shell, char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	check_fd_error(shell, fd, filename);
	return (fd);
}
