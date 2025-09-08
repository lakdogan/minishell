/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_right_cmd_prepare.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:47:36 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:14:50 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Prepares pipe ends for right command.
void	prepare_pipe_ends(t_minishell *shell, int *pipefd)
{
	safe_close(shell, pipefd[PIPE_WRITE_END], "pipe close write end");
	safe_dup2(shell, pipefd[PIPE_READ_END], STDIN_FILENO,
		"pipe right command stdin");
	safe_close(shell, pipefd[PIPE_READ_END], "pipe close read end after dup");
}

// Sets up default pipe input for right command.
void	setup_default_pipe_input(t_minishell *shell, int *pipefd)
{
	safe_close(shell, pipefd[PIPE_WRITE_END], "pipe close write end");
	safe_dup2(shell, pipefd[PIPE_READ_END], STDIN_FILENO,
		"pipe right command stdin");
	safe_close(shell, pipefd[PIPE_READ_END], "pipe close read end after dup");
}

// Checks if infile exists for exec, closes or sets up pipe accordingly.
void	check_if_infile_exists(t_minishell *shell, t_exec *exec, int *pipefd)
{
	if (exec->infiles)
		close_pipe(shell, pipefd);
	else
		setup_default_pipe_input(shell, pipefd);
}

// Sets up input and output redirections for exec.
void	setup_in_and_output_redirections(t_minishell *shell, t_exec *exec)
{
	setup_input_redirections(shell, exec, true);
	setup_output_redirections(shell, exec);
}

// Checks if exec has valid data, exits if not.
void	check_exec_has_valid_data(t_minishell *shell, t_exec *exec)
{
	if (!exec)
		exit(shell->exit_code);
}
