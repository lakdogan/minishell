/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 20:43:39 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 20:48:50 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

void	handle_builtin_and_exit(t_exec *exec, t_minishell *shell)
{
	exec_builtin(exec, shell);
	exit(shell->exit_code);
}
