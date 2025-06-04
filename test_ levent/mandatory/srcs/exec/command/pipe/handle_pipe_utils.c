#include "../../../../includes/core/minishell.h"

void	close_file_descriptors(t_minishell *shell, t_pipe_state *state)
{
	close_pipe(shell, state->pipefd);
	close(state->stdout_backup);
}

void	wait_for_children(t_minishell *shell, pid_t left_pid, pid_t right_pid)
{
	wait_for_process(left_pid, shell);
	wait_for_process(right_pid, shell);
}

void	restore_stdout(t_pipe_state *state)
{
	if (state->stdout_backup != -1)
	{
		dup2(state->stdout_backup, STDOUT_FILENO);
		close(state->stdout_backup);
	}
}

void	cleanup_pipe(t_minishell *shell, t_pipe_state *state)
{
	close_pipe(shell, state->pipefd);
	wait_for_children(shell, state->left_pid, state->right_pid);
	restore_stdout(state);
}
