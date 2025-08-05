/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 06:28:38 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/04 06:29:16 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

t_command_tree	*get_node(t_node_type type, t_command_tree *left,
				t_command_tree *right, t_gc *gc)
{
	t_command_tree	*new_node;

	new_node = gc_malloc(gc, sizeof(t_command_tree));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->left = left;
	new_node->right = right;
	new_node->data = NULL;
	return (new_node);
}

t_command_tree	*parse_pipeline(t_tokens *tokens, int *pos, t_gc *gc)
{
	t_command_tree	*node;
	t_command_tree	*right;
	t_command_tree	*left;

	left = parse_sub_or_cmd(tokens, pos, gc);
	if (!left)
		return (NULL);
	while ((*pos) < tokens->count && tokens->arr[(*pos)].type == PIPE)
	{
		(*pos)++;
		right = parse_sub_or_cmd(tokens, pos, gc);
		if (!right)
			return (NULL);
		node = get_node(N_PIPE, left, right, gc);
		left = node;
	}
	return (left);
}
