/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquoted_processor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 00:08:44 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/07 23:01:46 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

static void	append_char_unquoted(char **res, char c, t_minishell *shell)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	*res = gc_strjoin(shell->gc[GC_COMMAND], *res, tmp);
}

static int	is_quote_char(char c)
{
	return (c == '\'' || c == '"');
}

static void	process_regular_char(const char *input, size_t *i,
		t_minishell *shell, char **res)
{
	append_char_unquoted(res, input[*i], shell);
	(*i)++;
}

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
		process_regular_char(input, i, shell, &res);
	}
	return (res);
}
