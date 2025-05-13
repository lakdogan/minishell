

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
	PARENTHESSIS,
}					t_token_type;

typedef enum e_token_state
{
	GENERAL,
	IN_DQUOTES,
	IN_SQUOTES,
	EXPAND,
}					t_token_state;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_token_state	state;
	int				pos;
}					t_token;

#endif
