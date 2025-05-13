

/**
 * @file parser.h
 * @brief Provides functions to parse the
 * token list into an abstract syntax tree (AST),
 *        detect syntax errors, and construct command nodes for execution.
 *
 * TODO:
 * - Implement parse(t_minishell *mini)
 *   → Entry point: validate tokens and build AST root (t_command_tree *)
 *
 * - Implement syntax_error_check(t_list *tokens)
 *   → Check for unclosed quotes, misplaced pipes, invalid redirections
 *
 * - Implement AST builder functions:
 *   → build_tree(): convert token list to t_command_tree structure
 *   → create_exec_node(), create_pipe_node(), create_logic_nodes()
 *
 * - Handle operator precedence: ( ), &&, ||, | correctly
 * - Support multiple commands and pipelines
 * - Detect and report detailed syntax errors
 */
#ifndef PARSER_H
# define PARSER_H

#endif