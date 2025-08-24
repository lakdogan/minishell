/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 09:29:33 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/24 13:28:45 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/core/minishell.h"

t_command_tree	*setup_ast(char *line, t_minishell *shell, int arc, char **arv)
{
	t_tokens		*tokens;
	t_command_tree	*ast;

	(void)arc;
	(void)arv;
	tokens = lexer(line, shell);
	if (!tokens)
	{
		shell->exit_code = 2;
		return (NULL);
	}
	ast = start_parser(tokens, shell->gc[GC_COMMAND], shell);
	if (!ast)
	{
		shell->exit_code = 2;
		return (NULL);
	}
	return (ast);
}

// int	main(int argc, char **argv, char **env)
// {
// 	t_minishell	shell;

// 	init_minishell(&shell, env);
// 	while (1)
// 	{
// 		setup_parent_signals();
// 		if (isatty(fileno(stdin)))
// 		{
// 			shell.line = readline("minishell$ ");
// 			if (!shell.line)
// 			{
// 				ft_putstr_fd("exit\n", STDOUT_FILENO);
// 				break ;
// 			}
// 			if (shell.line[0] != '\0')
// 			{
// 				add_history((const char *)shell.line);
// 				shell.root = setup_ast(shell.line, &shell, argc, argv);
// 				if (shell.root)
// 					execute_tree(shell.root, &shell);
// 			}
// 		}
// 		free(shell.line);
// 	}
// 	return (gc_cleanup(shell.gc), shell.exit_code);
// }

int	main(int argc, char **argv, char **env)
{
	t_minishell	shell;
	char		*gnl_line;

	init_minishell(&shell, env);
	while (1)
	{
		setup_parent_signals();
		if (isatty(fileno(stdin)))
			shell.input = readline("minishell$ ");
		else
		{
			gnl_line = get_next_line(fileno(stdin));
			if (gnl_line)
			{
				shell.input = ft_strtrim(gnl_line, "\n");
				free(gnl_line);
			}
			else
				shell.input = NULL;
		}
		if (!shell.input)
		{
			// ft_putstr_fd("exit\n",
			// STDOUT_FILENO); // Wie gefordert auskommentiert
			break ;
		}
		if (shell.input[0] != '\0')
		{
			add_history((const char *)shell.input);
			shell.root = setup_ast(shell.input, &shell, argc, argv);
			if (shell.root == NULL)
            {
                gc_cleanup(shell.gc);
                return(shell.exit_code);
            }
            else
			    execute_tree(shell.root, &shell);
		}
		free(shell.input);
	}
	gc_cleanup(shell.gc);
	return (shell.exit_code);
}
