/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utilities_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 00:47:00 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:34:52 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

// Returns the length of the operator at the given position.
int	get_op_len(const char *cmd, int pos)
{
	if (!cmd || !cmd[pos])
		return (0);
	if (cmd[pos] == '|' && cmd[pos + 1] == '|')
		return (2);
	if (cmd[pos] == '&' && cmd[pos + 1] == '&')
		return (2);
	if (cmd[pos] == '>' && cmd[pos + 1] == '>')
		return (2);
	if (cmd[pos] == '<' && cmd[pos + 1] == '<')
		return (2);
	if (cmd[pos] == '2' && cmd[pos + 1] == '>')
		return (2);
	if (cmd[pos] == '|' || cmd[pos] == '<' || cmd[pos] == '>' || cmd[pos] == ';'
		|| cmd[pos] == '(' || cmd[pos] == ')')
		return (1);
	return (0);
}

// Handles output redirection token (stub).
int	handle_redirection_out(const char *cmd, int *i)
{
	(void)cmd;
	(void)i;
	return (0);
}
