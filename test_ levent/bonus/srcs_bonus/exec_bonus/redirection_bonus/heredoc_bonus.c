

/**
 * @file heredoc_bonus.c
 * @brief Implementation of heredoc functionality for shell redirections
 *
 * This file contains functions for handling heredoc redirections in the shell.
 * It includes functionality for reading input until a delimiter is encountered,
 * writing the collected content to pipes, and preparing heredocs for command
 * execution throughout the command tree.
 */

#include "../../../includes_bonus/core_bonus/minishell_bonus.h"

/**
 * @brief Writes a line to the heredoc pipe with a trailing newline
 *
 * Writes the given line of input to the specified file descriptor,
 * followed by a newline character. Handles write errors appropriately.
 *
 * @param fd File descriptor to write to (typically the write end of a pipe)
 * @param line Line of text to write
 * @return int OPERATION_SUCCESS on successful write, OPERATION_FAILURE on error
 */
static int	write_heredoc_line(int fd, char *line)
{
	if (write(fd, line, ft_strlen(line)) == WRITE_ERROR)
		return (OPERATION_FAILURE);
	if (write(fd, "\n", 1) == WRITE_ERROR)
		return (OPERATION_FAILURE);
	return (OPERATION_SUCCESS);
}

/**
 * @brief Processes a heredoc by collecting lines until delimiter is encountered
 *
 * Creates a pipe and reads input line by line from the user until the specified
 * delimiter is encountered. Each line is written to the pipe. Returns the read
 * end of the pipe, which can be used to read the heredoc contents.
 *
 * @param delimiter String that signals the end of heredoc input
 * @return int File descriptor for reading heredoc contents,
 *	or INVALID_FD on error
 */
int	process_heredoc(const char *delimiter)
{
	int		fd[2];
	char	*line;

	if (!create_pipe(fd))
		return (INVALID_FD);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == STRINGS_EQUAL)
		{
			free(line);
			break ;
		}
		if (!write_heredoc_line(fd[PIPE_WRITE_END], line))
		{
			free(line);
			close_pipe(fd);
			return (INVALID_FD);
		}
		free(line);
	}
	safe_close(fd[PIPE_WRITE_END], "heredoc pipe write end");
	return (fd[PIPE_READ_END]);
}

/**
 * @brief Recursively prepares all heredocs in a command tree
 *
 * Traverses the command tree and prepares any heredocs found in the execution
 * nodes. For pipe nodes, processes both the left and right subtrees.
 *
 * @param node Pointer to the root node of the command tree (or subtree)
 */
void	prepare_heredocs(t_command_tree *node)
{
	t_exec	*exec;

	if (!node)
		return ;
	if (node->type == N_EXEC)
	{
		exec = node->data;
		if (exec)
			save_heredoc_contents(exec);
	}
	if (node->type == N_PIPE)
	{
		if (node->left)
			prepare_heredocs(node->left);
		if (node->right)
			prepare_heredocs(node->right);
	}
}

/**
 * @brief Writes heredoc content to a pipe and saves the read end
 *
 * Creates a pipe, writes the provided content to it, and stores the
 * read end in the exec structure. This allows commands to read from
 * the heredoc as if it were a file.
 *
 * @param exec Pointer to the command execution structure
 * @param content String containing the
 * heredoc content (will be freed by this function)
 * @note This function assumes ownership of the content string and frees it
 */
void	write_heredoc_to_fd(t_exec *exec, char *content)
{
	int		pipefd[2];
	ssize_t	write_ret;

	if (!content)
		return ;
	if (!create_pipe(pipefd))
		exit_with_error("heredoc", "Failed to create pipe", 1);
	write_ret = write(pipefd[PIPE_WRITE_END], content, ft_strlen(content));
	if (write_ret == -1)
		exit_with_error("heredoc", "Failed to write to pipe", 1);
	close(pipefd[PIPE_WRITE_END]);
	exec->heredoc_fd = pipefd[PIPE_READ_END];
	free(content);
}
