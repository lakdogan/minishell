/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_len.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 07:45:08 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/04 07:45:53 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

int	check_for_paren(const char *cmd, int *i, int len)
{
	if (cmd[(*i) + len] == '(' || cmd[(*i) + len] == ')')
		return (1);
	else
		return (0);
}

int	check_for_qoutes(const char *cmd, int *i, int *len)
{
	char	quote;

	quote = cmd[(*i) + (*len)];
	(*len)++;
	while (cmd[(*i) + (*len)] && cmd[(*i) + (*len)] != quote)
		(*len)++;
	if (cmd[(*i) + (*len)] == quote)
	{
		(*len)++;
		return (1);
	}
	return (0);
}

int	get_op_len(const char *cmd, int i)
{
	if (cmd[i] == '>' || cmd[i] == '<')
	{
		if (cmd[i] == cmd[i + 1])
			return (2);
		return (1);
	}
	if (cmd[i] == '|' || cmd[i] == '&' || cmd[i] == '(' || cmd[i] == ')')
		return (1);
	return (0);
}

int	get_word_len(const char *cmd, int *i)
{
	int	len;

	len = 0;
	while (cmd[(*i) + len])
	{
		if (cmd[(*i) + len] == '"' || cmd[(*i) + len] == '\'')
		{
			if (!check_for_qoutes(cmd, i, &len))
				break ;
			continue ;
		}
		if (ft_isspace(cmd[(*i) + len]) || get_op_len(cmd, (*i) + len) > 0)
			break ;
		len++;
	}
	return (len);
}

int	tok_len(const char *cmd, int *i)
{
	int	len;

	len = get_op_len(cmd, (*i));
	if (len > 0)
		return (len);
	return (get_word_len(cmd, i));
}
