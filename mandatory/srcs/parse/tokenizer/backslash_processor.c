/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backslash_processor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 23:56:19 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:34:31 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Counts consecutive backslashes in input.
static int	count_backslashes(const char *input, size_t *i)
{
	int	bs_count;

	bs_count = 0;
	while (input[*i] && input[*i] == '\\')
	{
		bs_count++;
		(*i)++;
	}
	return (bs_count);
}

// Appends backslashes to buffer.
static void	append_backslashes(t_minishell *shell, char **buf, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		*buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, "\\");
		j++;
	}
}

// Handles $ after backslashes.
static void	handle_dollar_after_bs(size_t *i, t_minishell *shell, char **buf,
		int bs_count)
{
	bs_count--;
	if (bs_count % 2 == 0)
	{
		append_backslashes(shell, buf, bs_count / 2);
	}
	else
	{
		append_backslashes(shell, buf, bs_count / 2);
		*buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, "\1");
		(*i)++;
	}
}

// Processes backslashes in input.
void	process_bslashes(const char *input, size_t *i, t_minishell *shell,
		char **buf)
{
	int	bs_count;

	bs_count = count_backslashes(input, i);
	if (input[*i] == '$')
		handle_dollar_after_bs(i, shell, buf, bs_count);
	else
		append_backslashes(shell, buf, bs_count);
}
