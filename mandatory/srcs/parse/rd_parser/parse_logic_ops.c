/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_logic_ops.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 06:28:09 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/14 06:42:24 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Starts parsing and builds AST from tokens.
t_command_tree	*start_parser(t_tokens *tokens, t_minishell *shell)
{
	t_command_tree	*ast;
	int				pos;

	if (!tokens || tokens->count == 0)
		return (NULL);
	merge_adjacent_tokens(tokens, shell);
	pos = 0;
	ast = parse_logic_ops(tokens, &pos, shell);
	if (!ast)
	{
		shell->exit_code = 2;
		error_msg(tokens, &pos, shell);
		return (NULL);
	}
	if (pos < tokens->count)
	{
		shell->exit_code = 258;
		error_msg(tokens, &pos, shell);
		return (NULL);
	}
	return (ast);
}

// Checks if token type is a logical operator.
int	is_logical_op(t_token_type type)
{
	if (type == AND_IF || type == OR)
		return (1);
	return (0);
}

// Parses logical operators (&&, ||) and builds AST nodes.
t_command_tree	*parse_logic_ops(t_tokens *tokens, int *pos, t_minishell *shell)
{
	t_command_tree	*left;
	t_command_tree	*right;
	t_token_type	type;

	left = parse_sequence(tokens, pos, shell);
	if (!left)
		return (NULL);
	while ((*pos) < tokens->count && is_logical_op(tokens->arr[(*pos)].type))
	{
		type = tokens->arr[(*pos)].type;
		(*pos)++;
		right = parse_sequence(tokens, pos, shell);
		if (!right)
			return (NULL);
		if (type == AND_IF)
			left = get_node(NODE_ANDIF, left, right, shell->gc[GC_COMMAND]);
		else
			left = get_node(NODE_OR, left, right, shell->gc[GC_COMMAND]);
	}
	return (left);
}
