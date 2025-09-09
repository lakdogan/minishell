/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:35:45 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 01:23:22 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Creates a child process and updates shell state.
pid_t	create_child_process(t_minishell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid > 0)
	{
		shell->last_pid = pid;
	}
	return (pid);
}

// Processes the exit status of a child process.
void	process_child_status(int status, t_minishell *shell)
{
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_code = SIGNAL_EXIT_BASE + WTERMSIG(status);
	else
		shell->exit_code = GENERAL_ERROR;
}

// Checks if a command is a builtin.
int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (FALSE);
	return (ft_strncmp(cmd, "echo", SIZE_MAX) == STRINGS_EQUAL
		|| ft_strncmp(cmd, "cd", SIZE_MAX) == STRINGS_EQUAL || ft_strncmp(cmd,
			"pwd", SIZE_MAX) == STRINGS_EQUAL || ft_strncmp(cmd, "export",
			SIZE_MAX) == STRINGS_EQUAL || ft_strncmp(cmd, "unset",
			SIZE_MAX) == STRINGS_EQUAL || ft_strncmp(cmd, "env",
			SIZE_MAX) == STRINGS_EQUAL || ft_strncmp(cmd, "exit",
			SIZE_MAX) == STRINGS_EQUAL);
}

// Waits for a specific process and updates shell status.
void	wait_for_process(pid_t pid, t_minishell *shell)
{
	int	status;

	waitpid(pid, &status, WAIT_BLOCK);
	if (WIFSIGNALED(status))
		write(STDERR_FILENO, "\n", 1);
	process_child_status(status, shell);
}

// Executes a builtin or external command.
void	execute_builtin_or_external_cmd(t_minishell *shell, t_exec *exec)
{
	if (is_builtin(exec->command))
		shell->exit_code = exec_builtin(exec, shell);
	else
		execute_command(exec, shell);
}
