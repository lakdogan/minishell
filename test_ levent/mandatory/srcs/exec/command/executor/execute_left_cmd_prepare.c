#include "../../../../includes/core/minishell.h"

void	check_exec_outfile_and_prepare_pipes(t_minishell *shell, t_exec *exec,
		int *pipefd)
{
	safe_close(shell, pipefd[0], "pipe close read end");
	if (!exec->outfiles)
		safe_dup2(shell, pipefd[1], STDOUT_FILENO, "pipe left command stdout");
	safe_close(shell, pipefd[1], "pipe close write end after dup");
}

void	check_if_right_cmd_has_infile(t_command_tree *node, t_exec **right_exec,
		bool *right_has_input)
{
	*right_exec = node->right->data;
	*right_has_input = (*right_exec && (*right_exec)->infiles != NULL);
}

void	prepare_for_execute(t_minishell *shell, t_command_tree *node,
		t_exec *exec, int *pipefd)
{
	t_exec	*right_exec;
	bool	right_has_input;

	right_has_input = false;
	if (node->right->type == N_EXEC)
		check_if_right_cmd_has_infile(node, &right_exec, &right_has_input);
	setup_in_and_output_redirections(shell, exec);
	if (right_has_input)
		close_pipe(shell, pipefd);
	else
		check_exec_outfile_and_prepare_pipes(shell, exec, pipefd);
}
