/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 06:31:24 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/08 23:48:54 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Initializes execution context structure.
t_exec	*exec_init(t_gc *gc)
{
	t_exec	*exec;

	exec = gc_malloc(gc, sizeof(t_exec));
	exec->argv = NULL;
	exec->command = NULL;
	exec->infiles = NULL;
	exec->outfiles = NULL;
	exec->heredoc_fd = -1;
	exec->heredoc_prepared = false;
	return (exec);
}

// Initializes a new AST node with given type.
t_command_tree	*node_init(t_node_type type, t_gc *gc)
{
	t_command_tree	*node;

	node = gc_calloc(gc, 1, sizeof(t_command_tree));
	if (!node)
		return (NULL);
	node->type = type;
	return (node);
}
