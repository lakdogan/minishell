

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

typedef struct s_arg_lst
{
	char				*arg;
	bool				no_expand;
	struct s_arg_lst	*next;
}						t_arg_lst;


/*-----EROR---------------------------------------------------------------------------------*/
void				error_msg(t_tokens *tokens, int *pos);
void				error_msg_paren(void);
/*-----EROR---------------------------------------------------------------------------------*/

/*-----lexer--------------------------------------------------------------------------------*/
//main functions:
int					tok_len(const char *cmd, int *i);
t_tokens			*lexer(const char *cmd, t_minishell *shell);
t_token				init_token(const char *cmd, int *i, const int t_count, t_gc *gc);
//side functions:
t_token				*resize_array(t_token *tokens, int *cap, t_gc *gc);
t_token_type		get_tok_type(char *value);
t_token_state		get_tok_state(char *value, int len);
int					check_for_paren(const char *cmd, int *i, int len);
int					check_for_qoutes(const char *cmd, int *i, int *len);
int					is_token_valid(char *value, t_token_state state);
/*-----lexer-------------------------------------------------------------------------------*/

/*------AST--------------------------------------------------------------------------------*/
//main function for parsing:
t_command_tree		*start_parser(t_tokens *tokens, t_gc *gc, t_minishell *shell);
t_command_tree		*parse_sub_or_cmd(t_tokens *tokens, int *pos, t_gc *gc);
t_command_tree		*parse_cmd(t_tokens *tokens, int *pos, t_gc *gc);
t_command_tree		*parse_subshell(t_tokens *tokens, int *pos, t_gc *gc);
t_command_tree		*parse_pipeline(t_tokens *tokens, int *pos, t_gc *gc);
t_command_tree		*parse_logic_ops(t_tokens *tokens, int *pos, t_gc *gc);
//helper functions for parsing:
void				arg_to_list(t_arg_lst **head, char *value, bool no_expand, t_gc *gc);
char				**lst_to_argv(t_arg_lst *head, t_gc *gc);
int					handle_redir(t_exec	*exec, t_tokens *tokens, int *pos, t_gc *gc);
int					is_redir(t_token_type type);
int					add_outfile(t_exec *exec, t_token_type type, char *filename, t_gc *gc);
int					add_infile(t_exec *exec, t_token_type type, char *filename, t_gc *gc);
int					handle_sub_redir(t_command_tree *node, t_tokens *tokens, int *pos,
					t_gc *gc);
t_command_tree		*node_init(t_node_type type, t_gc *gc);
t_command_tree		*create_new_node(t_node_type type, t_gc *gc);
t_command_tree		*get_node(t_node_type type, t_command_tree *left, t_command_tree *right,
					t_gc *gc);
t_arg_lst			*get_args(t_exec *exec, t_tokens *tokens, int *pos, t_gc *gc);
/*------AST------------------------------------------------------------------------------*/

char *remove_quotes(char *token_value, t_gc *gc);
#endif
