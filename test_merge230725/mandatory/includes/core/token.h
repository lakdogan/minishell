

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

typedef enum e_token_type
{
	WORD,
	PIPE,
	APPEND,
	OUTFILE,
	INFILE,
	HEREDOC,
	AND_IF,
	OR,
	END,
	L_PAREN,
	R_PAREN,
}					t_token_type;

typedef enum e_token_state
{
	GENERAL,
	IN_DQUOTES,
	IN_SQUOTES,
	UNCLOSED_QUOTES,
	EXPAND,
}					t_token_state;

typedef struct s_token
{
	char			*value;		// String value of token
	t_token_type	type;		// e.g., WORD, PIPE, etc.
	t_token_state	state;		// Quoting/expansion context
	int				pos;		// Position in the input string
	bool			no_expand;	// flag to prevent expansion
}					t_token;

typedef struct	s_tokens
{
	t_token	*arr;
	int		count;
}			t_tokens;

#endif
