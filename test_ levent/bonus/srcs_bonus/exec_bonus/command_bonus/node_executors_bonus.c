

/**
 * @file node_executors_bonus.c
 * @brief Implements command tree node execution handlers
 *
 * This file contains functions for
 * executing different types of command tree nodes
 * in the shell,
 *	including simple commands and pipes. It manages the process creation,
 * pipe setup, and proper execution flow for each node type.
 */

#include "../../../includes_bonus/core_bonus/minishell_bonus.h"

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
	if (!exec || !exec->command || exec->command[0] == '\0')
	{
		minishell->exit_code = EXIT_SUCCESS;
		return ;
	}
	if (is_builtin(exec->command))
	{
		handle_builtin(exec, minishell);
		return ;
	}
	handle_external(exec, minishell);
}

/**
 * @brief Executes the left side of a pipe command
 *
 * Sets up the pipe's write end as standard output and executes the
 * left subtree of the pipe node. This function runs in a child process
 * and exits with the command's exit code.
 *
 * @param node Pointer to the pipe node
 * @param shell Pointer to the shell state structure
 * @param pipefd Array containing pipe file descriptors
 */
static void	execute_left_cmd(t_command_tree *node, t_minishell *shell,
		int *pipefd)
{
	setup_child_signals();
	safe_close(pipefd[0], "pipe close read end");
	safe_dup2(pipefd[1], STDOUT_FILENO, "pipe left command stdout");
	safe_close(pipefd[1], "pipe close write end after dup");
	execute_tree(node->left, shell);
	exit(shell->exit_code);
}

/**
 * @brief Executes the right side of a pipe command
 *
 * Sets up the pipe's read end as standard input and executes the
 * right subtree of the pipe node. This function runs in a child process
 * and exits with the command's exit code.
 *
 * @param node Pointer to the pipe node
 * @param shell Pointer to the shell state structure
 * @param pipefd Array containing pipe file descriptors
 */
static void	execute_right_cmd(t_command_tree *node, t_minishell *shell,
		int *pipefd)
{
	setup_child_signals();
	safe_close(pipefd[1], "pipe close write end");
	safe_dup2(pipefd[0], STDIN_FILENO, "pipe right command stdin");
	safe_close(pipefd[0], "pipe close read end after dup");
	execute_tree(node->right, shell);
	exit(shell->exit_code);
}

/**
 * @brief Waits for both pipe command child processes to complete
 *
 * Waits for the termination of both the left and right command processes
 * and updates the shell's exit code based on the last process to finish.
 *
 * @param left_pid Process ID of the left command
 * @param right_pid Process ID of the right command
 * @param shell Pointer to the shell state structure
 */
static void	wait_for_children(pid_t left_pid, pid_t right_pid,
		t_minishell *shell)
{
	wait_for_process(left_pid, shell);
	wait_for_process(right_pid, shell);
}

/**
 * @brief Handles execution of a pipe node
 *
 * Creates a pipe and two child processes to execute the left and right
 * sides of the pipe. The parent process waits for both children to complete.
 * Handles errors in pipe creation and process forking.
 *
 * @param node Pointer to the pipe node to execute
 * @param shell Pointer to the shell state structure
 */
void	handle_pipe(t_command_tree *node, t_minishell *shell)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (!node || !node->left || !node->right)
		return ;
	if (!create_pipe(pipefd))
		return ;
	left_pid = create_child_process();
	if (left_pid == FORK_ERROR)
		return ;
	if (left_pid == CHILD_PROCESS)
		execute_left_cmd(node, shell, pipefd);
	right_pid = create_child_process();
	if (right_pid == FORK_ERROR)
	{
		close_pipe(pipefd);
		waitpid(left_pid, NULL, WAIT_BLOCK);
		return ;
	}
	if (right_pid == CHILD_PROCESS)
		execute_right_cmd(node, shell, pipefd);
	close_pipe(pipefd);
	wait_for_children(left_pid, right_pid, shell);
}
