/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_logic_ops.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 06:28:09 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/11 22:01:01 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

t_command_tree	*start_parser(t_tokens *tokens, t_gc *gc, t_minishell *shell)
{
	t_command_tree	*ast;
	int				pos;

	(void)shell;
	if (!tokens || tokens->count == 0)
		return (NULL);
	pos = 0;
	ast = parse_logic_ops(tokens, &pos, gc);
	if (!ast)
	{
		ft_putstr_fd("Invalid syntax\n", STDERR_FILENO);
		return (NULL);
	}
	if (pos < tokens->count)
	{
		shell->exit_code = 2;
		error_msg(tokens, &pos);
		return (NULL);
	}
	return (ast);
}

int	is_logical_op(t_token_type type)
{
	if (type == AND_IF || type == OR)
		return (1);
	return (0);
}

t_command_tree	*parse_logic_ops(t_tokens *tokens, int *pos, t_gc *gc)
{
	t_command_tree	*left;
	t_command_tree	*right;
	t_token_type	type;

	left = parse_pipeline(tokens, pos, gc);
	if (!left)
		return (NULL);
	while ((*pos) < tokens->count && is_logical_op(tokens->arr[(*pos)].type))
	{
		type = tokens->arr[(*pos)].type;
		(*pos)++;
		right = parse_pipeline(tokens, pos, gc);
		if (!right)
			return (NULL);
		if (type == AND_IF)
			left = get_node(NODE_ANDIF, left, right, gc);
		else
			left = get_node(NODE_OR, left, right, gc);
	}
	return (left);
}
