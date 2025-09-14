/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_value.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 06:50:23 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 07:07:50 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

static void	handle_non_expandable_dollar(t_expand_ctx *ctx)
{
	ctx->result[ctx->ri++] = '$';
}

static void	handle_expandable_var(t_expand_ctx *ctx)
{
	size_t	varlen;
	size_t	maxlen;
	char	tmp[256];
	char	*exp;

	varlen = 0;
	maxlen = 255;
	while (ctx->token.value[ctx->i + varlen]
		&& (ft_isalnum(ctx->token.value[ctx->i + varlen])
			|| ctx->token.value[ctx->i + varlen] == '_'
			|| ctx->token.value[ctx->i + varlen] == '?')
		&& ctx->token.quote_mask[ctx->i + varlen] == 0 && varlen < maxlen)
	{
		varlen++;
	}
	ft_memcpy(tmp, ctx->token.value + ctx->i, varlen);
	tmp[varlen] = 0;
	exp = NULL;
	expand_single_var(tmp, &exp, ctx->shell);
	append_expanded_var(ctx, exp);
	ctx->i += varlen - 1;
}

static void	handle_regular_char(t_expand_ctx *ctx)
{
	ctx->result[ctx->ri++] = ctx->token.value[ctx->i];
}

static void	expand_token_loop(t_expand_ctx *ctx)
{
	ctx->i = 0;
	while (ctx->i < ctx->len)
	{
		if (is_quoted(ctx))
			handle_quoted_char(ctx);
		else if (is_expandable_dollar(ctx))
			handle_expandable_var(ctx);
		else if (ctx->token.value[ctx->i] == '$')
			handle_non_expandable_dollar(ctx);
		else
			handle_regular_char(ctx);
		ctx->i++;
	}
	ctx->result[ctx->ri] = 0;
}

char	*expand_token_value(t_token token, t_minishell *shell)
{
	t_expand_ctx	ctx;

	ctx.token = token;
	ctx.shell = shell;
	ctx.len = ft_strlen(token.value);
	ctx.result = gc_malloc(shell->gc[GC_COMMAND], ctx.len * 10 + 1);
	ctx.ri = 0;
	if (token.no_expand)
		return (gc_strdup(shell->gc[GC_COMMAND], token.value));
	expand_token_loop(&ctx);
	return (ctx.result);
}
