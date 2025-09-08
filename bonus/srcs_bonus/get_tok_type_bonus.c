/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tok_type_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 00:32:29 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:31:02 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

// Returns the token type for operators like ||, &&, |, ;
static t_token_type	get_operator_type(char *value)
{
	if (ft_strcmp(value, "||") == 0)
		return (OR);
	if (ft_strcmp(value, "&&") == 0)
		return (AND_IF);
	if (ft_strcmp(value, "|") == 0)
		return (PIPE);
	if (ft_strcmp(value, ";") == 0)
		return (SEMICOLON);
	return (END);
}

// Returns the token type for redirections like >, <, >>, <<, 2>
static t_token_type	get_redirection_type(char *value)
{
	if (ft_strcmp(value, ">") == 0)
		return (OUTFILE);
	if (ft_strcmp(value, "<") == 0)
		return (INFILE);
	if (ft_strcmp(value, ">>") == 0)
		return (APPEND);
	if (ft_strcmp(value, "<<") == 0)
		return (HEREDOC);
	if (ft_strcmp(value, "2>") == 0)
		return (T_2GREATER);
	return (END);
}

// Returns the token type for parentheses.
static t_token_type	get_paren_type(char *value)
{
	if (ft_strcmp(value, "(") == 0)
		return (L_PAREN);
	if (ft_strcmp(value, ")") == 0)
		return (R_PAREN);
	return (END);
}

// Determines the token type for a given value.
t_token_type	get_tok_type(char *value)
{
	t_token_type	type;

	if (!value)
		return (END);
	type = get_operator_type(value);
	if (type != END)
		return (type);
	type = get_redirection_type(value);
	if (type != END)
		return (type);
	type = get_paren_type(value);
	if (type != END)
		return (type);
	if (is_valid_assignment(value))
		return (ASSIGNMENT);
	return (WORD);
}
