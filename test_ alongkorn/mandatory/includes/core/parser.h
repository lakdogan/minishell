

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
# include "minishell.h"
/*-----lexer-----*/
//main functions:
int				tok_len(const char *cmd, int *i);
t_token			*lexer(const char *cmd, t_minishell *shell);
t_token			init_token(const char *cmd, int *i, const int t_count, t_gc *gc);
//side functions:
static int		lexer_loop(t_token **tokens, const char *cmd, int *cap, t_gc *gc[GC_COUNT]);
int				is_special_char(char c);
int				check_next_char(const char *cmd, int *i, int *len);
t_token			*resize_array(t_token *tokens, int *cap, t_gc *gc[GC_COUNT]);
t_token_type	get_tok_type(char *value);
t_token_state	get_toke_state(char *value, int len);
/*-----lexer-----*/

#endif
