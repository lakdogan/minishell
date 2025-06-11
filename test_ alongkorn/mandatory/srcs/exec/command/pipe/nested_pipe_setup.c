#include "../../../../includes/core/minishell.h"

void	setup_nested_pipe_output(t_minishell *shell, int *pipefd)
{
	safe_close(shell, pipefd[PIPE_READ_END], "nested pipe close read end");
	safe_dup2(shell, pipefd[PIPE_WRITE_END], STDOUT_FILENO,
		"nested pipe left command stdout");
	safe_close(shell, pipefd[PIPE_WRITE_END],
		"nested pipe close write end after dup");
}

void	setup_nested_pipe_input(t_minishell *shell, int *pipefd)
{
	safe_close(shell, pipefd[PIPE_WRITE_END], "nested pipe close write end");
	safe_dup2(shell, pipefd[PIPE_READ_END], STDIN_FILENO,
		"nested pipe right command stdin");
	safe_close(shell, pipefd[PIPE_READ_END],
		"nested pipe close read end after dup");
}

void	execute_nested_cmds(t_minishell *shell, t_exec *exec)
{
	check_exec_has_valid_data(shell, exec);
	setup_output_redirections(shell, exec);
	execute_builtin_or_external_cmd(shell, exec);
}
