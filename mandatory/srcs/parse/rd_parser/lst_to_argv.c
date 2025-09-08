/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_to_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:00:02 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 22:53:39 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

/* Count the number of elements in the argument list */
static int	count_arg_elements(t_arg_lst *head)
{
	t_arg_lst	*current;
	int			count;

	current = head;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/* Allocate arrays for arguments and flags */
static char	**allocate_arg_arrays(t_gc *gc, int count, bool **no_expand_flags,
		bool **was_quoted_flags)
{
	char	**argv;

	argv = gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	*no_expand_flags = gc_malloc(gc, sizeof(bool) * (count + 1));
	if (!*no_expand_flags)
		return (NULL);
	*was_quoted_flags = gc_malloc(gc, sizeof(bool) * (count + 1));
	if (!*was_quoted_flags)
		return (NULL);
	return (argv);
}

/* Fill arrays with data from the argument list */
static void	fill_arg_arrays(t_arg_lst *head, char **argv, bool *no_expand,
		bool *was_quoted)
{
	t_arg_lst	*current;
	int			i;

	current = head;
	i = 0;
	while (current)
	{
		argv[i] = current->arg;
		no_expand[i] = current->no_expand;
		was_quoted[i] = current->was_quoted;
		current = current->next;
		i++;
	}
	argv[i] = NULL;
	no_expand[i] = false;
	was_quoted[i] = false;
}

/* Convert argument list to argv array with flags */
char	**lst_to_argv(t_arg_lst *head, t_gc *gc, bool **no_expand_flags,
		bool **was_quoted_flags)
{
	char	**argv;
	int		count;

	if (!head)
	{
		argv = gc_malloc(gc, sizeof(char *));
		*no_expand_flags = gc_malloc(gc, sizeof(bool));
		*was_quoted_flags = gc_malloc(gc, sizeof(bool));
		if (!argv || !*no_expand_flags || !*was_quoted_flags)
			return (NULL);
		argv[0] = NULL;
		(*no_expand_flags)[0] = false;
		(*was_quoted_flags)[0] = false;
		return (argv);
	}
	count = count_arg_elements(head);
	argv = allocate_arg_arrays(gc, count, no_expand_flags, was_quoted_flags);
	if (!argv)
		return (NULL);
	fill_arg_arrays(head, argv, *no_expand_flags, *was_quoted_flags);
	return (argv);
}
