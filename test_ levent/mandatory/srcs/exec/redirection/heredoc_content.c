/**
 * @file heredoc_content.c
 * @brief Manages the collection and storage of heredoc content
 *
 * This file contains functions for handling heredoc
 * operations in the shell,
 * including collecting input from the user, storing it
 * temporarily, and making
 * it available to commands via file descriptors.
 * Heredocs allow multi-line input
 * to be passed to commands as if they were read from a file.
 */

#include "../../../includes/core/minishell.h"

/**
 * @brief Saves heredoc contents to a temporary file descriptor
 *
 * Identifies the last heredoc in the input chain and collects its content.
 * The content is then written to a file descriptor that can be used for
 * input redirection during command execution.
 *
 * @param exec Pointer to the command execution structure
 */
void	save_heredoc_contents(t_minishell *shell, t_exec *exec)
{
	char		*content;
	t_infile	*last_heredoc;
	t_infile	*temp;

	last_heredoc = NULL;
	if (exec->heredoc_prepared)
		return ;
	if (!exec || exec->heredoc_fd != INVALID_FD)
		return ;
	temp = exec->infiles;
	while (temp)
	{
		if (temp->type == INF_HEREDOC)
			last_heredoc = temp;
		temp = temp->next;
	}
	if (last_heredoc)
	{
		content = collect_heredoc_content(shell, exec, last_heredoc);
		if (!content)
			content = gc_strdup(shell->gc[GC_MAIN], "");
		write_heredoc_to_fd(shell, exec, content);
	}
	exec->heredoc_prepared = true;
}

/**
 * @brief Displays a prompt for heredoc input
 *
 * Prints a user-friendly prompt to stderr indicating that heredoc input
 * is expected, showing both the command that will receive the input and
 * the delimiter string that will terminate input collection.
 *
 * @param command Name of the command that will receive the heredoc input
 * @param delimiter String that will terminate heredoc input collection
 */
static void	print_heredoc_prompt(char *command, char *delimiter)
{
	ft_putstr_fd("heredoc for command '", STDERR_FILENO);
	if (command)
		ft_putstr_fd(command, STDERR_FILENO);
	else
		ft_putstr_fd("unknown", STDERR_FILENO);
	ft_putstr_fd("' with delimiter '", STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

/**
 * @brief Collects heredoc content from user input
 *
 * Creates a subprocess to read user input until the delimiter is encountered,
 * then returns all collected input as a dynamically allocated string.
 * Displays appropriate prompts and handles process communication.
 *
 * @param exec Pointer to the command execution structure
 * @param infile Pointer to the heredoc input file structure
 * @return char* Dynamically allocated string containing the heredoc content,
 *               or NULL if collection failed (caller must free if non-NULL)
 */
char	*collect_heredoc_content(t_minishell *shell, t_exec *exec, t_infile *infile)
{
	int		pipe_fd;
	char	*content;

	print_heredoc_prompt(exec->command, infile->delimeter);
	pipe_fd = process_heredoc(shell, infile->delimeter);
	if (pipe_fd < 0)
		return (NULL);
	content = read_from_pipe(shell, pipe_fd);
	safe_close(shell, pipe_fd, "heredoc pipe read end");
	return (content);
}
