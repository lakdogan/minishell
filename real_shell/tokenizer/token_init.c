/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 07:48:08 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/07 22:57:30 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

t_token	init_token_default(void)
{
	t_token	new_token;

	new_token.value = NULL;
	new_token.type = END;
	new_token.pos = -1;
	new_token.state = GENERAL;
	return (new_token);
}

t_token	init_token(const char *cmd, int *i, int t_count, t_minishell *shell)
{
	t_token					token;
	t_bool					success;
	t_special_token_ctx		spec_ctx;
	t_standard_token_ctx	std_ctx;

	token = init_token_default();
	spec_ctx.cmd = cmd;
	spec_ctx.i = i;
	spec_ctx.t_count = t_count;
	spec_ctx.token = &token;
	if (try_handle_special_tokens(spec_ctx, shell))
		return (token);
	std_ctx.cmd = cmd;
	std_ctx.i = i;
	std_ctx.t_count = t_count;
	std_ctx.token = &token;
	success = process_standard_token(std_ctx, shell);
	if (!success)
		return (init_token_default());
	apply_token_expansion(&token, shell);
	return (token);
}
