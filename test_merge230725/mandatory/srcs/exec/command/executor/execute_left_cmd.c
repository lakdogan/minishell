#include "../../../../includes/core/minishell.h"

void	handle_standard_left_cmds(t_minishell *shell, t_command_tree *node,
		int *pipefd)
{
	t_exec	*exec;

	exec = node->left->data;
	check_exec_has_valid_data(shell, exec);
	prepare_for_execute(shell, node, exec, pipefd);
	execute_builtin_or_external_cmd(shell, exec);
}

void	setup_default_pipe_output(t_minishell *shell, int *pipefd)
{
	safe_close(shell, pipefd[PIPE_READ_END], "pipe close read end");
	safe_dup2(shell, pipefd[PIPE_WRITE_END], STDOUT_FILENO,
		"pipe left command stdout");
	safe_close(shell, pipefd[PIPE_WRITE_END], "pipe close write end after dup");
}

void	execute_left_subtree(t_minishell *shell, t_command_tree *node,
		int *pipefd)
{
	if (!node || !node->left)
		return ;
	if (node->left->type == NODE_CMD)
		handle_standard_left_cmds(shell, node, pipefd);
	else if (node->left->type == NODE_PIPE)
	{
		setup_default_pipe_output(shell, pipefd);
		execute_tree(node->left, shell);
	}
	else
	{
		setup_default_pipe_output(shell, pipefd);
		execute_tree(node->left, shell);
	}
}

static void	execute_left_by_type(t_minishell *shell, t_command_tree *node,
		int *pipefd)
{
	if (node->left->type == NODE_CMD)
		handle_standard_left_cmds(shell, node, pipefd);
	else
		execute_left_subtree(shell, node, pipefd);
}

void	execute_left_cmd(t_command_tree *node, t_minishell *shell, int *pipefd)
{
	setup_child_signals();
	execute_left_by_type(shell, node, pipefd);
	exit(shell->exit_code);
}
