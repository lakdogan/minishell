/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 08:48:58 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/05 23:56:09 by almatsch         ###   ########.fr       */
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
	char			*value;
	t_token_type	type;
	t_token_state	state;
	int				pos;
	bool			no_expand;
}					t_token;

typedef struct s_tokens
{
	t_token	*arr;
	int		count;
}			t_tokens;

#endif
