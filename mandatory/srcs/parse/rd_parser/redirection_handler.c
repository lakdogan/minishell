/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 01:24:34 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:50:28 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Checks if the redirection is valid at the given position.
static int	is_valid_redirection(t_token_type type, t_tokens *tokens, int pos)
{
	if (type != HEREDOC && (pos > tokens->count
			|| tokens->arr[pos - 1].type != WORD))
		return (0);
	if (type == HEREDOC && (pos > tokens->count
			|| tokens->arr[pos - 1].type != WORD))
		return (0);
	return (1);
}

// Handles heredoc redirection.
static int	handle_heredoc_rd(t_redir_ctx ctx)
{
	int	success;

	if (*(ctx.pos) < ctx.tokens->count
		&& ctx.tokens->arr[*(ctx.pos)].type == WORD)
	{
		success = add_infile(ctx.exec, HEREDOC,
				ctx.tokens->arr[*(ctx.pos)].value, ctx.gc);
		(*(ctx.pos))++;
		return (success);
	}
	return (0);
}

// Processes input/output redirections.
static int	process_io_redirection(t_redir_ctx ctx, t_token_type type)
{
	if (type == APPEND || type == OUTFILE || type == ERR_OUTFILE)
		return (add_outfile(ctx.exec, type,
				ctx.tokens->arr[*(ctx.pos) - 1].value, ctx.gc));
	else if (type == INFILE)
		return (add_infile(ctx.exec, type,
				ctx.tokens->arr[*(ctx.pos) - 1].value, ctx.gc));
	return (1);
}

// Handles a redirection token in the context.
int	handle_redir(t_redir_ctx ctx)
{
	t_token_type	type;
	int				success;

	type = ctx.tokens->arr[*(ctx.pos)].type;
	(*(ctx.pos))++;
	if (type == HEREDOC)
		success = handle_heredoc_rd(ctx);
	else
		success = 1;
	if (!success || !is_valid_redirection(type, ctx.tokens, *(ctx.pos)))
		return (0);
	if (type != HEREDOC)
		success = process_io_redirection(ctx, type);
	return (success);
}
