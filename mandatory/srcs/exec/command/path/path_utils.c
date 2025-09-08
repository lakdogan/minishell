/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:05:29 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:10:31 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Gets the PATH variable from the environment and splits it.
int	get_path_from_env(t_minishell *shell, char **envp, char ***paths)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], PATH_VAR_PREFIX,
				PATH_PREFIX_LENGTH) == STRINGS_EQUAL)
		{
			*paths = gc_split(shell->gc[GC_MAIN], envp[i] + PATH_PREFIX_LENGTH,
					PATH_DELIMITER);
			return (OPERATION_SUCCESS);
		}
		i++;
	}
	*paths = NULL;
	return (OPERATION_FAILURE);
}
