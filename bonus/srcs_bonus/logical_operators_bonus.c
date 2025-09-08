/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_operators_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:55:30 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:31:27 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

// Handles the AND (&&) logical operator.
void	handle_and_operator(t_minishell *minishell, t_command_tree *node)
{
	int	left_exit_code;

	execute_tree(node->left, minishell);
	left_exit_code = minishell->exit_code;
	if (left_exit_code == EXIT_SUCCESS)
		execute_tree(node->right, minishell);
	else
		minishell->exit_code = left_exit_code;
}

// Handles the OR (||) logical operator.
void	handle_or_operator(t_minishell *minishell, t_command_tree *node)
{
	int	left_exit_code;

	execute_tree(node->left, minishell);
	left_exit_code = minishell->exit_code;
	if (left_exit_code != EXIT_SUCCESS)
	{
		execute_tree(node->right, minishell);
	}
	else
		minishell->exit_code = left_exit_code;
}
