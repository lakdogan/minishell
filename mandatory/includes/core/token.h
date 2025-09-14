/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 08:48:58 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/14 06:51:08 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file token.h
 * @brief Defines token types and states used
 * by the lexer to tokenize the user input into structured data.
 *
 * TODO:
 * - Implement lexer to convert input into t_token list
 * - Handle quotes, expansions, operators and heredoc delimiters
 * - Track token position and quoting state
 * - Provide token join/merge utilities
 */
#ifndef TOKEN_H
# define TOKEN_H

# include "minishell.h"

# ifdef FALSE
#  undef FALSE
# endif
# ifdef TRUE
#  undef TRUE
# endif

typedef enum e_bool
{
	FALSE = 0,
	TRUE = 1
}						t_bool;

typedef enum e_token_type
{
	WORD,
	PIPE,
	T_2GREATER,
	T_GREATER_BAR,
	APPEND,
	OUTFILE,
	INFILE,
	HEREDOC,
	AND_IF,
	OR,
	END,
	L_PAREN,
	R_PAREN,
	ASSIGNMENT,
	SEMICOLON,
	ERR_OUTFILE,
}						t_token_type;

typedef enum e_token_state
{
	GENERAL,
	IN_DQUOTES,
	IN_SQUOTES,
	UNCLOSED_QUOTES,
	EXPAND,
}						t_token_state;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	t_token_state		state;
	int					pos;
	bool				no_expand;
	bool				was_quoted;
	int					start;
	int					end;
	char				*quote_mask;
}						t_token;

typedef struct s_tokens
{
	t_token				*arr;
	int					count;
}						t_tokens;

typedef struct s_extract_params
{
	const char			*cmd;
	int					start;
	int					len;
	t_token_state		state;
}						t_extract_params;

typedef struct s_token_context
{
	t_token				**tokens;
	const char			*cmd;
	int					*i;
	int					t_count;
	t_minishell			*shell;
	int					*heredoc_next;
}						t_token_context;

typedef struct s_standard_token_ctx
{
	const char			*cmd;
	int					*i;
	int					t_count;
	t_token				*token;
}						t_standard_token_ctx;

typedef struct s_special_token_ctx
{
	const char			*cmd;
	int					*i;
	int					t_count;
	t_token				*token;
}						t_special_token_ctx;

typedef struct s_extract_ctx_internal
{
	t_extract_params	params;
	t_minishell			*shell;
	char				**quote_mask;
}						t_extract_ctx_internal;

typedef struct s_expand_ctx
{
	t_token				token;
	t_minishell			*shell;
	char				*result;
	size_t				ri;
	size_t				i;
	size_t				len;
}						t_expand_ctx;

int						lexer_loop(t_token **tokens, const char *cmd, int *cap,
							t_minishell *shell);
int						get_op_len(const char *cmd, int i);
int						is_operator(const char *s);
int						handle_redirection_out(const char *cmd, int *i);
int						check_for_qoutes(const char *cmd, int *i, int *len);
void					update_quote_state(char c, int *in_single,
							int *in_double);
int						check_for_paren(const char *cmd, int *i, int len);
bool					is_token_boundary(const char *cmd, int pos,
							int in_single, int in_double);
int						get_word_len(const char *cmd, int *i);
int						tok_len(const char *cmd, int *i);
int						is_valid_assignment(const char *s);
char					*get_env_var(t_minishell *shell, const char *var);
char					*restore_escaped_dollars(char *s, t_minishell *shell);
void					process_dollar(const char *input, size_t *i,
							t_minishell *shell, char **buf);
int						count_digits(int n);
char					*int_to_str(int n, t_minishell *shell);
int						handle_exit_status(const char *input, size_t *i,
							t_minishell *shell, char **buf);
int						handle_pid_var(const char *input, size_t *i,
							t_minishell *shell, char **buf);
int						process_special_var(const char *input, size_t *i,
							t_minishell *shell, char **buf);
void					process_bslashes(const char *input, size_t *i,
							t_minishell *shell, char **buf);
char					*proc_double(const char *input, size_t *i,
							t_minishell *shell);
char					*proc_unquoted(const char *input, size_t *i,
							t_minishell *shell);
char					*proc_single(const char *input, size_t *i,
							t_minishell *shell);
char					*process_token_full(t_token *token, t_minishell *shell);

t_token					init_token_default(void);
t_token					init_token(const char *cmd, int *i, int t_count,
							t_minishell *shell);

t_token					handle_2greater_token(const char *cmd, int *i,
							int t_count, t_minishell *shell);
t_token					handle_semicolon_token(const char *cmd, int *i,
							int t_count, t_minishell *shell);
t_bool					try_handle_special_tokens(t_special_token_ctx ctx,
							t_minishell *shell);

void					process_token(t_token *token, t_minishell *shell);
char					*extract_token_value(t_extract_params params,
							t_minishell *shell, char **quote_mask);
void					set_token_properties(t_token *token,
							t_token_state state);
void					apply_token_expansion(t_token *token,
							t_minishell *shell);
t_bool					process_standard_token(t_standard_token_ctx ctx,
							t_minishell *shell);
t_token					handle_greater_bar_token(const char *cmd, int *i,
							int t_count, t_minishell *shell);
#endif
