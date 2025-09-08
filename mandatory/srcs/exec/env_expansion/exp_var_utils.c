/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:20:39 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:01:33 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Handles quote state transitions.
char	handle_quote(char c, char quote_char)
{
	if ((c == SINGLE_QUOTE || c == DOUBLE_QUOTE) && (quote_char == NO_QUOTE
			|| quote_char == c))
	{
		if (quote_char == NO_QUOTE)
			return (c);
		else
			return (NO_QUOTE);
	}
	return (quote_char);
}

// Appends a character to the result string.
char	*append_char(char *result, char c, t_minishell *shell)
{
	char	tmp[CHAR_BUFFER_SIZE];
	char	*joined;

	tmp[FIRST_CHAR] = c;
	tmp[NULL_TERMINATOR_INDEX] = NULL_TERMINATOR;
	if (!result)
		return (gc_strdup(shell->gc[GC_EXPAND], tmp));
	joined = gc_strjoin(shell->gc[GC_EXPAND], result, tmp);
	if (!joined)
		return (result);
	return (joined);
}

// Checks if a variable should be expanded at position i.
bool	should_expand_var(const char *str, int i, char quote_char)
{
	return (str[i] == DOLLAR_SIGN && str[i + NEXT_CHAR_INDEX]
		&& (quote_char == NO_QUOTE || quote_char == DOUBLE_QUOTE)
		&& (is_var_char(str[i + NEXT_CHAR_INDEX]) || str[i
				+ NEXT_CHAR_INDEX] == QUESTION_MARK || str[i
				+ NEXT_CHAR_INDEX] == DOLLAR_SIGN));
}
