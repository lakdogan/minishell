/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_value_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 06:51:47 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 06:52:33 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

bool	is_quoted(const t_expand_ctx *ctx)
{
	return (ctx->token.quote_mask && ctx->token.quote_mask[ctx->i]);
}

bool	is_expandable_dollar(const t_expand_ctx *ctx)
{
	char	next;

	if (ctx->token.value[ctx->i] != '$')
		return (false);
	if (!ctx->token.quote_mask || ctx->token.quote_mask[ctx->i] != 0)
		return (false);
	next = ctx->token.value[ctx->i + 1];
	return (next && (ft_isalnum(next) || next == '_' || next == '?'
			|| next == '$'));
}

void	append_char_result(t_expand_ctx *ctx, char c)
{
	ctx->result[ctx->ri++] = c;
}

void	append_expanded_var(t_expand_ctx *ctx, const char *exp)
{
	size_t	explen;

	explen = ft_strlen(exp);
	memcpy(ctx->result + ctx->ri, exp, explen);
	ctx->ri += explen;
}

void	handle_quoted_char(t_expand_ctx *ctx)
{
	ctx->result[ctx->ri++] = ctx->token.value[ctx->i];
}
