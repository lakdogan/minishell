

/**
 * @file logical_operators_bonus.c
 * @brief Implementation of logical operator handling for command execution
 *
 * This file contains functions for implementing the short-circuit logical
 * operators (AND and OR) in the command tree. These operators control the
 * conditional execution of commands based on the success or failure of
 * previous commands.
 */

#include "../../inc_bonus/minishell_bonus.h"

/**
 * @brief Handles the logical AND (&&) operator node execution
 *
 * Implements the short-circuit AND behavior where the right subtree is
 * executed only if the left subtree execution succeeds (returns EXIT_SUCCESS).
 * If the left command fails, the right command is skipped.
 *
 * @param node Pointer to the AND operator node in the command tree
 * @param minishell Pointer to the shell state structure
 */
void	handle_and_operator(t_minishell *minishell, t_command_tree *node)
{
	if (!node || !node->left || !node->right)
		return ;
	execute_tree(node->left, minishell);
	if (minishell->exit_code == EXIT_SUCCESS)
		execute_tree(node->right, minishell);
}

/**
 * @brief Handles the logical OR (||) operator node execution
 *
 * Implements the short-circuit OR behavior where the right subtree is
 * executed only if the left subtree execution fails (returns non-zero).
 * If the left command succeeds, the right command is skipped.
 *
 * @param node Pointer to the OR operator node in the command tree
 * @param minishell Pointer to the shell state structure
 */
void	handle_or_operator(t_minishell *minishell, t_command_tree *node)
{
	execute_tree(node->left, minishell);
	if (minishell->exit_code != EXIT_SUCCESS)
		execute_tree(node->right, minishell);
}
