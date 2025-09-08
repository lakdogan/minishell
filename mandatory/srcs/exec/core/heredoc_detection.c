/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_detection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:08:19 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:05:18 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Checks if exec has heredoc redirection.
bool	has_heredoc_redirection(t_exec *exec)
{
	t_infile	*current;

	if (!exec || !exec->infiles)
		return (false);
	current = exec->infiles;
	while (current)
	{
		if (current->type == INF_HEREDOC)
			return (true);
		current = current->next;
	}
	return (false);
}

// Checks if any heredoc exists in the command tree.
bool	has_heredoc_in_tree(t_minishell *minishell, t_command_tree *node)
{
	t_exec	*exec;

	if (!node)
		return (false);
	if (node->type == NODE_CMD)
	{
		exec = node->data;
		if (exec && has_heredoc_redirection(exec))
			return (true);
	}
	else if (node->type == NODE_PIPE)
	{
		return (has_heredoc_in_tree(minishell, node->left)
			|| has_heredoc_in_tree(minishell, node->right));
	}
	return (false);
}

// Prepares heredocs if needed for the tree.
void	prepare_heredocs_if_needed(t_minishell *minishell, t_command_tree *root)
{
	static int	heredocs_prepared;
	static void	*last_root;

	if (root != last_root)
	{
		heredocs_prepared = HEREDOCS_NOT_PREPARED;
		last_root = root;
	}
	if (!heredocs_prepared && has_heredoc_in_tree(minishell, root))
	{
		prepare_heredocs(minishell, root);
		heredocs_prepared = HEREDOCS_PREPARED;
	}
}

// Prepares tree execution, including heredocs.
void	prepare_tree_execution(t_command_tree *root, t_minishell *minishell)
{
	if (!minishell->in_nested_pipe)
		prepare_heredocs_if_needed(minishell, root);
}
