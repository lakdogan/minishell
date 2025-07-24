#include "../../../includes/core/minishell.h"

// static bool	is_delimiter_match(const char *line, const char *delimiter)
// {
// 	return (ft_strcmp(line, delimiter) == STRINGS_EQUAL);
// }

static int	write_heredoc_line(int fd, char *line)
{
	if (write(fd, line, ft_strlen(line)) == WRITE_ERROR)
		return (OPERATION_FAILURE);
	if (write(fd, "\n", 1) == WRITE_ERROR)
		return (OPERATION_FAILURE);
	return (OPERATION_SUCCESS);
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

// int	process_heredoc(t_minishell *shell, const char *delimiter,
// 		bool quoted_delimiter)
// {
// 	int		fd[PIPE_FD_COUNT];
// 	char	*line;

// 	if (!create_pipe(shell, fd))
// 		return (INVALID_FD);
	
// 	ft_putstr_fd("\nEnter heredoc content. Type '", STDERR_FILENO);
//     ft_putstr_fd((char *)delimiter, STDERR_FILENO);
//     ft_putstr_fd("' to end input:\n", STDERR_FILENO);
// 	while (INFINITE_LOOP)
// 	{
// 		write(STDOUT_FILENO, "heredoc> ", 9);
//         fflush(stdout);
// 		line = readline("");
// 		if (!line)
// 			break ;
// 		gc_register(shell->gc[GC_PROC_HERE], line, ft_strlen(line)
// 			+ NULL_TERMINATOR_SIZE);
// 		if (is_delimiter_match(line, delimiter))
// 			break ;
// 		if (!process_and_write_line(shell, fd, line, quoted_delimiter))
// 		{
// 			close_pipe(shell, fd);
// 			return (INVALID_FD);
// 		}
// 	}
// 	gc_collect(shell->gc[GC_PROC_HERE]);
// 	safe_close(shell, fd[PIPE_WRITE_END], "heredoc pipe write end");
// 	return (fd[PIPE_READ_END]);
// }

int process_heredoc(t_minishell *shell, const char *delimiter,
	bool quoted_delimiter)
{
int fd[PIPE_FD_COUNT];
char *line;
struct termios orig_termios, raw_termios;

// Terminal-Einstellungen sichern und für Heredoc anpassen
tcgetattr(STDIN_FILENO, &orig_termios);
raw_termios = orig_termios;
raw_termios.c_lflag &= ~(ECHOCTL); // Steuerzeichen nicht anzeigen
tcsetattr(STDIN_FILENO, TCSANOW, &raw_termios);

if (!create_pipe(shell, fd))
	return (INVALID_FD);


// Signal-Handler temporär deaktivieren
signal(SIGINT, SIG_IGN);

while (1)
{
	// Prompt direkt ausgeben (nicht über readline)
	write(STDERR_FILENO, "> ", 1);
	fflush(stderr);
	
	// readline mit leerem Prompt verwenden
	line = readline("");
	
	if (!line)
		break;
		
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		break;
	}
	
	if (!process_and_write_line(shell, fd, line, quoted_delimiter))
	{
		free(line);
		close_pipe(shell, fd);
		return (INVALID_FD);
	}
	
	free(line);
}

// Terminal-Einstellungen wiederherstellen
tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);

// Standard-Signalbehandlung wiederherstellen
setup_parent_signals();

safe_close(shell, fd[PIPE_WRITE_END], "heredoc pipe write end");
return (fd[PIPE_READ_END]);
}
