/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pipe_utils_a.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:32:27 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 21:35:06 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Skip past any leading whitespace characters in a string */
static const char	*skip_leading_whitespace(const char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

/* Check if a string matches any command in the command list */
static bool	is_command_match(const char *str, const char **cmds, int cmd_count)
{
	int		i;
	size_t	len;

	i = 0;
	while (i < cmd_count)
	{
		len = strlen(cmds[i]);
		if (strncmp(str, cmds[i], len) == 0 && (str[len] == ' '
				|| str[len] == '\0' || str[len] == '\t'))
			return (true);
		i++;
	}
	return (false);
}

/* Check if a string starts with a valid command token */
bool	is_command_token(const char *str, const char **cmds, int cmd_count)
{
	str = skip_leading_whitespace(str);
	return (is_command_match(str, cmds, cmd_count));
}

/* Track quote state for command parsing - renamed to avoid conflict */
char	handle_cmd_quote_state(char current_quote, char input_char)
{
	if (input_char == '\'' || input_char == '"')
	{
		if (current_quote == 0)
			return (input_char);
		else if (current_quote == input_char)
			return (0);
	}
	return (current_quote);
}

/* Process pipe character and update state */
void	handle_pipe_character(t_cmd_ctx *ctx)
{
	ctx->after_pipe = true;
	ctx->last_was_cmd = false;
	ctx->result[ctx->ri++] = ctx->input[ctx->i++];
	while (ctx->input[ctx->i] == ' ' || ctx->input[ctx->i] == '\t')
		ctx->result[ctx->ri++] = ctx->input[ctx->i++];
	ctx->i--;
}
