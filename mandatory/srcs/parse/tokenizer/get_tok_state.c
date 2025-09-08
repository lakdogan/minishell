/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tok_state.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 23:27:04 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:36:15 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Determines token state based on quotes.
static int	is_quoted_token(const char *value)
{
	return (value[0] == '\'' || value[0] == '"');
}

// Checks if quotes are matching.
static int	has_matching_quotes(const char *value, int len)
{
	if (len < 2)
		return (0);
	return (value[0] == value[len - 1]);
}

// Returns quote state for given char.
static t_token_state	get_quote_state(char quote_char)
{
	if (quote_char == '"')
		return (IN_DQUOTES);
	else if (quote_char == '\'')
		return (IN_SQUOTES);
	return (GENERAL);
}

// Gets token state based on quotes.
t_token_state	get_tok_state(const char *value, int len)
{
	if (!value || len <= 0)
		return (GENERAL);
	if (is_quoted_token(value))
	{
		if (has_matching_quotes(value, len))
			return (get_quote_state(value[0]));
		else
			return (UNCLOSED_QUOTES);
	}
	return (GENERAL);
}
