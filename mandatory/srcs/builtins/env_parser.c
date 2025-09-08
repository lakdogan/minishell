/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:14:09 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 19:17:01 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

/* Find environment variable by key */
t_env	*find_env_by_key(t_minishell *minishell, const char *key)
{
	t_list	*node;
	t_env	*env;

	node = minishell->envp;
	while (node)
	{
		env = (t_env *)node->content;
		if (ft_strcmp(env->value, key) == 0)
			return (env);
		node = node->next;
	}
	return (NULL);
}

/* Extract the key part from an environment string */
char	*extract_key(t_minishell *minishell, const char *arg)
{
	int	len;

	len = 0;
	while (arg[len] && arg[len] != EQUALS_SIGN)
		len++;
	return (gc_substr(minishell->gc[GC_TEMP], arg, FIRST_CHAR, len));
}

/* Extract the value part from an environment string */
char	*extract_value(t_minishell *minishell, const char *arg)
{
	char	*equal;

	equal = ft_strchr(arg, EQUALS_SIGN);
	if (!equal)
		return (NULL);
	if (*(equal + 1) == '\0')
		return (gc_strdup(minishell->gc[GC_TEMP], ""));
	return (gc_strdup(minishell->gc[GC_TEMP], equal + 1));
}
