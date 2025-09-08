/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:21:06 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:02:48 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Checks if character is valid for a variable name.
bool	is_var_char(char c)
{
	return (ft_isalnum(c) || c == UNDERSCORE);
}

// Gets the length of a variable name in a string.
int	get_var_name_len(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && is_var_char(str[i]))
		i++;
	return (i);
}

// Extracts a variable name from a string.
char	*extract_var_name(const char *str, t_minishell *shell)
{
	int	name_len;

	name_len = get_var_name_len(str);
	return (gc_substr(shell->gc[GC_EXPAND], str, FIRST_CHAR, name_len));
}
