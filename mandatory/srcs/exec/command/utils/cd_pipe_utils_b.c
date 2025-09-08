/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pipe_utils_b.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:32:18 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 21:35:10 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Handle command after pipe character */
static void	handle_after_pipe_cmd(t_cmd_ctx *ctx)
{
	if (is_command_token(&ctx->input[ctx->i], ctx->cmds, ctx->cmd_count))
	{
		ctx->last_was_cmd = true;
		ctx->after_pipe = false;
	}
}

/* Handle command not after pipe */
static void	handle_normal_cmd(t_cmd_ctx *ctx)
{
	if (is_command_token(&ctx->input[ctx->i], ctx->cmds, ctx->cmd_count))
	{
		if (ctx->last_was_cmd)
		{
			ctx->result[ctx->ri++] = ' ';
			ctx->result[ctx->ri++] = '|';
			ctx->result[ctx->ri++] = ' ';
		}
		ctx->last_was_cmd = true;
	}
	else if (ctx->input[ctx->i] != ' ' && ctx->input[ctx->i] != '\t')
	{
		ctx->last_was_cmd = false;
	}
}

/* Process a character based on current context */
static void	process_command_char(t_cmd_ctx *ctx)
{
	if (ctx->quote == 0)
	{
		if (ctx->input[ctx->i] == '|')
		{
			handle_pipe_character(ctx);
			return ;
		}
		if (ctx->after_pipe)
			handle_after_pipe_cmd(ctx);
		else
			handle_normal_cmd(ctx);
	}
	else
	{
		ctx->last_was_cmd = false;
		ctx->after_pipe = false;
	}
	ctx->result[ctx->ri++] = ctx->input[ctx->i];
}

/* Main function to inject pipes between commands */
char	*inject_pipes_between_commands(const char *input, const char **cmds,
		int cmd_count)
{
	t_cmd_ctx	ctx;
	size_t		len;

	len = strlen(input);
	ctx.result = malloc(len * 2 + 1);
	if (!ctx.result)
		return (NULL);
	ctx.input = input;
	ctx.cmds = cmds;
	ctx.cmd_count = cmd_count;
	ctx.ri = 0;
	ctx.i = 0;
	ctx.last_was_cmd = false;
	ctx.after_pipe = false;
	ctx.quote = 0;
	while (ctx.i < len)
	{
		ctx.quote = handle_cmd_quote_state(ctx.quote, input[ctx.i]);
		process_command_char(&ctx);
		ctx.i++;
	}
	ctx.result[ctx.ri] = '\0';
	return (ctx.result);
}
