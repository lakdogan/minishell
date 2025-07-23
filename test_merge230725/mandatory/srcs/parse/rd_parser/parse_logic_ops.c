#include "../../../includes/core/minishell.h"

t_command_tree	*start_parser(t_tokens *tokens, t_gc *gc)
{
	t_command_tree	*ast;
	int				pos;

	if (!tokens || tokens->count == 0)
		return (NULL);
	pos = 0;
	ast = parse_logic_ops(tokens, &pos, gc);
	if (!ast)
		return (NULL);
	if (pos < tokens->count)
	{
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
	while((*pos) < tokens->count && is_logical_op(tokens->arr[(*pos)].type))
	{
		type = tokens->arr[(*pos)].type;
		(*pos)++;
		right = parse_pipeline(tokens, pos, gc);
		if (!right)
			return (NULL);
		if (type == AND_IF)
			left = get_node(N_ANDIF, left, right, gc);
		else
			left = get_node(N_OR, left, right, gc);
	}
	return (left);
}
