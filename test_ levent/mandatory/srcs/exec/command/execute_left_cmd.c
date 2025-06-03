#include "../../../includes/core/minishell.h"

void    check_exec_outfile_and_prepare_pipes(t_minishell *shell, t_exec *exec, int *pipefd)
{
    safe_close(shell, pipefd[0], "pipe close read end");
    if (!exec->outfiles)

}