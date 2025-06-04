#include "../../../../includes/core/minishell.h"

static int	fork_left_and_execute(t_minishell *shell, t_command_tree *node,
		t_exec *exec, t_pipe_state *pipe_state)
{
	pipe_state->left_pid = create_child_process(shell);
	if (pipe_state->left_pid == FORK_ERROR)
		return (FORK_FAILED);
	else if (pipe_state->left_pid == CHILD_PROCESS)
	{
		exec = node->left->data;
		setup_nested_pipe_output(shell, pipe_state->pipefd);
		execute_nested_cmd(shell, exec);
		exit(shell->exit_code);
	}
	return (FORK_SUCCESS);
}

static int	fork_right_and_execute(t_minishell *shell, t_command_tree *node,
		t_exec *exec, t_pipe_state *pipe_state)
{
	pipe_state->right_pid = create_child_process(shell);
	if (pipe_state->right_pid == FORK_ERROR)
		return (FORK_FAILED);
	else if (pipe_state->right_pid == CHILD_PROCESS)
	{
		exec = node->right->data;
		setup_nested_pipe_input(shell, pipe_state->pipefd);
		execute_nested_cmds(shell, exec);
		exit(shell->exit_code);
	}
	return (FORK_SUCCESS);
}

void	handle_nested_pipe(t_command_tree *node, t_minishell *shell)
{
	t_exec			*exec;
	t_pipe_state	pipe_state;

	exec = NULL;
	if (!create_pipe(shell, pipe_state.pipefd))
		return ;
	if (!fork_left_and_execute(shell, node, exec, &pipe_state))
	{
		close_pipe(shell, pipe_state.pipefd);
		return ;
	}
	if (!fork_right_and_execute(shell, node, exec, &pipe_state))
	{
		close_pipe(shell, pipe_state.pipefd);
		waitpid(pipe_state.left_pid, NULL, WAIT_BLOCK);
		return ;
	}
	close_pipe(shell, pipe_state.pipefd);
	wait_for_children(shell, pipe_state.left_pid, pipe_state.right_pid);
}
