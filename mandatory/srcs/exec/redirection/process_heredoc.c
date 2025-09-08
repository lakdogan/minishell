/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:51:04 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:37:18 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Checks if the given line matches the heredoc delimiter.
static bool	is_delimiter_match(const char *line, const char *delimiter)
{
	return (ft_strcmp(line, delimiter) == 0);
}

// Writes a single line to the heredoc pipe.
static int	write_heredoc_line(int fd, char *line)
{
	if (write(fd, line, ft_strlen(line)) == WRITE_ERROR)
		return (OPERATION_FAILURE);
	if (write(fd, "\n", 1) == WRITE_ERROR)
		return (OPERATION_FAILURE);
	return (OPERATION_SUCCESS);
}

// Processes and writes a line to the heredoc pipe, 
// expanding variables if needed.
static bool	process_and_write_line(int *fd, char *line, t_minishell *shell,
		bool quoted_delimiter)
{
	char	*processed_line;
	bool	result;

	if (quoted_delimiter)
		processed_line = line;
	else
		processed_line = expand_variables(line, shell);
	if (!processed_line)
		return (false);
	result = write_heredoc_line(fd[PIPE_WRITE_END], processed_line);
	return (result);
}

// Processes heredoc input and returns the read end of the pipe.
int	process_heredoc(t_minishell *shell, const char *delimiter,
		bool quoted_delimiter)
{
	int		fd[PIPE_FD_COUNT];
	char	*line;

	(void)quoted_delimiter;
	if (!create_pipe(shell, fd))
		return (INVALID_FD);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		gc_register(shell->gc[GC_PROC_HERE], line, ft_strlen(line)
			+ NULL_TERMINATOR_SIZE);
		if (is_delimiter_match(line, delimiter))
			break ;
		if (!process_and_write_line(fd, line, shell, quoted_delimiter))
		{
			close_pipe(shell, fd);
			return (INVALID_FD);
		}
	}
	gc_collect(shell->gc[GC_PROC_HERE]);
	safe_close(shell, fd[PIPE_WRITE_END], "heredoc pipe write end");
	return (fd[PIPE_READ_END]);
}
