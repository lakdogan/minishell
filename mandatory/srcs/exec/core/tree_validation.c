/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_validation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:07:28 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 19:59:49 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Validates the command tree root.
int	validate_command_tree(t_command_tree *root)
{
	if (!root)
	{
		ft_putstr_fd("Error: Command tree is NULL\n", STDERR_FILENO);
		return (VALIDATION_FAILURE);
	}
	return (VALIDATION_SUCCESS);
}

// Handles invalid command tree.
void	handle_invalid_tree(t_command_tree *root, t_minishell *minishell)
{
	if (!root)
		ft_printf("execute_tree: root is NULL\n");
	minishell->exit_code = EXIT_FAILURE;
}

// Checks if the command is assignment-only.
bool	is_assignment_only_command(t_command_tree *root)
{
	return (root->type == NODE_CMD && root->cmd && root->cmd->env_vars
		&& !root->cmd->argv);
}
