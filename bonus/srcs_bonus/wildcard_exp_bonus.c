/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_exp_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:09:45 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 00:11:12 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

// Counts the number of matches in the current directory for a pattern.
static int	count_matches_in_dir(const char *pattern)
{
	DIR				*dir;
	struct dirent	*ent;
	int				count;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	ent = readdir(dir);
	while (ent)
	{
		if (ent->d_name[0] != '.' && match_star(pattern, ent->d_name))
			count++;
		ent = readdir(dir);
	}
	closedir(dir);
	return (count);
}

// Counts the total number of expanded arguments.
static int	count_expanded_args(char **argv, bool *no_expand_flags)
{
	int	i;
	int	count;
	int	matches;

	i = 0;
	count = 0;
	while (argv[i])
	{
		if (no_expand_flags && no_expand_flags[i] == 0
			&& has_unquoted_star(argv[i]))
		{
			matches = count_matches_in_dir(argv[i]);
			if (matches == 0)
				count += 1;
			else
				count += matches;
		}
		else
			count++;
		i++;
	}
	return (count);
}

// Expands a single argument with wildcards.
static void	expand_single_arg(char **result, int *j, char *pattern,
		t_minishell *shell)
{
	DIR				*dir;
	struct dirent	*ent;
	int				found;

	found = 0;
	dir = opendir(".");
	if (dir)
	{
		ent = readdir(dir);
		while (ent)
		{
			if (ent->d_name[0] != '.' && match_star(pattern, ent->d_name))
			{
				result[*j] = gc_strdup(shell->gc[GC_MAIN], ent->d_name);
				(*j)++;
				found = 1;
			}
			ent = readdir(dir);
		}
		closedir(dir);
	}
	if (!found)
		add_pattern_if_no_match(result, j, pattern, shell);
}

// Copies or expands arguments into the result array.
static void	copy_or_expand_args(char **argv, bool *no_expand_flags,
		char **result, t_minishell *shell)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (argv[i])
	{
		if (no_expand_flags && no_expand_flags[i] == 0
			&& has_unquoted_star(argv[i]))
			expand_single_arg(result, &j, argv[i], shell);
		else
		{
			result[j] = gc_strdup(shell->gc[GC_MAIN], argv[i]);
			j++;
		}
		i++;
	}
	result[j] = NULL;
}

// Expands wildcards in the argument array.
char	**expand_wildcards(char **argv, bool *no_expand_flags,
		t_minishell *shell)
{
	int		count;
	char	**result;

	count = count_expanded_args(argv, no_expand_flags);
	result = gc_malloc(shell->gc[GC_MAIN], sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	copy_or_expand_args(argv, no_expand_flags, result, shell);
	return (result);
}
