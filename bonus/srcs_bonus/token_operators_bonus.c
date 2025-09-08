/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operators_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 00:24:44 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:32:57 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

// Checks if a character is an operator.
int	is_operator_char(char c)
{
	return (c == '|' || c == '&' || c == '<' || c == '>' || c == ';' || c == '('
		|| c == ')');
}

// Checks if a string starts with an operator and returns its length.
int	is_operator(const char *str)
{
	if (!str || !*str)
		return (0);
	if (str[0] == '|' && str[1] == '|')
		return (2);
	if (str[0] == '&' && str[1] == '&')
		return (2);
	if (str[0] == '>' && str[1] == '>')
		return (2);
	if (str[0] == '<' && str[1] == '<')
		return (2);
	if (str[0] == '2' && str[1] == '>')
		return (2);
	if (is_operator_char(str[0]))
		return (1);
	return (0);
}

// Processes an operator token in the command string.
int	process_operator_token(const char *cmd, int *i)
{
	int	len;

	len = is_operator(cmd + *i);
	if (len > 0)
		*i += len;
	return (len);
}
