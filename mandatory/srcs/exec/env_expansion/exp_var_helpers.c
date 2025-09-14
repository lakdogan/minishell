/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_var_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 05:39:58 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 05:54:38 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

char	*expand_var_at_position(const char *str, int *i, t_minishell *shell)
{
	char	*expanded;
	int		consumed;

	expanded = NULL;
	consumed = expand_single_var(&str[*i], &expanded, shell);
	*i += consumed;
	return (expanded);
}

bool	is_active_escape_sequence(const char *str, int i, char quote_char)
{
	return (str[i] == BACKSLASH && str[i + NEXT_CHAR_INDEX] != NULL_TERMINATOR
		&& quote_char != SINGLE_QUOTE);
}

bool	is_escapable_char(char c)
{
	return (c == DOUBLE_QUOTE || c == SINGLE_QUOTE || c == BACKSLASH
		|| c == DOLLAR_SIGN);
}

char	*process_character(const char *str, int *i, char *quote_char,
		t_minishell *shell)
{
	char	new_quote_state;

	if (is_active_escape_sequence(str, *i, *quote_char))
	{
		if (is_escapable_char(str[*i + NEXT_CHAR_INDEX]))
		{
			(*i) += SKIP_BACKSLASH;
			return (append_char(NULL, str[*i], shell));
		}
		else
			return (append_char(NULL, str[*i], shell));
	}
	new_quote_state = handle_quote(str[*i], *quote_char);
	if (new_quote_state != *quote_char)
	{
		*quote_char = new_quote_state;
		return (append_char(NULL, str[*i], shell));
	}
	else if (should_expand_var(str, *i, *quote_char))
		return (expand_var_at_position(str, i, shell));
	else
		return (append_char(NULL, str[*i], shell));
}

bool	is_bare_question_mark(const char *str, int i, char quote_char)
{
	return (i == 0 && str[i] == '?' && quote_char == NO_QUOTE);
}
