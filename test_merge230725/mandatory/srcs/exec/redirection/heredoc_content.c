#include "../../../includes/core/minishell.h"

static int	should_skip_heredoc_processing(t_minishell *shell, t_exec *exec)
{
	return (!shell || !exec || exec->heredoc_prepared);
}

void	save_heredoc_contents(t_minishell *shell, t_exec *exec)
{
	char		*content;
	t_infile	*current;

	if (should_skip_heredoc_processing(shell, exec))
		return ;
	current = exec->infiles;
	while (current)
	{
		if (current->type == INF_HEREDOC)
		{
			content = collect_heredoc_content(shell, exec, current);
			if (!content)
				content = gc_strdup(shell->gc[GC_MAIN], EMPTY_STRING);
			write_heredoc_to_fd(shell, exec, content);
			exec->heredoc_prepared = true;
			break ;
		}
		current = current->next;
	}
}

char	*collect_heredoc_content(t_minishell *shell, t_exec *exec,
		t_infile *infile)
{
	int		pipe_fd;
	char	*content;

	if (!shell || !exec || !infile || !infile->delimeter)
		return (NULL);
	pipe_fd = process_heredoc(shell, infile->delimeter,
			infile->quoted_delimiter);
	if (pipe_fd == INVALID_FD)
	{
		ft_putstr_fd("minishell: heredoc: pipe creation failed\n",
			STDERR_FILENO);
		return (NULL);
	}
	content = read_from_pipe(shell, pipe_fd);
	safe_close(shell, pipe_fd, "heredoc pipe read end");
	return (content);
}
