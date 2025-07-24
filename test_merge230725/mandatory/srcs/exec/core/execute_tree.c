/**
 * @file execute_tree.c
 * @brief Command tree execution functionality
 *
 * This file contains the core functionality for executing a command tree
 * structure in the shell. It handles validation of the tree, preparation
 * of heredocs, and dispatching execution based on node types.
 */

#include "../../../includes/core/minishell.h"


static bool has_heredoc_redirection(t_exec *exec)
{
	t_infile *current;
	
	if (!exec || !exec->infiles)
		return false;
		
	current = exec->infiles;
	while (current)
	{
		if (current->type == INF_HEREDOC)
			return true;
		current = current->next;
	}
	return false;
}
/**
 * @brief Routes execution to the appropriate handler based on node type
 *
 * Dispatches command tree node execution to specialized handlers based on
 * the node's type (exec, pipe). Each handler implements the specific
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

static bool has_heredoc_in_tree(t_minishell *minishell, t_command_tree *node)
{
	t_exec *exec;
	
	if (!node)
		return false;
		
	if (node->type == N_EXEC)
	{
		exec = node->data;
		if (exec && has_heredoc_redirection(exec))
			return true;
	}
	else if (node->type == N_PIPE)
	{
		return (has_heredoc_in_tree(minishell, node->left) || 
				has_heredoc_in_tree(minishell, node->right));
	}
	
	return false;
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
static void prepare_heredocs_if_needed(t_minishell *minishell, t_command_tree *root)
{
    static int heredocs_prepared;
    static void *last_root;

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
	if (!minishell->in_nested_pipe)
		prepare_heredocs_if_needed(minishell, root);
	execute_node_by_type(root, minishell);
	if (!minishell->in_nested_pipe)
	{
		while (waitpid(WAIT_ANY, &status, WAIT_BLOCK) > NO_PROCESS)
			continue ;
	}
	minishell->in_nested_pipe = false;
}
