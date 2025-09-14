/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquoted_processor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 00:08:44 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 06:24:03 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Appends a character to unquoted result buffer.
static void	append_char_unquoted(char **res, char c, t_minishell *shell)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	*res = gc_strjoin(shell->gc[GC_COMMAND], *res, tmp);
}

// Checks if character is a quote.
static int	is_quote_char(char c)
{
	return (c == '\'' || c == '"');
}

// Processes a regular character in unquoted text.
static void	process_regular_char_uq(const char *input, size_t *i,
		t_minishell *shell, char **res)
{
	append_char_unquoted(res, input[*i], shell);
	(*i)++;
}

// Processes unquoted string.
char	*proc_unquoted(const char *input, size_t *i, t_minishell *shell)
{
	char	*res;

	res = gc_strdup(shell->gc[GC_COMMAND], "");
	while (input[*i] && !is_quote_char(input[*i]))
	{
		if (input[*i] == '\\')
		{
			process_bslashes(input, i, shell, &res);
			continue ;
		}
		if (input[*i] == '$')
		{
			process_dollar(input, i, shell, &res);
			continue ;
		}
		process_regular_char_uq(input, i, shell, &res);
	}
	return (res);
}
