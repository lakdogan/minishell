/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:10:13 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/07 23:00:07 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

static char	*init_token_result(t_minishell *shell)
{
	return (gc_strdup(shell->gc[GC_COMMAND], ""));
}

static char	*handle_single_quote(const char *input, size_t *i, char *result,
		t_minishell *shell)
{
	char	*quoted_text;

	quoted_text = proc_single(input, i, shell);
	return (gc_strjoin(shell->gc[GC_COMMAND], result, quoted_text));
}

static char	*handle_double_quote(const char *input, size_t *i, char *result,
		t_minishell *shell)
{
	char	*quoted_text;

	quoted_text = proc_double(input, i, shell);
	return (gc_strjoin(shell->gc[GC_COMMAND], result, quoted_text));
}

static char	*handle_unquoted_text(const char *input, size_t *i, char *result,
		t_minishell *shell)
{
	char	*unquoted_text;

	unquoted_text = proc_unquoted(input, i, shell);
	return (gc_strjoin(shell->gc[GC_COMMAND], result, unquoted_text));
}

char	*process_token_full(const char *input, t_minishell *shell)
{
	size_t	i;
	char	*result;

	i = 0;
	result = init_token_result(shell);
	while (input[i])
	{
		if (input[i] == '\'')
			result = handle_single_quote(input, &i, result, shell);
		else if (input[i] == '"')
			result = handle_double_quote(input, &i, result, shell);
		else
			result = handle_unquoted_text(input, &i, result, shell);
	}
	return (result);
}
