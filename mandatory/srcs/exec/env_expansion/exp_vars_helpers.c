/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_vars_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 05:51:09 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 06:06:56 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Appends a regular character to the result buffer.
void	process_regular_char(char c, char **result, t_minishell *shell)
{
	char	tmp[CHAR_BUFFER_SIZE];

	tmp[FIRST_CHAR] = c;
	tmp[NULL_TERMINATOR_INDEX] = NULL_TERMINATOR;
	*result = gc_strjoin(shell->gc[GC_EXPAND], *result, tmp);
}

// Checks if the current position in the string is a variable reference.
bool	is_variable_reference(const char *str, int i)
{
	return (str[i] == DOLLAR_SIGN && str[i + NEXT_CHAR_INDEX]
		&& (is_var_char(str[i + NEXT_CHAR_INDEX]) || str[i
				+ NEXT_CHAR_INDEX] == QUESTION_MARK || str[i
				+ NEXT_CHAR_INDEX] == DOLLAR_SIGN));
}

bool	is_tilde_expansion(const char *str)
{
	return (str && str[0] == '~' && (str[1] == '\0' || str[1] == '/'));
}

char	*expand_tilde_if_needed(const char *str, t_minishell *shell)
{
	if (is_tilde_expansion(str))
		return (expand_tilde(str, shell));
	return (NULL);
}
