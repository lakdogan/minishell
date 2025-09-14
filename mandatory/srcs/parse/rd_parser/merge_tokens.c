/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 06:42:01 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 23:43:42 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

static void	calc_total_len(t_merge_ctx *ctx)
{
	ctx->total_len = 0;
	ctx->k = ctx->start;
	while (ctx->k <= ctx->in)
	{
		ctx->total_len += ft_strlen(ctx->tokens->arr[ctx->k].value);
		ctx->k++;
	}
}

static void	merge_values_and_masks(t_merge_ctx *ctx)
{
	ctx->merged = gc_malloc(ctx->shell->gc[GC_MAIN], ctx->total_len + 1);
	ctx->merged_mask = gc_malloc(ctx->shell->gc[GC_MAIN], ctx->total_len + 1);
	ctx->merged[0] = '\0';
	ctx->pos = 0;
	ctx->k = ctx->start;
	while (ctx->k <= ctx->in)
	{
		ctx->len = ft_strlen(ctx->tokens->arr[ctx->k].value);
		ft_strlcat(ctx->merged, ctx->tokens->arr[ctx->k].value, ctx->total_len
			+ 1);
		ft_memcpy(
			ctx->merged_mask + ctx->pos, ctx->tokens->arr[ctx->k].quote_mask,
			ctx->len);
		ctx->pos += ctx->len;
		ctx->k++;
	}
	ctx->merged_mask[ctx->total_len] = 0;
}

static void	update_token_properties(t_merge_ctx *ctx)
{
	ctx->tokens->arr[ctx->start].value = ctx->merged;
	ctx->tokens->arr[ctx->start].quote_mask = ctx->merged_mask;
	ctx->tokens->arr[ctx->start].end = ctx->tokens->arr[ctx->in].end;
	ctx->tokens->arr[ctx->start].was_quoted = 0;
	ctx->k = ctx->start;
	while (ctx->k <= ctx->in)
	{
		ctx->tokens->arr[ctx->start].was_quoted
			|= ctx->tokens->arr[ctx->k].was_quoted;
		ctx->k++;
	}
	ctx->tokens->arr[ctx->start].no_expand = 0;
	ctx->j = 0;
	while (ctx->j < ctx->total_len)
	{
		if (ctx->merged_mask[ctx->j])
		{
			ctx->tokens->arr[ctx->start].no_expand = 1;
			break ;
		}
		ctx->j++;
	}
}

void	merge_adjacent_tokens(t_tokens *tokens, t_minishell *shell)
{
	t_merge_ctx	ctx;

	ctx.tokens = tokens;
	ctx.shell = shell;
	ctx.out = 0;
	ctx.in = 0;
	while (ctx.in < ctx.tokens->count)
	{
		ctx.start = ctx.in;
		while (ctx.in + 1 < ctx.tokens->count
			&& ctx.tokens->arr[ctx.in].end == ctx.tokens->arr[ctx.in + 1].start
			&& ctx.tokens->arr[ctx.in].type == WORD && ctx.tokens->arr[ctx.in
				+ 1].type == WORD)
			ctx.in++;
		if (ctx.start != ctx.in)
		{
			calc_total_len(&ctx);
			merge_values_and_masks(&ctx);
			update_token_properties(&ctx);
		}
		ctx.tokens->arr[ctx.out++] = ctx.tokens->arr[ctx.start];
		ctx.in++;
	}
	ctx.tokens->count = ctx.out;
}
