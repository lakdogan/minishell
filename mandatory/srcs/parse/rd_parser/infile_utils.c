/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:55:22 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/15 01:36:11 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

/* Create an infile node with memory allocation */
t_infile	*create_infile(t_token_type type, char *filename, t_gc *gc)
{
	t_infile	*node;

	node = gc_calloc(gc, 1, sizeof(t_infile));
	if (!node)
		return (NULL);
	node->name = filename;
	if (type == HEREDOC)
	{
		node->type = INF_HEREDOC;
		node->delimeter = filename;
	}
	else
		node->type = INF_IN;
	node->next = NULL;
	return (node);
}

/* Add an infile to the execution context */
int	add_infile(t_exec *exec, t_token_type type, char *filename, t_gc *gc)
{
	t_infile	*node;
	t_infile	*current;

	node = create_infile(type, filename, gc);
	if (!node)
		return (0);
	if (exec->infiles == NULL)
		exec->infiles = node;
	else
	{
		current = exec->infiles;
		while (current->next)
			current = current->next;
		current->next = node;
	}
	return (1);
}
