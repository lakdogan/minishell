/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:54:35 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 22:54:39 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

/* Handle heredoc redirection type */
int	handle_heredoc(t_redir_ctx ctx)
{
	if (*(ctx.pos) < ctx.tokens->count
		&& ctx.tokens->arr[*(ctx.pos)].type == WORD)
	{
		if (!add_infile(ctx.exec, HEREDOC, ctx.tokens->arr[*(ctx.pos)].value,
				ctx.gc))
			return (0);
		(*(ctx.pos))++;
		return (1);
	}
	return (0);
}

/* Handle output redirection types */
int	process_output_redir(t_redir_ctx ctx, t_token_type type)
{
	add_outfile(ctx.exec, type, ctx.tokens->arr[*(ctx.pos)].value, ctx.gc);
	(*(ctx.pos))++;
	return (1);
}

/* Handle input redirection types */
int	process_input_redir(t_redir_ctx ctx, t_token_type type)
{
	add_infile(ctx.exec, type, ctx.tokens->arr[*(ctx.pos)].value, ctx.gc);
	(*(ctx.pos))++;
	return (1);
}
