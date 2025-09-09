/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 23:30:15 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 18:37:06 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Checks if variable name is valid.
static int	is_var_name_valid(const char *var)
{
	return (var && *var);
}

// Extracts value from environment variable string.
static char	*extract_env_value(char *content)
{
	char	*eq;

	if (!content)
		return ("");
	eq = ft_strchr(content, '=');
	if (eq)
		return (eq + 1);
	return (content);
}

// Gets environment variable value from shell.
char	*get_env_var(t_minishell *shell, const char *var)
{
	t_env	*env;

	if (!is_var_name_valid(var))
		return ("");
	env = find_env_var(var, shell);
	if (env && env->content)
		return (extract_env_value(env->content));
	return ("");
}
