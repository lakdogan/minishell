/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:35:24 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:59:24 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Prints error message and exits with cleanup.
void	exit_with_error(t_error_context ctx)
{
	ft_putstr_fd((char *)ctx.prefix, STDERR_FILENO);
	ft_putstr_fd((char *)ctx.subject, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd((char *)ctx.message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (ctx.shell)
		cleanup_memory(ctx.shell);
	exit(ctx.exit_code);
}
