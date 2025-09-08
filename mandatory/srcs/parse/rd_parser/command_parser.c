/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:55:14 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:46:42 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Creates a command node with the execution context.
static t_command_tree	*create_command_node(t_exec *exec, t_minishell *shell)
{
	t_command_tree	*node;

	node = gc_calloc(shell->gc[GC_COMMAND], 1, sizeof(t_command_tree));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->data = exec;
	return (node);
}

// Creates a new AST node of given type.
t_command_tree	*create_new_node(t_node_type type, t_gc *gc)
{
	t_command_tree	*node;

	node = gc_calloc(gc, 1, sizeof(t_command_tree));
	if (node)
		node->type = type;
	return (node);
}

// Parses a command and returns command node.
t_command_tree	*parse_command(t_tokens *tokens, int *pos, t_minishell *shell)
{
	t_command_tree	*node;
	t_exec			*exec;
	int				had_redir;

	had_redir = 0;
	exec = gc_calloc(shell->gc[GC_COMMAND], 1, sizeof(t_exec));
	if (!exec)
		return (NULL);
	if (!process_assignments(tokens, pos, exec, shell))
		return (NULL);
	if (!process_command_tokens(tokens, pos, exec, shell))
		return (NULL);
	if (!exec->argv && !had_redir && !exec->assignments)
		return (NULL);
	node = create_command_node(exec, shell);
	return (node);
}
