/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:41:13 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:33:43 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Builds AST from input line.
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
	ast = start_parser(tokens, shell);
	if (!ast)
	{
		shell->exit_code = 2;
		return (NULL);
	}
	return (ast);
}
