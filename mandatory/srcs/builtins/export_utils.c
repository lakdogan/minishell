/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:29:46 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 00:30:00 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

/* Check if option flag is provided and handle it */
int	handle_export_option(char *arg)
{
	write(STDERR_FILENO, "minishell: export: ", 20);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, ": invalid option\n", 17);
	write(STDERR_FILENO,
		"export: usage: export [-nf] [name[=value] ...] or export -p\n", 61);
	return (2);
}
