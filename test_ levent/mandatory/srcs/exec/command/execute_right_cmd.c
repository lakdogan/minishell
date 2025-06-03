#include "../../../includes/core/minishell.h"

void	prepare_pipe_ends(t_minishell *shell, int *pipefd)
{
	safe_close(shell, pipefd[1], "pipe close write end");
	safe_dup2(shell, pipefd[0], STDIN_FILENO, "pipe right command stdin");
	safe_close(shell, pipefd[0], "pipe close read end after dup");
}

void	setup_in_and_output_redirections(t_minishell *shell, t_exec *exec)
{
	setup_input_redirections(shell, exec);
	setup_output_redirections(shell, exec);
}

void	check_exec_has_valid_data(t_minishell *shell, t_exec *exec)
{
	if (!exec)
		exit(shell->exit_code);
}

void	setup_default_pipe_input(t_minishell *shell, int *pipefd)
{
	safe_close(shell, pipefd[1], "pipe close write end");
	safe_dup2(shell, pipefd[0], STDIN_FILENO, "pipe right command stdin");
	safe_close(shell, pipefd[0], "pipe close read end after dup");
}

void	check_if_infile_exists(t_minishell *shell, t_exec *exec, int *pipefd)
{
	if (exec->infiles)
		close_pipe(shell, pipefd);
	else
		setup_default_pipe_input(shell, pipefd);
}

void	execute_builtin_or_external_cmd(t_minishell *shell, t_exec *exec)
{
	if (is_builtin(exec->command))
		shell->exit_code = exec_builtin(exec, shell);
	else
		execute_command(exec, shell);
}

void	handle_nested_cmds_recursively(t_minishell *shell, t_command_tree *node,
		int *pipefd)
{
	t_command_tree	*nested_pipe;

	nested_pipe = node->right;
	prepare_pipe_ends(shell, pipefd);
	handle_nested_pipe(nested_pipe, shell);
	exit(shell->exit_code);
}

void	handle_standard_cmds(t_minishell *shell, t_command_tree *node,
		int *pipefd)
{
	t_exec	*exec;

	exec = node->right->data;
	check_exec_has_valid_data(shell, exec);
	setup_in_and_output_redirections(shell, exec);
	check_if_infile_exists(shell, exec, pipefd);
	execute_builtin_or_external_cmd(shell, exec);
}

void	execute_right_subtree(t_minishell *shell, t_command_tree *node,
		int *pipefd)
{
	setup_default_pipe_input(shell, pipefd);
	execute_tree(node->right, shell);
}

static void	execute_right_by_type(t_minishell *shell, t_command_tree *node,
		int *pipefd)
{
	if (node->right->type == N_PIPE)
		handle_nested_cmds_recursively(shell, node, pipefd);
	else if (node->right->type == N_EXEC)
		handle_standard_cmds(shell, node, pipefd);
	else
		execute_right_subtree(shell, node, pipefd);
}

void	execute_right_cmd(t_command_tree *node, t_minishell *shell, int *pipefd)
{
	setup_child_signals();
	execute_right_by_type(shell, node, pipefd);
	exit(shell->exit_code);
}
