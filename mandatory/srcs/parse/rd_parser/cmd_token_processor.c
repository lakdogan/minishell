/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_token_processor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:25:02 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/06 15:57:53 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

/* Check if current token is of a valid type for command processing */
static int	is_valid_cmd_token(t_token_ctx ctx)
{
	t_token_type	type;

	if (*(ctx.pos) >= ctx.tokens->count)
		return (0);
	type = ctx.tokens->arr[*(ctx.pos)].type;
	return (type == WORD || type == ASSIGNMENT || is_redir(type));
}

/* Process a single token in the command context */
static int	process_single_token(t_token_ctx ctx, t_arg_lst **args)
{
	if (is_redir(ctx.tokens->arr[*(ctx.pos)].type))
	{
		if (!handle_redirection(ctx.tokens, ctx.pos, ctx.exec, ctx.shell))
			return (0);
	}
	else
	{
		if (!process_word(ctx.tokens, ctx.pos, args, ctx.shell))
			return (0);
	}
	return (1);
}

/* Convert argument list to argv array and set command */
static int	finalize_command(t_token_ctx ctx, t_arg_lst *args)
{
	ctx.exec->argv = lst_to_argv(args, ctx.shell->gc[GC_COMMAND],
			&ctx.exec->no_expand_flags, &ctx.exec->was_quoted_flags);
	if (ctx.exec->argv)
		ctx.exec->command = ctx.exec->argv[0];
	return (1);
}

/* Process all tokens for the command */
int	process_command_tokens(t_tokens *tokens, int *pos, t_exec *exec,
		t_minishell *shell)
{
	t_arg_lst	*args;
	t_token_ctx	ctx;

	args = NULL;
	ctx.tokens = tokens;
	ctx.pos = pos;
	ctx.exec = exec;
	ctx.shell = shell;
	while (is_valid_cmd_token(ctx))
	{
		if (!process_single_token(ctx, &args))
			return (0);
	}
	return (finalize_command(ctx, args));
}
