

/**
 * @file executor.h
 * @brief Provides functions to traverse and
 * execute the command tree (AST),
 * including handling of pipelines,
 * logical operators, redirections, and builtin dispatch.
 *
 * TODO:
 * - Implement exec_tree(t_minishell *mini, t_command_tree *node)
 *   → Traverse the AST and execute each node type (N_EXEC, N_PIPE, N_ANDIF,
 * 	N_OR)
 *
 * - Handle command execution:
 *   → exec_command(): fork + execve, setup redirections
 *   → exec_builtin(): call builtin if applicable
 *
 * - Implement pipe handling:
 *   → setup and close pipes across multiple execs in pipelines
 *
 * - Implement logical operator handling:
 *   → &&: only execute right if left returns 0
 *   → ||: only execute right if left fails (non-zero)
 *
 * - Restore original stdin/stdout after redirections
 * - Set exit_code in minishell based on child status or builtin return
 */
#ifndef EXECUTOR_H
# define EXECUTOR_H

#endif