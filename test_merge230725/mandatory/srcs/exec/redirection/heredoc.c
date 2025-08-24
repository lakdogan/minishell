#include "../../../includes/core/minishell.h"

void	prepare_heredocs(t_minishell *shell, t_command_tree *node)
{
	t_exec	*exec;

	if (!node)
		return ;
	if (node->type == NODE_CMD)  // Replaced N_EXEC with NODE_CMD
	{
		exec = node->data;
		if (exec)
			save_heredoc_contents(shell, exec);
	}
	if (node->type == NODE_PIPE)  // Replaced N_PIPE with NODE_PIPE
	{
		if (node->left)
			prepare_heredocs(shell, node->left);
		if (node->right)
			prepare_heredocs(shell, node->right);
	}
}

void	write_heredoc_to_fd(t_minishell *shell, t_exec *exec, char *content)
{
    int		pipefd[PIPE_FD_COUNT];
    ssize_t	write_ret;

    if (!content)
        return ;
    if (!create_pipe(shell, pipefd))
        exit_with_error(shell, "minishell: ", "heredoc", strerror(errno), EXIT_FAILURE);
    write_ret = write(pipefd[PIPE_WRITE_END], content, ft_strlen(content));
    if (write_ret == WRITE_ERROR)
        exit_with_error(shell, "minishell: ", "heredoc", "Failed to write to pipe", EXIT_FAILURE);
    close(pipefd[PIPE_WRITE_END]);
    exec->heredoc_fd = pipefd[PIPE_READ_END];
}
