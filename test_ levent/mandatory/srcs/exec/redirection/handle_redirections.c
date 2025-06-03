/**
 * @file handle_redirections.c
 * @brief Manages input and output redirections for shell commands
 *
 * This file contains functions to handle various types of I/O redirections
 * in the shell, including input files, heredocs, output files with truncation,
 * and output files with append mode. Each function properly manages file
 * descriptors and error handling for robust command execution.
 */

#include "../../../includes/core/minishell.h"

/**
 * @brief Handles input file redirection
 *
 * Opens the specified input file and redirects standard input to read from it.
 * Properly closes the file descriptor after setting up the redirection.
 *
 * @param in Pointer to the input file structure containing the file name
 * @return int EXIT_SUCCESS on successful redirection
 */
static int	handle_input_file(t_infile *in)
{
	int	fd;

	fd = open_infile(in->name);
	safe_dup2(fd, STDIN_FILENO, "input redirection");
	safe_close(fd, "close input file");
	return (EXIT_SUCCESS);
}

/**
 * @brief Handles heredoc input redirection
 *
 * Redirects standard input to read from a previously prepared heredoc file
 * descriptor if one is available. Properly closes the file descriptor after
 * setting up the redirection.
 *
 * @param heredoc_fd File descriptor for the heredoc content
 */
static void	handle_heredoc_input(int heredoc_fd)
{
	if (heredoc_fd != INVALID_FD)
	{
		safe_dup2(heredoc_fd, STDIN_FILENO, "heredoc redirection");
		safe_close(heredoc_fd, "close heredoc fd");
	}
}

/**
 * @brief Sets up all input redirections for a command
 *
 * Processes all input files in the command's input list, applying each
 * redirection in order. After processing all regular input files, applies
 * any heredoc redirection if present.
 *
 * @param exec Pointer to the command execution structure
 */
void	setup_input_redirections(t_exec *exec)
{
	t_infile	*in;

	in = exec->infiles;
	while (in)
	{
		if (in->type == INFILE)
			handle_input_file(in);
		in = in->next;
	}
	handle_heredoc_input(exec->heredoc_fd);
}

/**
 * @brief Opens an output file with the appropriate mode
 *
 * Opens the specified output file with either truncate or append mode
 * based on the redirection type. Creates the file if it doesn't exist.
 *
 * @param out Pointer to the output file structure
 * @return int File descriptor for the opened file
 */
static int	open_output_file(t_outfile *out)
{
	int	fd;

	fd = INVALID_FD;
	if (out->type == OUT_TRUNC)
		fd = open(out->name, O_WRONLY | O_CREAT | O_TRUNC,
				DEFAULT_FILE_PERMISSION);
	else if (out->type == OUT_APPEND)
		fd = open(out->name, O_WRONLY | O_CREAT | O_APPEND,
				DEFAULT_FILE_PERMISSION);
	check_fd_error(fd, out->name);
	return (fd);
}

/**
 * @brief Sets up all output redirections for a command
 *
 * Processes all output files in the command's output list, applying each
 * redirection in order. Each output file is opened with the appropriate mode
 * and standard output is redirected to it.
 *
 * @param exec Pointer to the command execution structure
 */
void	setup_output_redirections(t_exec *exec)
{
	t_outfile	*out;
	int			fd;

	out = exec->outfiles;
	while (out)
	{
		fd = open_output_file(out);
		safe_dup2(fd, STDOUT_FILENO, "output redirection");
		safe_close(fd, "close output file");
		out = out->next;
	}
}
