/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_processor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:54:55 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 22:54:58 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

/* Handle no-expand tokens */
static char	*handle_no_expand(t_word_ctx ctx)
{
	return (gc_strdup(ctx.shell->gc[GC_COMMAND],
			ctx.tokens->arr[*(ctx.pos)].value));
}

/* Handle tilde expansion */
static int	is_tilde_expand(t_word_ctx ctx)
{
	char	*value;

	value = ctx.tokens->arr[*(ctx.pos)].value;
	return (value[0] == '~' && (value[1] == '\0' || value[1] == '/'));
}

/* Process expansion based on token properties */
static char	*process_expansion(t_word_ctx ctx)
{
	if (ctx.tokens->arr[*(ctx.pos)].no_expand)
		return (handle_no_expand(ctx));
	else if (is_tilde_expand(ctx))
		return (expand_tilde(ctx.tokens->arr[*(ctx.pos)].value, ctx.shell));
	else
		return (expand_variables(ctx.tokens->arr[*(ctx.pos)].value, ctx.shell));
}

/* Setup argument flags for the token */
static t_arg_flags	setup_arg_flags(t_word_ctx ctx)
{
	t_arg_flags	flags;

	flags.no_expand = ctx.tokens->arr[*(ctx.pos)].no_expand;
	flags.was_quoted = ctx.tokens->arr[*(ctx.pos)].was_quoted;
	return (flags);
}

/* Process a word token into the arguments list */
int	process_word(t_tokens *tokens, int *pos, t_arg_lst **args,
		t_minishell *shell)
{
	char		*expanded;
	t_arg_flags	flags;
	t_word_ctx	ctx;

	ctx.tokens = tokens;
	ctx.pos = pos;
	ctx.args = args;
	ctx.shell = shell;
	expanded = process_expansion(ctx);
	flags = setup_arg_flags(ctx);
	arg_to_list(args, expanded, flags, shell);
	(*pos)++;
	return (1);
}
