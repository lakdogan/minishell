/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outfile_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:55:36 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 22:55:39 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

/* Create an outfile node with memory allocation */
static t_outfile	*create_outfile_node(t_gc *gc, char *filename)
{
	t_outfile	*node;

	node = gc_calloc(gc, 1, sizeof(t_outfile));
	if (!node)
		return (NULL);
	node->name = filename;
	node->next = NULL;
	return (node);
}

/* Set the type of output redirection */
static void	set_outfile_type(t_outfile *node, t_token_type type)
{
	if (type == APPEND)
		node->type = OUT_APPEND;
	else
		node->type = OUT_TRUNC;
}

/* Set the target file descriptor for redirection */
static void	set_outfile_target(t_outfile *node, t_token_type type)
{
	if (type == OUTFILE || type == APPEND)
		node->fd_target = STDOUT_FILENO;
	else if (type == ERR_OUTFILE)
		node->fd_target = STDERR_FILENO;
	else
		node->fd_target = STDOUT_FILENO;
}

/* Add the outfile node to the list */
static void	add_outfile_to_list(t_exec *exec, t_outfile *node)
{
	t_outfile	*current;

	if (exec->outfiles == NULL)
		exec->outfiles = node;
	else
	{
		current = exec->outfiles;
		while (current->next)
			current = current->next;
		current->next = node;
	}
}

/* Main function to add an outfile to the execution context */
int	add_outfile(t_exec *exec, t_token_type type, char *filename, t_gc *gc)
{
	t_outfile	*node;

	node = create_outfile_node(gc, filename);
	if (!node)
		return (0);
	set_outfile_type(node, type);
	set_outfile_target(node, type);
	add_outfile_to_list(exec, node);
	return (1);
}
