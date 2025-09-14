/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_properties.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 00:36:44 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 06:30:08 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

void	process_token(t_token *token, t_minishell *shell)
{
	char	*new_value;

	if (token->value && ft_strlen(token->value) == 1 && token->value[0] == '$')
		return ;
	if (token->was_quoted && token->value && ft_strlen(token->value) == 1
		&& token->value[0] == '$')
		return ;
	new_value = process_token_full(token, shell);
	new_value = restore_escaped_dollars(new_value, shell);
	token->value = new_value;
}

static char	*extract_quoted_value(t_extract_ctx_internal ctx,
		size_t *substr_len)
{
	char	*val;

	*substr_len = ctx.params.len - 2;
	val = gc_substr(ctx.shell->gc[GC_COMMAND], ctx.params.cmd, ctx.params.start
			+ 1, *substr_len);
	if (ctx.quote_mask)
	{
		*ctx.quote_mask = gc_malloc(ctx.shell->gc[GC_COMMAND], *substr_len + 1);
		memset(*ctx.quote_mask, 1, *substr_len);
		(*ctx.quote_mask)[*substr_len] = 0;
	}
	return (val);
}

static char	*extract_unquoted_value(t_extract_ctx_internal ctx)
{
	char	*val;

	val = gc_substr(ctx.shell->gc[GC_COMMAND], ctx.params.cmd, ctx.params.start,
			ctx.params.len);
	if (ctx.quote_mask)
	{
		*ctx.quote_mask = gc_malloc(ctx.shell->gc[GC_COMMAND], ctx.params.len
				+ 1);
		memset(*ctx.quote_mask, 0, ctx.params.len);
		(*ctx.quote_mask)[ctx.params.len] = 0;
	}
	return (val);
}

char	*extract_token_value(t_extract_params params, t_minishell *shell,
		char **quote_mask)
{
	t_extract_ctx_internal	ctx;
	size_t					substr_len;

	ctx.params = params;
	ctx.shell = shell;
	ctx.quote_mask = quote_mask;
	if ((params.state == IN_SQUOTES || params.state == IN_DQUOTES)
		&& params.len >= 2
		&& params.cmd[params.start] == params.cmd[params.start + params.len - 1]
		&& (params.cmd[params.start] == '\''
			|| params.cmd[params.start] == '"'))
		return (extract_quoted_value(ctx, &substr_len));
	return (extract_unquoted_value(ctx));
}
