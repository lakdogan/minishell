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
	if (node->type == NODE_CMD)
		handle_exec(node, minishell);
	else if (node->type == NODE_PIPE)
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

	if (node->type == NODE_CMD)
	{
		exec = node->data;
		if (exec && has_heredoc_redirection(exec))
			return true;
	}
	else if (node->type == NODE_PIPE)
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

// Helper function to handle assignment-only commands
static void handle_assignment_command(t_command *cmd, t_minishell *shell)
{
    int i;
    char *name, *value, *eq_pos;
    
    if (!cmd->env_vars)
        return;
    
    i = 0;
    while (cmd->env_vars[i])
    {
        eq_pos = ft_strchr(cmd->env_vars[i], '=');
        if (eq_pos)
        {
            // Extract name and value
            *eq_pos = '\0';  // Temporarily null-terminate
            name = cmd->env_vars[i];
            value = eq_pos + 1;
            
            // Set the environment variable
            set_env_var(name, value, shell);
            
            *eq_pos = '=';  // Restore the string
        }
        i++;
    }
    
    // Rebuild the environment array
    shell->envp_arr = rebuild_env_array(shell);
    shell->exit_code = 0;  // Assignment commands always succeed
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

	if (!root) {
        printf("execute_tree: root is NULL\n");
        return;
    }
	if (!validate_command_tree(root))
	{
		minishell->exit_code = EXIT_FAILURE;
		return ;
	}
	if (!minishell->in_nested_pipe)
		prepare_heredocs_if_needed(minishell, root);

	if (root->type == NODE_CMD)
	{
		// Check if this is an assignment-only command
		if (root->cmd && root->cmd->env_vars && !root->cmd->argv)
		{
			handle_assignment_command(root->cmd, minishell);
			return;
		}
		// ...existing execution code for regular commands...
	}

	execute_node_by_type(root, minishell);
	if (!minishell->in_nested_pipe)
	{
		while (waitpid(WAIT_ANY, &status, WAIT_BLOCK) > NO_PROCESS)
			continue ;
	}
	minishell->in_nested_pipe = false;
}

// void execute_tree(t_command_tree *root, t_minishell *minishell)
// {
//     int status;
//     // Reset the flag at the beginning
//     minishell->redirection_failed = false;

//     if (!validate_command_tree(root))
//     {
//         minishell->exit_code = EXIT_FAILURE;
//         return;
//     }

//     if (!minishell->in_nested_pipe)
//         prepare_heredocs_if_needed(minishell, root);

//     // Execute the command tree
//     execute_node_by_type(root, minishell);

//     // Only wait for processes if we're not in a nested pipe
//     if (!minishell->in_nested_pipe)
//     {
//         // Check if any processes were actually created
//         if (minishell->last_pid > 0) {
//             // Wait for all child processes to complete
//             while (waitpid(WAIT_ANY, &status, WAIT_BLOCK) > NO_PROCESS)
//                 continue;
//             minishell->last_pid = 0; // Reset PID tracker after waiting
//         }
//         // If no processes were created but redirection failed, ensure exit code is set
//         else if (minishell->redirection_failed) {
//             minishell->exit_code = 1;  // Force exit code to 1 for failed redirection
//             // No waiting needed - return to prompt immediately
//         }
//     }
//     minishell->in_nested_pipe = false;
// }
