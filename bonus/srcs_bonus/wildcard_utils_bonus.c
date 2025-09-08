/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 20:55:30 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:36:38 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

// Checks if the string contains an unquoted star.
bool	has_unquoted_star(const char *s)
{
	while (*s)
	{
		if (*s == '*')
			return (true);
		s++;
	}
	return (false);
}

// Recursively matches a pattern with '*' wildcards.
bool	match_star_recursive(const char *p, const char *s)
{
	while (*p && *s)
	{
		if (*p == '*')
		{
			p++;
			if (!*p)
				return (true);
			while (*s)
			{
				if (match_star_recursive(p, s))
					return (true);
				s++;
			}
			return (false);
		}
		if (*p != *s)
			return (false);
		p++;
		s++;
	}
	while (*p == '*')
		p++;
	return (!*p && !*s);
}

// Matches a pattern with '*' wildcards.
bool	match_star(const char *pattern, const char *str)
{
	return (match_star_recursive(pattern, str));
}

// Adds the pattern to result if no match was found.
void	add_pattern_if_no_match(char **result, int *j, int found, char *pattern)
{
	if (!found)
	{
		result[*j] = strdup(pattern);
		(*j)++;
	}
}
