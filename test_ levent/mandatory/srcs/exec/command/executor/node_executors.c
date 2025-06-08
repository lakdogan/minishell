#include "../../../../includes/core/minishell.h"

/**
 * @brief Executes a simple command node
 *
 * Handles the execution of a leaf node in the command tree that represents
 * a single command. Determines if the command is a built-in or an external
 * command and routes execution accordingly.
 *
 * @param node Pointer to the command tree node to execute
 * @param minishell Pointer to the shell state structure
 */
void	handle_exec(t_command_tree *node, t_minishell *minishell)
{
	t_exec	*exec;

	exec = (t_exec *)node->data;
	if (!exec || !exec->command || exec->command[FIRST_CHAR] == NULL_TERMINATOR)
	{
		minishell->exit_code = EXIT_SUCCESS;
		return ;
	}
	if (is_builtin(exec->command))
	{
		handle_var_expansion_exec(minishell, exec);
		handle_builtin(exec, minishell);
		return ;
	}
	handle_var_expansion_exec(minishell, exec);
	handle_external(exec, minishell);
}
