#include "../../../includes/core/execution.h"

void initialize_exec(t_exec *exec)
{
    exec->heredoc_fd = INVALID_FD;
    exec->heredoc_prepared = false;
}
