/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:36:52 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:56:13 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Opens an input file for reading, prints error and sets exit code on failure.
int	open_infile(t_minishell *shell, char *filename)
{
	int	fd;

	if (!filename)
		return (INVALID_FD);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		if (shell->current_command)
			write(STDERR_FILENO, shell->current_command,
				ft_strlen(shell->current_command));
		else
			write(STDERR_FILENO, "minishell", 9);
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, filename, ft_strlen(filename));
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
		write(STDERR_FILENO, "\n", 1);
		shell->exit_code = 1;
		return (INVALID_FD);
	}
	return (fd);
}
