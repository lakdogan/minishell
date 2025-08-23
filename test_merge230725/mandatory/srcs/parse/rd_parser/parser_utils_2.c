/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 06:21:59 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/11 22:03:24 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

void	check_expand(t_exec *exec, t_arg_lst *args, t_gc *gc)
{
	t_arg_lst	*current;
	int			arg_count;
	int			i;

	if (!exec->argv)
		return ;
	arg_count = 0;
	current = args;
	while (current)
	{
		arg_count++;
		current = current->next;
	}
	exec->no_expand_flags = gc_calloc(gc, arg_count + 1, sizeof(bool));
	if (!exec->no_expand_flags)
		return ;
	i = 0;
	current = args;
	while (current)
	{
		exec->no_expand_flags[i] = current->no_expand;
		i++;
		current = current->next;
	}
}

t_command_tree	*create_exec_node(t_exec *exec, t_arg_lst *args, t_gc *gc)
{
	t_command_tree	*node;

	exec->argv = lst_to_argv(args, gc);
	if (!exec->argv && args)
		return (NULL);
	if (exec->argv)
		exec->command = exec->argv[0];
	check_expand(exec, args, gc);
	node = node_init(NODE_CMD, gc);
	if (!node)
		return (NULL);
	node->data = exec;
	return (node);
}

t_infile	*create_infile(t_token_type type, char *filename, t_gc *gc)
{
	t_infile	*node;

	node = gc_calloc(gc, 1, sizeof(t_infile));
	if (!node)
		return (0);
	if (type == HEREDOC)
	{
		node->type = INF_HEREDOC;
		node->delimeter = filename;
		node->name = NULL;
	}
	else
	{
		node->type = INF_IN;
		node->name = filename;
	}
	return (node);
}
