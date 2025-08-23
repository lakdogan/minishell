/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 06:29:26 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/23 15:08:29 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

t_command_tree	*parse_sub_or_cmd(t_tokens *tokens, int *pos, t_gc *gc)
{
	// Handle assignment tokens first
	if ((*pos) < tokens->count && tokens->arr[*pos].type == ASSIGNMENT)
		return (parse_command(tokens, pos, gc));
	if (tokens->arr[(*pos)].type == L_PAREN)
		return (parse_subshell(tokens, pos, gc));
	else
		return (parse_cmd(tokens, pos, gc));
}

int	handle_sub_redir(t_command_tree *node, t_tokens *tokens, int *pos,
	t_gc *gc)
{
	t_exec	*redir;

	if ((*pos) >= tokens->count || !is_redir(tokens->arr[(*pos)].type))
		return (1);
	redir = gc_calloc(gc, 1, sizeof(t_exec));
	if (!redir)
		return (0);
	while ((*pos) < tokens->count && is_redir(tokens->arr[(*pos)].type))
	{
		if (!handle_redir(redir, tokens, pos, gc))
			return (0);
	}
	node->data = redir;
	return (1);
}

t_command_tree	*parse_subshell(t_tokens *tokens, int *pos, t_gc *gc)
{
	t_command_tree	*sub;
	t_command_tree	*node;

	(*pos)++;
	sub = parse_logic_ops(tokens, pos, gc);
	if (!sub)
		return (NULL);
	if ((*pos) >= tokens->count || tokens->arr[(*pos)].type != R_PAREN)
	{
		error_msg_paren();
		return (NULL);
	}
	(*pos)++;
	node = node_init(NODE_SUBSHELL, gc);
	if (!node)
		return (NULL);
	node->left = sub;
	if (!handle_sub_redir(node, tokens, pos, gc))
		return (NULL);
	return (node);
}
