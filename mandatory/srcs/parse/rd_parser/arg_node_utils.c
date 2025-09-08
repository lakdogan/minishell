/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_node_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:35:02 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 22:52:56 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

/* Creates a new argument node with the given properties */
t_arg_lst	*create_arg_node(t_minishell *shell, char *value, bool no_expand,
		bool was_quoted)
{
	t_arg_lst	*new_node;

	new_node = gc_malloc(shell->gc[GC_MAIN], sizeof(t_arg_lst));
	if (!new_node)
		return (NULL);
	new_node->arg = value;
	new_node->no_expand = no_expand;
	new_node->was_quoted = was_quoted;
	new_node->next = NULL;
	return (new_node);
}

/* Adds a node to the end of the argument list */
void	add_node_to_list(t_arg_lst **head, t_arg_lst *new_node)
{
	t_arg_lst	*current;

	if (!new_node)
		return ;
	if ((*head) == NULL)
		(*head) = new_node;
	else
	{
		current = (*head);
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}
