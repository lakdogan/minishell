/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmd_array.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:45:35 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:18:13 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Counts the number of pipe nodes in the tree.
static int	count_pipe_nodes(t_command_tree *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
		return (count_pipe_nodes(node->left) + 1);
	return (1);
}

// Fills the exec array from the command tree.
static void	fill_exec_array(t_command_tree *node, t_exec **execs, int *idx)
{
	t_exec	*exec;

	if (!node)
		return ;
	if (node->type == NODE_PIPE)
	{
		fill_exec_array(node->left, execs, idx);
		fill_exec_array(node->right, execs, idx);
	}
	else if (node->type == NODE_EXEC || node->type == NODE_CMD)
	{
		exec = (t_exec *)node->data;
		execs[*idx] = exec;
		(*idx)++;
	}
	else if (node->type == NODE_SUBSHELL)
	{
		execs[*idx] = (t_exec *)node;
		(*idx)++;
	}
}

// Builds an array of execs from the command tree.
t_exec	**build_exec_array_from_tree(t_command_tree *node)
{
	int		num_cmds;
	t_exec	**execs;
	int		idx;

	num_cmds = count_pipe_nodes(node);
	execs = malloc(sizeof(t_exec *) * (num_cmds + 1));
	idx = 0;
	if (!execs)
		return (NULL);
	fill_exec_array(node, execs, &idx);
	execs[idx] = NULL;
	return (execs);
}
