/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote_processor.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 00:16:27 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:39:30 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Finds start of single-quoted content.
static size_t	find_content_start(size_t *pos)
{
	size_t	start;

	(*pos)++;
	start = *pos;
	return (start);
}

// Finds closing single quote.
static void	find_closing_quote(const char *input, size_t *pos)
{
	while (input[*pos] && input[*pos] != '\'')
		(*pos)++;
}

// Extracts content between single quotes.
static char	*extract_quoted_content(const char *input, size_t start, size_t end,
		t_minishell *shell)
{
	return (gc_substr(shell->gc[GC_COMMAND], input, start, end - start));
}

// Skips closing single quote.
static void	skip_closing_quote(const char *input, size_t *pos)
{
	if (input[*pos] == '\'')
		(*pos)++;
}

// Processes a single-quoted string.
char	*proc_single(const char *input, size_t *i, t_minishell *shell)
{
	size_t	content_start;
	char	*quoted_content;

	content_start = find_content_start(i);
	find_closing_quote(input, i);
	quoted_content = extract_quoted_content(input, content_start, *i, shell);
	skip_closing_quote(input, i);
	return (quoted_content);
}
