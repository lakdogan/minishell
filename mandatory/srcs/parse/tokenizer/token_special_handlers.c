/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_special_handlers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:57:32 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 21:21:24 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Handles 2> token.
t_token	handle_2greater_token(const char *cmd, int *i, int t_count,
		t_minishell *shell)
{
	t_token	token;

	if (!(cmd[*i] == '2' && cmd[*i + 1] == '>'))
		return (init_token_default());
	token = init_token_default();
	token.type = T_2GREATER;
	token.value = gc_strdup(shell->gc[GC_COMMAND], "2>");
	token.state = GENERAL;
	token.no_expand = 0;
	token.pos = t_count;
	token.start = *i;
	token.end = *i + 2;
	(*i) += 2;
	return (token);
}

// Handles ; token.
t_token	handle_semicolon_token(const char *cmd, int *i, int t_count,
		t_minishell *shell)
{
	t_token	token;

	if (cmd[*i] != ';')
		return (init_token_default());
	token = init_token_default();
	token.type = SEMICOLON;
	token.value = gc_strdup(shell->gc[GC_COMMAND], ";");
	token.state = GENERAL;
	token.no_expand = 0;
	token.pos = t_count;
	token.start = *i;
	token.end = *i + 2;
	(*i)++;
	return (token);
}

t_token	handle_greater_bar_token(const char *cmd, int *i, int t_count,
		t_minishell *shell)
{
	t_token	token;

	if (!(cmd[*i] == '>' && cmd[*i + 1] == '|'))
		return (init_token_default());
	token = init_token_default();
	token.type = T_GREATER_BAR;
	token.value = gc_strdup(shell->gc[GC_COMMAND], ">|");
	token.state = GENERAL;
	token.no_expand = 0;
	token.pos = t_count;
	token.start = *i;
	token.end = *i + 2;
	(*i) += 2;
	return (token);
}

// Tries to handle special tokens.
t_bool	try_handle_special_tokens(t_special_token_ctx ctx, t_minishell *shell)
{
	*(ctx.token) = handle_2greater_token(ctx.cmd, ctx.i, ctx.t_count, shell);
	if (ctx.token->type != END)
		return (TRUE);
	*(ctx.token) = handle_greater_bar_token(ctx.cmd, ctx.i, ctx.t_count, shell);
	if (ctx.token->type != END)
		return (TRUE);
	*(ctx.token) = handle_semicolon_token(ctx.cmd, ctx.i, ctx.t_count, shell);
	if (ctx.token->type != END)
		return (TRUE);
	return (FALSE);
}
