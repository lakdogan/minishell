#include "../../../../includes/core/minishell.h"

void	prepare_pipe_ends(t_minishell *shell, int *pipefd)
{
	safe_close(shell, pipefd[1], "pipe close write end");
	safe_dup2(shell, pipefd[0], STDIN_FILENO, "pipe right command stdin");
	safe_close(shell, pipefd[0], "pipe close read end after dup");
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
