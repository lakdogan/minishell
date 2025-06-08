#include "../../../includes/core/minishell.h"

int	should_skip_heredoc_processing(t_minishell *shell, t_exec *exec)
{
	return (!shell || !exec || exec->heredoc_prepared
		|| exec->heredoc_fd != INVALID_FD);
}

void	save_heredoc_contents(t_minishell *shell, t_exec *exec)
{
	char		*content;
	t_infile	*last_heredoc;
	t_infile	*temp;

	if (should_skip_heredoc_processing(shell, exec))
		return ;
	last_heredoc = NULL;
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

char	*collect_heredoc_content(t_minishell *shell, t_exec *exec,
		t_infile *infile)
{
	int		pipe_fd;
	char	*content;

	if (!shell || !exec || !infile || !infile->delimeter)
		return (NULL);
	print_heredoc_prompt(exec->command, infile->delimeter);
	pipe_fd = process_heredoc(shell, infile->delimeter,
			infile->quoted_delimiter);
	if (pipe_fd == INVALID_FD)
		return (NULL);
	content = read_from_pipe(shell, pipe_fd);
	safe_close(shell, pipe_fd, "heredoc pipe read end");
	return (content);
}
