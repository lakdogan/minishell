#include "../../../includes/core/minishell.h"

int	write_heredoc_line(int fd, char *line)
{
	if (write(fd, line, ft_strlen(line)) == WRITE_ERROR)
		return (OPERATION_FAILURE);
	if (write(fd, "\n", 1) == WRITE_ERROR)
		return (OPERATION_FAILURE);
	return (OPERATION_SUCCESS);
}

void	prepare_heredocs(t_minishell *shell, t_command_tree *node)
{
	t_exec	*exec;

	if (!node)
		return ;
	if (node->type == N_EXEC)
	{
		exec = node->data;
		if (exec)
			save_heredoc_contents(shell, exec);
	}
	if (node->type == N_PIPE)
	{
		if (node->left)
			prepare_heredocs(shell, node->left);
		if (node->right)
			prepare_heredocs(shell, node->right);
	}
}

void	write_heredoc_to_fd(t_minishell *shell, t_exec *exec, char *content)
{
	int		pipefd[2];
	ssize_t	write_ret;

	if (!content)
		return ;
	if (!create_pipe(shell, pipefd))
		exit_with_error(shell, "heredoc", "Failed to create pipe", 1);
	write_ret = write(pipefd[PIPE_WRITE_END], content, ft_strlen(content));
	if (write_ret == -1)
		exit_with_error(shell, "heredoc", "Failed to write to pipe", 1);
	close(pipefd[PIPE_WRITE_END]);
	exec->heredoc_fd = pipefd[PIPE_READ_END];
}
