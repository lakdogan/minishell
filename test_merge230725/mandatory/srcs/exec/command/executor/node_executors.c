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

	if (!node)
        return;
	exec = (t_exec *)node->data;
	if (!exec) {
        printf("handle_exec: exec is NULL\n");
        return;
    }
    if (!exec->command) {
        printf("handle_exec: exec->command is NULL\n");
        return;
    }
	if (!exec || !exec->command || exec->command[FIRST_CHAR] == NULL_TERMINATOR)
    {
        minishell->exit_code = EXIT_SUCCESS;
        return;
    }
    
    if (is_builtin(exec->command))
    {

        handle_var_expansion_exec(minishell, exec);
		int stdin_backup = dup(STDIN_FILENO);
		int stdout_backup = dup(STDOUT_FILENO);
        
        // Set up redirections for built-in command (in the main process)
        setup_input_redirections(minishell, exec, false);
        
        // Check if redirection failed before executing the built-in
		if (exec->redirection_failed)
		{
			// Restore original file descriptors
			dup2(stdin_backup, STDIN_FILENO);
			dup2(stdout_backup, STDOUT_FILENO);
			close(stdin_backup);
			close(stdout_backup);
			
			minishell->redirection_failed = true;
			minishell->exit_code = 1;
			return;
		}
        
        // Set up output redirections and execute the built-in
        setup_output_redirections(minishell, exec);
        handle_builtin(exec, minishell);
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
        return;
    }
    
    // Handle external commands as before
    if (exec->redirection_failed)
    {
        minishell->redirection_failed = true;
        minishell->exit_code = 1;
        return;
    }
	if (exec->redirection_failed)
	{
		minishell->redirection_failed = true;
		minishell->exit_code = 1;  // Ensure exit code is set
		return;
	}
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

// void handle_exec(t_command_tree *node, t_minishell *minishell)
// {
//     t_exec *exec;

//     exec = (t_exec *)node->data;
//     if (!exec || !exec->command || exec->command[FIRST_CHAR] == NULL_TERMINATOR)
//     {
//         minishell->exit_code = EXIT_SUCCESS;
//         return;
//     }
    
//     if (is_builtin(exec->command))
//     {
//         // First, do variable expansion
//         handle_var_expansion_exec(minishell, exec);
        
//         // Set up redirections for built-in command (in the main process)
//         setup_input_redirections(minishell, exec, false);
        
//         // Check if redirection failed before executing the built-in
//         if (exec->redirection_failed)
//         {
//             minishell->redirection_failed = true;
//             minishell->exit_code = 1;
//             return;
//         }
        
//         // Set up output redirections and execute the built-in
//         setup_output_redirections(minishell, exec);
//         handle_builtin(exec, minishell);
//         return;
//     }
    
//     // Handle external commands as before
//     if (exec->redirection_failed)
//     {
//         minishell->redirection_failed = true;
//         minishell->exit_code = 1;
//         return;
//     }
    
//     // Rest of your function for external commands...
// }