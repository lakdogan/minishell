

/**
 * @file free.h
 * @brief Provides functions to safely free all dynamically allocated memory
 *        used in the Minishell project, including tokens, AST, environment,
 *        redirections, and command structures.
 *
 * TODO:
 * - Implement free_minishell(t_minishell *mini)
 *   → Frees all components: envp list, token list, AST, input string, etc.
 * 
 * - Implement free_token_list(t_list *tokens)
 *   → Frees each t_token and the list node
 * 
 * - Implement free_command_tree(t_command_tree *node)
 *   → Recursively free all AST nodes (exec, pipe, and/or)
 * 
 * - Implement free_exec(t_exec *exec)
 *   → Free argv, infiles, outfiles
 * 
 * - Implement free_envp(t_list *envp)
 *   → Free each environment variable node
 * 
 * - Optional: add debug-safe versions (e.g., debug_free)
 */
#ifndef FREE_H
# define FREE_H

#endif