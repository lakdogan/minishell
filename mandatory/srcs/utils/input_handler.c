/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:58:12 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 01:29:42 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Reads input from user or stdin.
char	*read_shell_input(t_minishell *shell)
{
	char	*gnl_line;

	if (isatty(fileno(stdin)))
		return (readline("minishell$ "));
	else
	{
		gnl_line = get_next_line(fileno(stdin));
		if (gnl_line)
		{
			shell->input = ft_strtrim(gnl_line, "\n");
			free(gnl_line);
			return (shell->input);
		}
		return (NULL);
	}
}

// Processes a single command line.
void	process_command(t_minishell *shell, int argc, char **argv)
{
	shell->redirection_failed = false;
	add_history((const char *)shell->input);
	shell->root = setup_ast(shell->input, shell, argc, argv);
	if (shell->root == NULL)
	{
		if (!isatty(STDIN_FILENO))
		{
			shell->exit_code = 2;
			gc_cleanup(shell->gc);
			exit(shell->exit_code);
		}
	}
	else
	{
		execute_tree(shell->root, shell);
		waitpid(-1, NULL, 0);
	}
}

// Main shell input-processing loop.
void	shell_loop(t_minishell *shell, int argc, char **argv)
{
	while (1)
	{
		shell->input = read_shell_input(shell);
		if (!shell->input)
			break ;
		if (shell->input[0] != '\0')
			process_command(shell, argc, argv);
		reset_terminal_mode();
		free(shell->input);
	}
	gc_cleanup(shell->gc);
	exit(shell->exit_code);
}
