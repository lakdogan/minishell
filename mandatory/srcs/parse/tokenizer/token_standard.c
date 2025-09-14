/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_standard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:57:43 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 06:25:26 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

static int	get_token_len(const char *cmd, int *i)
{
	int	len;

	len = tok_len(cmd, i);
	return (len);
}

static int	handle_quotes(const char *cmd, int start, int len,
		t_token_state state)
{
	int	end;

	if (state == IN_SQUOTES || state == IN_DQUOTES)
	{
		end = start + 1;
		while (cmd[end] && cmd[end] != cmd[start])
			end++;
		if (cmd[end] == cmd[start])
			len = end - start + 1;
		else
			len = end - start;
	}
	return (len);
}

static void	set_token_bounds_and_quoted(t_standard_token_ctx ctx, int start,
		int len, t_token_state state)
{
	ctx.token->start = start;
	ctx.token->end = start + len;
	if (state == IN_SQUOTES || state == IN_DQUOTES)
		ctx.token->was_quoted = 1;
	else
		ctx.token->was_quoted = 0;
}

static t_bool	extract_and_set_token_value(t_extract_ctx ectx)
{
	t_extract_params	params;

	params.cmd = ectx.ctx.cmd;
	params.start = ectx.start;
	params.len = ectx.len;
	params.state = ectx.state;
	ectx.ctx.token->quote_mask = NULL;
	ectx.ctx.token->value = extract_token_value(params, ectx.shell,
			&ectx.ctx.token->quote_mask);
	if (!ectx.ctx.token->value)
		return (FALSE);
	set_token_properties(ectx.ctx.token, ectx.state);
	return (TRUE);
}

t_bool	process_standard_token(t_standard_token_ctx ctx, t_minishell *shell)
{
	int				start;
	int				len;
	t_token_state	state;
	t_extract_ctx	ectx;

	start = *(ctx.i);
	len = get_token_len(ctx.cmd, ctx.i);
	state = get_tok_state(ctx.cmd + start, len);
	len = handle_quotes(ctx.cmd, start, len, state);
	set_token_bounds_and_quoted(ctx, start, len, state);
	ectx.ctx = ctx;
	ectx.start = start;
	ectx.len = len;
	ectx.state = state;
	ectx.shell = shell;
	if (!extract_and_set_token_value(ectx))
		return (FALSE);
	if (ctx.token->state == UNCLOSED_QUOTES)
		return (FALSE);
	ctx.token->pos = ctx.t_count;
	*(ctx.i) = start + len;
	return (TRUE);
}
