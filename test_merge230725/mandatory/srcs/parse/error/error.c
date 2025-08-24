/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:52:21 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/24 14:22:47 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// void	error_msg(t_tokens *tokens, int *pos)
// {
// 	ft_putstr_fd("syntax error near unexpected token '", STDERR_FILENO);
// 	if ((*pos) >= tokens->count && tokens->arr[(*pos)].value == NULL)
// 		ft_putstr_fd("newline'\n", STDERR_FILENO);
// 	else
// 	{
// 		ft_putstr_fd(tokens->arr[(*pos)].value, STDERR_FILENO);
// 		ft_putstr_fd("'\n", STDERR_FILENO);
// 	}
// }

void	error_msg(t_tokens *tokens, int *pos, t_minishell *shell)
{
    if (!tokens || !pos || !shell || !tokens->arr)
        return;
    ft_putstr_fd("minishell: syntax error near unexpected token '", STDERR_FILENO);

    // Zusätzlicher Schutz: *pos muss im gültigen Bereich liegen!
    if ((*pos) < 0 || (*pos) >= tokens->count)
        ft_putstr_fd("newline", STDERR_FILENO);
    else if (!tokens->arr[(*pos)].value)
        ft_putstr_fd("newline", STDERR_FILENO);
    else
        ft_putstr_fd(tokens->arr[(*pos)].value, STDERR_FILENO);

    ft_putstr_fd("'\n", STDERR_FILENO);
    shell->exit_code = 2;
}


void	error_msg_paren(void)
{
	printf("minishell$ syntax error: unexpected end of file\n");
}
