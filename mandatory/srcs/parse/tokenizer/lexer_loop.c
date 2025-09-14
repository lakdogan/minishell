/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:53:16 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 06:33:39 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Skips whitespace characters in command string.
static void	skip_whitespace(const char *cmd, int *i)
{
	while (ft_isspace(cmd[*i]))
		(*i)++;
}

// Resizes token array if needed.
static bool	resize_token_array_if_needed(t_token **tokens, int t_count,
		int *cap, t_gc *gc)
{
	if (t_count >= (*cap))
	{
		(*tokens) = resize_array(*tokens, cap, gc);
		if (!(*tokens))
			return (false);
	}
	return (true);
}

// Processes a single token in the loop.
static bool	process_token_in_loop(t_token_context context)
{
	context.shell->is_heredoc = *(context.heredoc_next);
	(*(context.tokens))[context.t_count] = init_token(context.cmd, context.i,
			context.t_count, context.shell);
	if ((*(context.tokens))[context.t_count].type == HEREDOC)
		*(context.heredoc_next) = 1;
	else
		*(context.heredoc_next) = 0;
	if (!(*(context.tokens))[context.t_count].value)
		return (false);
	return (true);
}

// Main lexer loop for tokenizing input.
int	lexer_loop(t_token **tokens, const char *cmd, int *cap, t_minishell *shell)
{
	int				i;
	int				t_count;
	int				heredoc_next;
	t_token_context	context;

	heredoc_next = 0;
	i = 0;
	t_count = 0;
	while (cmd[i])
	{
		skip_whitespace(cmd, &i);
		if (!cmd[i])
			break ;
		if (!resize_token_array_if_needed(tokens, t_count, cap,
				shell->gc[GC_COMMAND]))
			return (-1);
		context = (t_token_context){tokens, cmd, &i, t_count, shell,
			&heredoc_next};
		if (!process_token_in_loop(context))
			return (-1);
		t_count++;
	}
	return (t_count);
}
