/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:37:57 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:55:49 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Handles input file redirection for a command.
static int	handle_input_file(t_minishell *shell, t_infile *in,
		bool in_child_process)
{
	char	*expanded_name;
	int		fd;

	if (!shell || !in || !in->name)
		return (EXIT_FAILURE);
	expanded_name = expand_variables_with_quotes(in->name, shell);
	if (!expanded_name)
		expanded_name = ft_strdup(in->name);
	fd = open_infile(shell, expanded_name);
	if (fd == INVALID_FD)
	{
		shell->redirection_failed = true;
		if (in_child_process)
			exit(EXIT_FAILURE);
		return (EXIT_FAILURE);
	}
	else
	{
		safe_dup2(shell, fd, STDIN_FILENO, "");
		safe_close(shell, fd, "close input file");
		return (EXIT_SUCCESS);
	}
}

// Handles heredoc input redirection.
static void	handle_heredoc_input(t_minishell *shell, int heredoc_fd)
{
	if (heredoc_fd > 0 && heredoc_fd != INVALID_FD)
	{
		safe_dup2(shell, heredoc_fd, STDIN_FILENO, "heredoc redirection");
		safe_close(shell, heredoc_fd, "close heredoc fd");
	}
}

// Sets up input redirections for a command.
void	setup_input_redirections(t_minishell *shell, t_exec *exec,
		bool in_child_process)
{
	t_infile	*in;
	int			result;

	result = EXIT_SUCCESS;
	if (!shell || !exec)
		return ;
	if (exec->command)
		shell->current_command = exec->command;
	in = exec->infiles;
	while (in)
	{
		if (in->type == INF_IN && in->name)
		{
			if (handle_input_file(shell, in, in_child_process) == EXIT_FAILURE)
			{
				result = EXIT_FAILURE;
				break ;
			}
		}
		in = in->next;
	}
	if (result == EXIT_SUCCESS && exec->heredoc_fd > 0
		&& exec->heredoc_fd != INVALID_FD)
		handle_heredoc_input(shell, exec->heredoc_fd);
	exec->redirection_failed = (result == EXIT_FAILURE);
}

// Opens an output file for redirection.
static int	open_output_file(t_minishell *shell, t_outfile *out)
{
	int	fd;

	fd = INVALID_FD;
	if (out->type == OUT_TRUNC)
		fd = open(out->name, O_WRONLY | O_CREAT | O_TRUNC,
				DEFAULT_FILE_PERMISSION);
	else if (out->type == OUT_APPEND)
		fd = open(out->name, O_WRONLY | O_CREAT | O_APPEND,
				DEFAULT_FILE_PERMISSION);
	if (fd == INVALID_FD)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(out->name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		shell->exit_code = 1;
		shell->redirection_failed = true;
	}
	return (fd);
}

// Sets up output redirections for a command.
void	setup_output_redirections(t_minishell *shell, t_exec *exec)
{
	t_outfile	*out;
	int			fd;

	if (!shell || !exec)
		return ;
	out = exec->outfiles;
	while (out)
	{
		fd = open_output_file(shell, out);
		if (fd == INVALID_FD)
		{
			exec->redirection_failed = true;
			break ;
		}
		if (fd != INVALID_FD)
		{
			if (out->fd_target == STDERR_FILENO)
				safe_dup2(shell, fd, STDERR_FILENO, "stderr redirection");
			else
				safe_dup2(shell, fd, STDOUT_FILENO, "output redirection");
			safe_close(shell, fd, "close output file");
		}
		out = out->next;
	}
}
