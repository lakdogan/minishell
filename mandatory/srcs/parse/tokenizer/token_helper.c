/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 06:48:21 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/08 23:40:38 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Prints syntax error for unexpected token.
void	token_error(char c)
{
	ft_putstr_fd("syntax error near unexpected token '", STDERR_FILENO);
	ft_putchar_fd(c, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

// Checks if token is valid based on state.
int	is_token_valid(char *value, t_token_state state)
{
	char	c;
	int		i;

	if (state == IN_DQUOTES || state == IN_SQUOTES)
		return (1);
	i = 0;
	c = value[i];
	if (value[i + 1])
	{
		if (value[i + 1] != c)
		{
			token_error(c);
			return (0);
		}
		if (value[i + 2])
		{
			token_error(c);
			return (0);
		}
	}
	return (1);
}

// Copies value inside quotes to cleaned string.
void	val_cpy(char *cleaned, char *value, int *i, int *j)
{
	char	quote;

	quote = value[(*i)];
	(*i)++;
	while (value[(*i)] && value[(*i)] != quote)
	{
		cleaned[(*j)] = value[(*i)];
		(*j)++;
		(*i)++;
	}
	if (value[(*i)] == quote)
		(*i)++;
}
