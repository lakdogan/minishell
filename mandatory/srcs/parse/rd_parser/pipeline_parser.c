/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:30:02 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 06:36:41 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Validates that pipeline does not start with a pipe.
static int	validate_pipeline_start(t_tokens *tokens, int *pos,
		t_minishell *shell)
{
	if ((*pos) < tokens->count && tokens->arr[(*pos)].type == PIPE)
	{
		shell->exit_code = 2;
		return (0);
	}
	return (1);
}

// Validates that a pipe is followed by a valid command.
static int	validate_pipe_continuation(t_tokens *tokens, int *pos,
		t_minishell *shell)
{
	if ((*pos) + 1 >= tokens->count)
		return (1);
	if (tokens->arr[(*pos) + 1].type == PIPE)
	{
		shell->exit_code = 2;
		return (0);
	}
	return (1);
}

// Processes a pipe operation in the pipeline.
static t_command_tree	*process_pipe_operation(t_command_tree *left,
		t_tokens *tokens, int *pos, t_minishell *shell)
{
	t_command_tree	*right;
	t_command_tree	*node;

	if (!validate_pipe_continuation(tokens, pos, shell))
		return (NULL);
	(*pos)++;
	right = parse_sub_or_cmd(tokens, pos, shell);
	if (!right)
		return (NULL);
	node = get_node(NODE_PIPE, left, right, shell->gc[GC_COMMAND]);
	return (node);
}

// Parses a pipeline of commands separated by pipes.
t_command_tree	*parse_pipeline(t_tokens *tokens, int *pos, t_minishell *shell)
{
	t_command_tree	*left;
	t_command_tree	*node;

	if (!validate_pipeline_start(tokens, pos, shell))
		return (NULL);
	left = parse_sub_or_cmd(tokens, pos, shell);
	if (!left)
		return (NULL);
	while ((*pos) < tokens->count && tokens->arr[(*pos)].type == PIPE)
	{
		node = process_pipe_operation(left, tokens, pos, shell);
		if (!node)
			return (NULL);
		left = node;
	}
	return (left);
}

// Creates a new AST node for the pipeline.
t_command_tree	*get_node(t_node_type type, t_command_tree *left,
		t_command_tree *right, t_gc *gc)
{
	t_command_tree	*new_node;

	new_node = gc_malloc(gc, sizeof(t_command_tree));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->left = left;
	new_node->right = right;
	new_node->cmd = NULL;
	return (new_node);
}
