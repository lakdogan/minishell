/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 20:42:14 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 20:49:18 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

void	handle_redirections_and_exit(t_exec *exec, t_minishell *shell)
{
	setup_input_redirections(shell, exec, true);
	setup_output_redirections(shell, exec);
	if (!exec->redirection_failed)
		exit(0);
	else
		exit(1);
}
