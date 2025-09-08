/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 06:29:26 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/08 23:48:10 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Parses either a subshell or a command.
t_command_tree	*parse_sub_or_cmd(t_tokens *tokens, int *pos,
		t_minishell *shell)
{
	if ((*pos) < tokens->count && tokens->arr[*pos].type == ASSIGNMENT)
		return (parse_command(tokens, pos, shell));
	if (tokens->arr[(*pos)].type == L_PAREN)
		return (parse_subshell(tokens, pos, shell));
	else
		return (parse_command(tokens, pos, shell));
}

// Handles redirections for a subshell node.
int	handle_sub_redir(t_command_tree *node, t_tokens *tokens, int *pos, t_gc *gc)
{
	t_exec		*redir;
	t_redir_ctx	ctx;

	if ((*pos) >= tokens->count || !is_redir(tokens->arr[(*pos)].type))
		return (1);
	redir = gc_calloc(gc, 1, sizeof(t_exec));
	if (!redir)
		return (0);
	ctx.exec = redir;
	ctx.tokens = tokens;
	ctx.pos = pos;
	ctx.gc = gc;
	while ((*pos) < tokens->count && is_redir(tokens->arr[(*pos)].type))
	{
		if (!handle_redir(ctx))
			return (0);
	}
	node->data = redir;
	return (1);
}

// Parses a subshell expression.
t_command_tree	*parse_subshell(t_tokens *tokens, int *pos, t_minishell *shell)
{
	t_command_tree	*sub;
	t_command_tree	*node;

	(*pos)++;
	sub = parse_logic_ops(tokens, pos, shell);
	if (!sub)
		return (NULL);
	if ((*pos) >= tokens->count || tokens->arr[(*pos)].type != R_PAREN)
	{
		error_msg_paren();
		return (NULL);
	}
	(*pos)++;
	node = node_init(NODE_SUBSHELL, shell->gc[GC_COMMAND]);
	if (!node)
		return (NULL);
	node->left = sub;
	if (!handle_sub_redir(node, tokens, pos, shell->gc[GC_COMMAND]))
		return (NULL);
	return (node);
}
