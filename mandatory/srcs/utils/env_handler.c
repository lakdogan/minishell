/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:42:35 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:33:12 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Returns incremented SHLVL string.
char	*get_shlvl(const char *lvl, t_gc *gc)
{
	int		shlvl;
	char	*tmp;
	char	*new_lvl;

	shlvl = ft_atoi(lvl + 6);
	shlvl++;
	tmp = ft_itoa(shlvl);
	if (!tmp)
		return (NULL);
	new_lvl = gc_strjoin(gc, "SHLVL=", tmp);
	free(tmp);
	return (new_lvl);
}

// Increments SHLVL in environment array.
int	increment_shlvl(t_minishell *shell)
{
	int		i;
	char	**env;
	char	*shlvl;

	env = shell->envp_arr;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
		{
			shlvl = get_shlvl(env[i], shell->gc[GC_LVL]);
			if (!shlvl)
				return (0);
			env[i] = shlvl;
			return (1);
		}
		i++;
	}
	return (0);
}
