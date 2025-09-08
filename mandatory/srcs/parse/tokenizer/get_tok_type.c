/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tok_type.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 23:24:20 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:36:46 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Checks for redirection tokens.
static t_token_type	check_redirection_tokens(char *value)
{
	if (ft_strncmp(value, "2>", 2) == 0)
		return (T_2GREATER);
	if (ft_strncmp(value, ">>", 2) == 0)
		return (APPEND);
	if (ft_strncmp(value, "<<", 2) == 0)
		return (HEREDOC);
	if (ft_strncmp(value, ">", 1) == 0)
		return (OUTFILE);
	if (ft_strncmp(value, "<", 1) == 0)
		return (INFILE);
	return (WORD);
}

// Checks for operator tokens.
static t_token_type	check_operator_tokens(char *value)
{
	if (ft_strncmp(value, "&&", 2) == 0)
		return (AND_IF);
	if (ft_strncmp(value, "||", 2) == 0)
		return (OR);
	if (ft_strncmp(value, "|", 1) == 0)
		return (PIPE);
	if (ft_strncmp(value, "&", 1) == 0)
		return (AND_IF);
	return (WORD);
}

// Checks for parenthesis tokens.
static t_token_type	check_parenthesis_tokens(char *value)
{
	if (ft_strncmp(value, "(", 1) == 0)
		return (L_PAREN);
	if (ft_strncmp(value, ")", 1) == 0)
		return (R_PAREN);
	return (WORD);
}

// Determines token type from value.
t_token_type	get_tok_type(char *value)
{
	t_token_type	type;

	if (!value)
		return (WORD);
	if (is_valid_assignment(value))
		return (ASSIGNMENT);
	type = check_redirection_tokens(value);
	if (type != WORD)
		return (type);
	type = check_operator_tokens(value);
	if (type != WORD)
		return (type);
	type = check_parenthesis_tokens(value);
	if (type != WORD)
		return (type);
	return (WORD);
}
