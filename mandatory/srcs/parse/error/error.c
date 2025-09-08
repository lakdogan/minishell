/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:52:21 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/08 23:51:05 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Prints a syntax error message for a token.
void	error_msg(t_tokens *tokens, int *pos, t_minishell *shell)
{
	if (!tokens || !pos || !shell || !tokens->arr)
		return ;
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	if ((*pos) < 0 || (*pos) >= tokens->count)
		ft_putstr_fd("newline", STDERR_FILENO);
	else if (!tokens->arr[(*pos)].value)
		ft_putstr_fd("newline", STDERR_FILENO);
	else
		ft_putstr_fd(tokens->arr[(*pos)].value, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	shell->exit_code = 2;
}

// Prints a syntax error message for unexpected end of file (parenthesis).
void	error_msg_paren(void)
{
	printf("minishell$ syntax error: unexpected end of file\n");
}
