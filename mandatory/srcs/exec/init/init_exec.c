/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:35:34 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:59:04 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/execution.h"

// Initializes exec structure for heredoc usage.
void	initialize_exec(t_exec *exec)
{
	exec->heredoc_fd = INVALID_FD;
	exec->heredoc_prepared = false;
}
