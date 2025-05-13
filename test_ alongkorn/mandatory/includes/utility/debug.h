

/**
 * @file debug.h
 * @brief Provides debugging utilities to print internal
 *        structures such as tokens, environment, AST, and exec nodes.
 *
 * TODO:
 * - Implement print_tokens(t_list *token_list)
 *   → Show each token’s value, type, and state
 *
 * - Implement print_env(t_list *envp)
 *   → Show environment key=value pairs with flags (is_export, printed)
 *
 * - Implement print_ast(t_command_tree *root)
 *   → Recursively print the AST with indentation (N_EXEC, N_PIPE, etc.)
 *
 * - Implement print_exec(t_exec *exec_node)
 *   → Show argv, infiles, outfiles, redirection types
 *
 * - Optional: Add color highlighting for categories (tokens, AST nodes)
 * - Optional: Add debug_mode flag to toggle printing
 */
#ifndef DEBUG_H
# define DEBUG_H

#endif