/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:54:46 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 22:54:48 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

/* Validate redirection token has a valid target */
static int	validate_redirection(t_tokens *tokens, int *pos)
{
	if ((*pos) >= tokens->count || tokens->arr[*pos].type != WORD)
		return (0);
	return (1);
}

/* Process redirections based on type */
static int	process_io_redirection(t_redir_ctx ctx, t_token_type type)
{
	if (type == APPEND || type == OUTFILE || type == ERR_OUTFILE)
		return (process_output_redir(ctx, type));
	else if (type == INFILE)
		return (process_input_redir(ctx, type));
	(*(ctx.pos))++;
	return (1);
}

/* Main function to handle redirection tokens */
int	handle_redirection(t_tokens *tokens, int *pos, t_exec *exec,
		t_minishell *shell)
{
	t_token_type	type;
	t_redir_ctx		ctx;

	ctx.tokens = tokens;
	ctx.pos = pos;
	ctx.exec = exec;
	ctx.gc = shell->gc[GC_COMMAND];
	type = tokens->arr[*pos].type;
	(*pos)++;
	if (type == HEREDOC && (*pos) < tokens->count
		&& tokens->arr[*pos].type == WORD)
		return (handle_heredoc(ctx));
	else if (!validate_redirection(tokens, pos))
		return (0);
	else
		return (process_io_redirection(ctx, type));
}
