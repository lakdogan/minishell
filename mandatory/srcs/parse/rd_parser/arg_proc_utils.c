/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_proc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 07:03:23 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 07:04:18 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

void	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

// Helper: returns true if c is in charset
static bool	is_in_charset(char c, const char *charset)
{
	while (*charset)
	{
		if (c == *charset)
			return (true);
		charset++;
	}
	return (false);
}

// Counts the number of words
static int	count_words(const char *s, const char *charset)
{
	int		count;
	bool	in_word;

	count = 0;
	in_word = false;
	while (*s)
	{
		if (is_in_charset(*s, charset))
			in_word = false;
		else if (!in_word)
		{
			in_word = true;
			count++;
		}
		s++;
	}
	return (count);
}

// The main split function
char	**ft_split_charset(const char *s, const char *charset)
{
	int			words;
	char		**result;
	int			i;
	const char	*start = s;
	int			len;

	words = count_words(s, charset);
	result = malloc(sizeof(char *) * (words + 1));
	i = 0;
	while (*s)
	{
		while (*s && is_in_charset(*s, charset))
			s++;
		len = 0;
		while (*s && !is_in_charset(*s, charset))
		{
			s++;
			len++;
		}
		if (len > 0)
			result[i++] = word_dup(start, len);
	}
	result[i] = NULL;
	return (result);
}
