/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_standard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:57:43 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/07 23:00:27 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

t_bool	process_standard_token(t_standard_token_ctx ctx, t_minishell *shell)
{
	int					start;
	int					len;
	t_token_state		state;
	t_extract_params	params;

	start = *(ctx.i);
	len = tok_len(ctx.cmd, ctx.i);
	if (len == -1)
		return (FALSE);
	state = get_tok_state(ctx.cmd + start, len);
	params.cmd = ctx.cmd;
	params.start = start;
	params.len = len;
	params.state = state;
	ctx.token->value = extract_token_value(params, shell);
	if (!ctx.token->value)
		return (FALSE);
	set_token_properties(ctx.token, state);
	if (ctx.token->state == UNCLOSED_QUOTES)
		return (FALSE);
	ctx.token->pos = ctx.t_count;
	*(ctx.i) = start + len;
	return (TRUE);
}
