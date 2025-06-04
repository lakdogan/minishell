#include "../../../../includes/core/minishell.h"

static bool	pipe_state_initialized(t_minishell *shell, t_pipe_state *state,
		t_command_tree *node)
{
	if (!node || !node->left || !node->right)
		return (false);
	state->stdout_backup = dup(STDOUT_FILENO);
	if (state->stdout_backup == -1)
	{
		perror("stdout backup");
		return (false);
	}
	if (!create_pipe(shell, state->pipefd))
	{
		close(state->stdout_backup);
		return (false);
	}
	return (true);
}

static bool	fork_succeed_pid_recived(t_minishell *shell, t_pipe_state *state)
{
	if (state->pipe_identity == IS_LEFT_PID)
	{
		state->left_pid = create_child_process(shell);
		if (state->left_pid == FORK_ERROR)
		{
			close_file_descriptors(shell, state);
			return (false);
		}
		return (true);
	}
	else if (state->pipe_identity == IS_RIGHT_PID)
	{
		state->right_pid = create_child_process(shell);
		if (state->right_pid == FORK_ERROR)
		{
			close_file_descriptors(shell, state);
			waitpid(state->left_pid, NULL, WAIT_BLOCK);
			return (false);
		}
		return (true);
	}
	else
		return (false);
}

static bool	child_recognized_and_cmd_executed(t_minishell *shell,
		t_pipe_state *state, t_command_tree *node)
{
	if (state->pipe_identity == IS_LEFT_PID)
	{
		if (state->left_pid == CHILD_PROCESS)
		{
			close(state->stdout_backup);
			execute_left_cmd(node, shell, state->pipefd);
		}
		return (true);
	}
	else if (state->pipe_identity == IS_RIGHT_PID)
	{
		if (state->right_pid == CHILD_PROCESS)
		{
			close(state->stdout_backup);
			execute_right_cmd(node, shell, state->pipefd);
		}
		return (true);
	}
	else
	{
		return (false);
	}
}

static bool	pipe_created_and_cmd_executed(t_minishell *shell,
		t_command_tree *node, t_pipe_state *state)
{
	if (!fork_succeed_pid_recived(shell, state))
		return (false);
	if (!child_recognized_and_cmd_executed(shell, state, node))
		return (false);
	return (true);
}

void	handle_pipe(t_minishell *shell, t_command_tree *node)
{
	t_pipe_state	state;

	if (!pipe_state_initialized(shell, &state, node))
		return ;
	state.pipe_identity = IS_LEFT_PID;
	if (!pipe_created_and_cmd_executed(shell, node, &state))
		return ;
	state.pipe_identity = IS_RIGHT_PID;
	if (!pipe_created_and_cmd_executed(shell, node, &state))
		return ;
	cleanup_pipe(shell, &state);
}
