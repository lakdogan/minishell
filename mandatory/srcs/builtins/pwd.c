/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:21:55 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:25:48 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Implementation of the pwd builtin.
int	ft_pwd(void)
{
	char	cwd[PATH_MAX];
	int		len;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		len = ft_strlen(cwd);
		write(STDOUT_FILENO, cwd, len);
		write(STDOUT_FILENO, "\n", 1);
		return (BUILTIN_SUCCESS);
	}
	else
	{
		perror("pwd");
		return (BUILTIN_FAILURE);
	}
}
