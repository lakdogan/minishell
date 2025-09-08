/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_boundaries.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 23:04:18 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/07 22:59:57 by almatsch         ###   ########.fr       */
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

bool	is_token_boundary(const char *cmd, int pos, int in_single,
		int in_double)
{
	if (in_single || in_double)
		return (false);
	if (get_op_len(cmd, pos) > 0)
		return (true);
	if (ft_isspace(cmd[pos]))
		return (true);
	return (false);
}
