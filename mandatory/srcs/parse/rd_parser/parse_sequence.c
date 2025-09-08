/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:55:48 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:47:41 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Parses command sequences separated by semicolons.
t_command_tree	*parse_sequence(t_tokens *tokens, int *pos, t_minishell *shell)
{
	t_command_tree	*left;
	t_command_tree	*right;
	t_command_tree	*node;

	left = parse_pipeline(tokens, pos, shell);
	if (!left)
		return (NULL);
	while (*pos < tokens->count && tokens->arr[*pos].type == SEMICOLON)
	{
		(*pos)++;
		right = parse_pipeline(tokens, pos, shell);
		if (!right)
			return (NULL);
		node = get_node(NODE_SEQUENCE, left, right, shell->gc[GC_COMMAND]);
		left = node;
	}
	return (left);
}
