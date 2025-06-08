#include "../../../includes/core/minishell.h"

static bool	is_delimiter_match(const char *line, const char *delimiter)
{
	return (ft_strcmp(line, delimiter) == STRINGS_EQUAL);
}

static char	*prepare_heredoc_line(t_minishell *shell, char *line,
		bool quoted_delimiter)
{
	char	*processed_line;

	processed_line = NULL;
	if (!line)
		return (NULL);
	if (!quoted_delimiter)
	{
		processed_line = expand_variables(line, shell);
		if (processed_line)
			return (processed_line);
	}
	return (line);
}

static bool	process_and_write_line(t_minishell *shell, int *fd, char *line,
		bool quoted_delimiter)
{
	char	*processed_line;
	bool	result;

	processed_line = prepare_heredoc_line(shell, line, quoted_delimiter);
	if (!processed_line)
		return (false);
	result = write_heredoc_line(fd[PIPE_WRITE_END], processed_line);
	return (result);
}

int	process_heredoc(t_minishell *shell, const char *delimiter,
		bool quoted_delimiter)
{
	int		fd[PIPE_FD_COUNT];
	char	*line;

	if (!create_pipe(shell, fd))
		return (INVALID_FD);
	while (INFINITE_LOOP)
	{
		line = readline("> ");
		if (!line)
			break ;
		gc_register(shell->gc[GC_PROC_HERE], line, ft_strlen(line)
			+ NULL_TERMINATOR_SIZE);
		if (is_delimiter_match(line, delimiter))
			break ;
		if (!process_and_write_line(shell, fd, line, quoted_delimiter))
		{
			close_pipe(shell, fd);
			return (INVALID_FD);
		}
	}
	gc_collect(shell->gc[GC_PROC_HERE]);
	safe_close(shell, fd[PIPE_WRITE_END], "heredoc pipe write end");
	return (fd[PIPE_READ_END]);
}
