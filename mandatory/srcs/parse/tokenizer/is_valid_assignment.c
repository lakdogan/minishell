/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_assignment.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 23:12:49 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:37:15 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Checks if character is valid as identifier start.
static int	is_valid_identifier_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

// Checks if character is valid in identifier.
static int	is_valid_identifier_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

// Validates identifier part before '='.
static int	validate_identifier_part(const char *s, int *i)
{
	if (!s || !is_valid_identifier_start(s[0]))
		return (0);
	(*i)++;
	while (s[*i] && s[*i] != '=')
	{
		if (!is_valid_identifier_char(s[*i]))
			return (0);
		(*i)++;
	}
	return (1);
}

// Checks if string is a valid assignment.
int	is_valid_assignment(const char *s)
{
	int	i;

	i = 0;
	if (!validate_identifier_part(s, &i))
		return (0);
	if (s[i] != '=')
		return (0);
	return (1);
}
