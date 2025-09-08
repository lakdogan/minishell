/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 23:01:37 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:41:39 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Returns length of operator at position i.
int	get_op_len(const char *cmd, int i)
{
	if (cmd[i] == '>' || cmd[i] == '<')
	{
		if (cmd[i] == cmd[i + 1])
			return (2);
		return (1);
	}
	if (cmd[i] == '|' || cmd[i] == '&' || cmd[i] == '(' || cmd[i] == ')'
		|| cmd[i] == ';')
		return (1);
	return (0);
}

// Checks if string starts with an operator.
int	is_operator(const char *s)
{
	if (s[0] == '|' || s[0] == '<' || s[0] == '>' || s[0] == '&' || s[0] == ';'
		|| s[0] == '(' || s[0] == ')')
		return (1);
	return (0);
}

// Handles output redirection operators.
int	handle_redirection_out(const char *cmd, int *i)
{
	if (cmd[*i] == '>')
	{
		if (cmd[*i + 1] == '>')
			return ((*i)++, 2);
		else if (cmd[*i + 1] == '|')
			return ((*i)++, 2);
		return (1);
	}
	return (0);
}
