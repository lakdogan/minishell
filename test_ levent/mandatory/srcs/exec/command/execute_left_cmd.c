#include "../../../includes/core/minishell.h"

void    check_exec_outfile_and_prepare_pipes(t_minishell *shell, t_exec *exec, int *pipefd)
{
    safe_close(shell, pipefd[0], "pipe close read end");
    if (!exec->outfiles)
        safe_dup2(shell, pipefd[1], STDOUT_FILENO, "pipe left command stdout");
    safe_close(shell, pipefd[1], "pipe close write end after dup");
}

void    check_if_right_cmd_has_infile(t_command_tree *node, t_exec **right_exec, bool *right_has_input)
{
    *right_exec = node->right->data;
    *right_has_input = (*right_exec && (*right_exec)->infiles != NULL);
}

void    prepare_for_execute(t_minishell *shell, t_command_tree *node, t_exec *exec, int *pipefd)
{
    t_exec  *right_exec;
    bool    right_has_input;

    right_has_input = false;
    if (node->right->type == N_EXEC)
        check_if_right_cmd_has_infile(node, &right_exec, &right_has_input);
    setup_in_and_output_redirections(shell, exec);
    if (right_has_input)
        close_pipe(shell, pipefd);
    else
        check_exec_outfile_and_prepare_pipes(shell, exec, pipefd);
}

void    handle_standard_left_cmds(t_minishell *shell, t_command_tree *node, int *pipefd)
{
    t_exec  *exec;

    exec = node->left->data;
    check_exec_has_valid_data(shell, exec); // not existing yet
    prepare_for_execute(shell, node, exec, pipefd);
    execute_builtin_or_external_cmd(shell, exec); // not existing yet
}

void    setup_default_pipe_output(t_minishell *shell, int *pipefd)
{
    safe_close(shell, pipefd[0], "pipe close read end");
    safe_dup2(shell, pipefd[1], STDOUT_FILENO, "pipe left command stdout");
    safe_close(shell, pipefd[1], "pipe close write end after dup");
}

void    execute_left_subtree(t_minishell *shell, t_command_tree *node, int *pipefd)
{
    if (node->left->type == N_EXEC)
        handle_standard_left_cmds(shell, node, pipefd);
    else
        execute_left_subtree(shell, node, pipefd);
}

void    execute_left_cmd(t_command_tree *node, t_minishell *shell, int *pipefd)
{
    setup_child_signals();
    execute_left_by_type(shell, node, pipefd); // not existing yet
    exit(shell->exit_code);
}
