/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quote_processor.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 00:03:34 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 06:35:40 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Appends a character to the buffer.
static void	append_char_to_buffer(char **res, char c, t_minishell *shell)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	*res = gc_strjoin(shell->gc[GC_COMMAND], *res, tmp);
}

// Processes a standard character inside double quotes.
static void	process_standard_char(const char *input, size_t *i,
		t_minishell *shell, char **res)
{
	append_char_to_buffer(res, input[*i], shell);
	(*i)++;
}

// Initializes result buffer and skips opening quote.
static char	*init_result_and_skip_quote(t_minishell *shell, size_t *i)
{
	char	*res;

	res = gc_strdup(shell->gc[GC_COMMAND], "");
	(*i)++;
	return (res);
}

static void	process_double_content(const char *input, size_t *i,
		t_minishell *shell, char **res)
{
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '\\')
		{
			process_bslashes(input, i, shell, res);
			continue ;
		}
		if (input[*i] == '$')
		{
			if (!input[*i + 1] || !(ft_isalnum(input[*i + 1]) || input[*i
						+ 1] == '_' || input[*i + 1] == '?' || input[*i
						+ 1] == '$'))
			{
				append_char_to_buffer(res, '$', shell);
				(*i)++;
				continue ;
			}
			process_dollar(input, i, shell, res);
			continue ;
		}
		process_standard_char(input, i, shell, res);
	}
}

char	*proc_double(const char *input, size_t *i, t_minishell *shell)
{
	char	*res;

	res = init_result_and_skip_quote(shell, i);
	process_double_content(input, i, shell, &res);
	if (input[*i] == '"')
		(*i)++;
	return (res);
}
