/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_special_handler_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 20:04:46 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 20:05:07 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

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
	(*i) += 2;
	return (token);
}
