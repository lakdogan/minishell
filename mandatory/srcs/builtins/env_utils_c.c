/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_c.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:30:28 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:21:38 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Looks up an environment variable value.
int	lookup_env_value(const char *name, t_minishell *minishell, char **result)
{
	*result = find_env_value(minishell, name);
	if (*result == NULL)
		return (BUILTIN_FAILURE);
	return (BUILTIN_SUCCESS);
}
