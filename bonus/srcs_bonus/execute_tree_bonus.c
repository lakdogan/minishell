/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:53:51 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:29:48 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

// Validates the command tree before execution.
static int	validate_command_tree_bonus(t_command_tree *root)
{
	if (!root)
	{
		ft_putstr_fd("Error: Command tree is NULL\n", STDERR_FILENO);
		return (VALIDATION_FAILURE);
	}
	return (VALIDATION_SUCCESS);
}

// Prepares heredocs if needed for the command tree.
static void	prepare_heredocs_if_needed_bonus(t_minishell *shell,
		t_command_tree *root)
{
	static int	heredocs_prepared;
	static void	*last_root;

	if (root != last_root)
	{
		heredocs_prepared = HEREDOCS_NOT_PREPARED;
		last_root = root;
	}
	if (!heredocs_prepared)
	{
		prepare_heredocs(shell, root);
		heredocs_prepared = HEREDOCS_PREPARED;
	}
}

// Executes the command tree.
void	execute_tree(t_command_tree *root, t_minishell *minishell)
{
	int	status;

	if (!validate_command_tree_bonus(root))
	{
		minishell->exit_code = EXIT_FAILURE;
		return ;
	}
	prepare_heredocs_if_needed_bonus(minishell, root);
	execute_node_by_type_bonus(root, minishell);
	while (waitpid(-1, &status, 0) > 0)
		continue ;
}
