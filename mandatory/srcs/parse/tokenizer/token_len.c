/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_len.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 07:45:08 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/14 06:31:52 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

static int	get_quoted_len(const char *cmd, int i)
{
	char	quote;
	int		len;

	quote = cmd[i];
	len = 1;
	while (cmd[i + len] && cmd[i + len] != quote)
		len++;
	if (cmd[i + len] == quote)
		len++;
	return (len);
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

static int	get_token_len_loop(const char *cmd, int i)
{
	int	len;
	int	in_single;
	int	in_double;

	len = 0;
	in_single = 0;
	in_double = 0;
	while (cmd[i + len])
	{
		update_quote_state(cmd[i + len], &in_single, &in_double);
		if (is_token_boundary(cmd, i + len, in_single, in_double))
		{
			if (len == 0 && get_op_len(cmd, i + len) > 0)
				len += get_op_len(cmd, i + len);
			break ;
		}
		len++;
	}
	return (len);
}

int	tok_len(const char *cmd, int *i)
{
	int	redirection_len;

	if (cmd[*i] == '\'' || cmd[*i] == '"')
		return (get_quoted_len(cmd, *i));
	redirection_len = get_op_len(cmd, *i);
	if (redirection_len > 0)
		return (redirection_len);
	return (get_token_len_loop(cmd, *i));
}
