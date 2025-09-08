/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_len.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 07:45:08 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/08 23:41:18 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Returns length of word token.
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

// Returns length of next token.
int	tok_len(const char *cmd, int *i)
{
	int	len;
	int	in_single;
	int	redirection_len;
	int	in_double;

	in_single = 0;
	in_double = 0;
	len = 0;
	redirection_len = handle_redirection_out(cmd, i);
	if (redirection_len > 0)
		return (redirection_len);
	while (cmd[(*i) + len])
	{
		update_quote_state(cmd[(*i) + len], &in_single, &in_double);
		if (is_token_boundary(cmd, (*i) + len, in_single, in_double))
		{
			if (len == 0 && get_op_len(cmd, (*i) + len) > 0)
				len += get_op_len(cmd, (*i) + len);
			break ;
		}
		len++;
	}
	return (len);
}
