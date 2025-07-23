

/**
 * @file execute_tree_bonus.c
 * @brief Command tree execution functionality
 *
 * This file contains the core functionality for executing a command tree
 * structure in the shell. It handles validation of the tree, preparation
 * of heredocs, and dispatching execution based on node types.
 */

#include "../../inc_bonus/minishell_bonus.h"

/**
 * @brief Routes execution to the appropriate handler based on node type
 *
 * Dispatches command tree node execution to specialized handlers based on
 * the node's type (exec, pipe, AND, OR). Each handler implements the specific
 * behavior required for that node type.
 *
 * @param node Pointer to the command tree node to execute
 * @param minishell Pointer to the shell state structure
 */
static void	execute_node_by_type(t_command_tree *node, t_minishell *minishell)
{
	if (node->type == N_EXEC)
		handle_exec(node, minishell);
	else if (node->type == N_PIPE)
		handle_pipe(minishell, node);
	else if (node->type == N_ANDIF)
		handle_and_operator(minishell, node);
	else if (node->type == N_OR)
		handle_or_operator(minishell, node);
}

/**
 * @brief Validates that the command tree is not NULL
 *
 * Performs basic validation on the command tree before execution.
 * Currently only checks for NULL tree, but could be extended for
 * more comprehensive validation.
 *
 * @param root Pointer to the root of the command tree
 * @return int VALIDATION_SUCCESS if valid, VALIDATION_FAILURE otherwise
 */
static int	validate_command_tree(t_command_tree *root)
{
	if (!root)
	{
		ft_putstr_fd("Error: Command tree is NULL\n", STDERR_FILENO);
		return (VALIDATION_FAILURE);
	}
	return (VALIDATION_SUCCESS);
}

/**
 * @brief Prepares heredocs for the command tree if not already prepared
 *
 * Uses static variables to track whether heredocs have been prepared for
 * a given tree. This prevents re-preparing heredocs for the same tree
 * if execute_tree is called multiple times, but ensures different trees
 * have their heredocs properly prepared.
 *
 * @param root Pointer to the root of the command tree
 */
static void	prepare_heredocs_if_needed(t_minishell *shell, t_command_tree *root)
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

/**
 * @brief Main entry point for executing a command tree
 *
 * Orchestrates the execution of an entire command tree by:
 * 1. Validating the tree structure
 * 2. Preparing heredocs if needed
 * 3. Executing the appropriate handler for the root node
 * 4. Waiting for all child processes to complete
 *
 * @param root Pointer to the root of the command tree
 * @param minishell Pointer to the shell state structure
 */
void	execute_tree(t_command_tree *root, t_minishell *minishell)
{
	int	status;

	if (!validate_command_tree(root))
	{
		minishell->exit_code = EXIT_FAILURE;
		return ;
	}
	prepare_heredocs_if_needed(minishell, root);
	execute_node_by_type(root, minishell);
	while (waitpid(-1, &status, 0) > 0)
		continue ;
}
