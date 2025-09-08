/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 06:32:02 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/08 23:49:21 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Checks if token type is a redirection.
int	is_redir(t_token_type type)
{
	if (type == APPEND || type == OUTFILE || type == INFILE || type == HEREDOC
		|| type == T_2GREATER)
		return (1);
	else
		return (0);
}

// Sets no_expand flags for exec from arg list.
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

// Creates a command node from exec and arg list.
t_command_tree	*create_exec_node(t_exec *exec, t_arg_lst *args, t_gc *gc)
{
	t_command_tree	*node;

	exec->argv = lst_to_argv(args, gc, &exec->no_expand_flags,
			&exec->was_quoted_flags);
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
