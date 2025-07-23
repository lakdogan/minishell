
#include "../../../includes/core/minishell.h"

t_command_tree	*create_new_node(t_node_type type, t_gc *gc)
{
	t_command_tree	*node;

	node = gc_calloc(gc, 1,sizeof(t_command_tree));
	if (node)
		node->type = type;
	return (node);
}

int	handle_redir(t_exec	*exec, t_tokens *tokens, int *pos, t_gc *gc)
{
	t_token_type	type;
	int				if_fail;

	type = tokens->arr[(*pos)].type;
	(*pos)++;
	if ((*pos) >= tokens->count || tokens->arr[(*pos)].type != WORD)
	{
		error_msg(tokens, pos);
		return (0);
	}
	if_fail = 0;
	if (type == APPEND || type == OUTFILE)
		if_fail = add_outfile(exec, type, tokens->arr[(*pos)].value, gc);
	else if (type == HEREDOC || type == INFILE)
		if_fail = add_infile(exec, type, tokens->arr[(*pos)].value, gc);
	if (if_fail == 0)
		return (0);
	(*pos)++;
	return (1);
}

t_arg_lst	*get_args(t_exec *exec, t_tokens *tokens, int *pos, t_gc *gc)
{
	t_arg_lst	*args;

	args = NULL;
	while ((*pos) < tokens->count && (tokens->arr[(*pos)].type == WORD ||
			is_redir(tokens->arr[(*pos)].type)))
	{
		if (is_redir(tokens->arr[(*pos)].type))
		{
			if (!handle_redir(exec, tokens, pos, gc))
			{
				(*pos) = -1;
				return (NULL);
			}
		}
		else
		{
			arg_to_list(&args, tokens->arr[(*pos)].value, gc);
			(*pos)++;
		}
	}
	return (args);
}

t_command_tree	*parse_cmd(t_tokens *tokens, int *pos, t_gc *gc)
{
	t_exec			*exec;
	t_arg_lst		*args;
	t_command_tree	*node;
	int				s_pos;

	exec = gc_calloc(gc, 1, sizeof(t_exec));
	if (!exec)
		return (NULL);
	s_pos = (*pos);
	args = get_args(exec, tokens, pos, gc);
	if ((*pos) == -1 || s_pos == (*pos))
		return (NULL);
	exec->argv = lst_to_argv(args, gc);
	if (!exec->argv && args)
		return (NULL);
	if (exec->argv)
		exec->command = exec->argv[0];
	node = node_init(N_EXEC, gc);
	if (!node)
		return (NULL);
	node->data = exec;
	return (node);
}


