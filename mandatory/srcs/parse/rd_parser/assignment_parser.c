/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignment_parser.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:50:02 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 22:53:16 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

/* Count the number of consecutive assignment tokens */
static int	count_assignments(t_tokens *tokens, int *pos, int *start)
{
	int	count;

	*start = *pos;
	count = 0;
	while (*pos < tokens->count && tokens->arr[*pos].type == ASSIGNMENT)
	{
		count++;
		(*pos)++;
	}
	return (count);
}

/* Allocate memory for the assignment array */
static char	**allocate_assignments(t_gc *gc, int count)
{
	char	**assignments;

	assignments = gc_calloc(gc, count + 1, sizeof(char *));
	return (assignments);
}

/* Copy assignment values to the allocated array */
static int	copy_assignment_values(t_tokens *tokens, int start,
		char **assignments, t_minishell *shell)
{
	int	i;

	i = 0;
	while (assignments[i] != NULL)
	{
		assignments[i] = gc_strdup(shell->gc[GC_COMMAND], tokens->arr[start
				+ i].value);
		if (!assignments[i])
			return (0);
		i++;
	}
	assignments[i] = NULL;
	return (1);
}

/* Process assignment statements at the current position */
int	process_assignments(t_tokens *tokens, int *pos, t_exec *exec,
		t_minishell *shell)
{
	int	start;
	int	count;

	count = count_assignments(tokens, pos, &start);
	if (count > 0)
	{
		exec->assignments = allocate_assignments(shell->gc[GC_COMMAND], count);
		if (!exec->assignments)
			return (0);
		if (!copy_assignment_values(tokens, start, exec->assignments, shell))
			return (0);
	}
	return (1);
}
