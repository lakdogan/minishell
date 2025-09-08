/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_find_env_value.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:21:24 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:03:52 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Finds the value of an environment variable by name.
char	*find_env_value(t_minishell *minishell, const char *name)
{
	t_env	*env;

	env = find_env_var(name, minishell);
	if (!env || !env->content)
		return (NULL);
	if (ft_strchr(env->content, EQUALS_SIGN))
		return (ft_strchr(env->content, EQUALS_SIGN) + SKIP_EQUALS_SIGN);
	return (env->content);
}
