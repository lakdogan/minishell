/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:22:52 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:27:06 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Checks if the given string is a valid identifier.
static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[FIRST_CHAR] || ft_isdigit(str[FIRST_CHAR]))
		return (IDENTIFIER_INVALID);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != UNDERSCORE)
			return (IDENTIFIER_INVALID);
		i++;
	}
	return (IDENTIFIER_VALID);
}

// Removes an environment variable from the list.
static void	remove_env_var(const char *name, t_minishell *minishell)
{
	t_list	*prev;
	t_list	*node;
	t_env	*env;

	prev = NULL;
	node = minishell->envp;
	while (node)
	{
		env = (t_env *)node->content;
		if (env && ft_strcmp(env->value, name) == STRINGS_EQUAL)
		{
			if (prev)
				prev->next = node->next;
			else
				minishell->envp = node->next;
			unsetenv(name);
			return ;
		}
		prev = node;
		node = node->next;
	}
}

// Implementation of the unset builtin.
int	ft_unset(char **argv, t_minishell *minishell)
{
	int	i;
	int	status;

	status = BUILTIN_SUCCESS;
	if (!argv || !argv[1])
		return (BUILTIN_SUCCESS);
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			write(STDERR_FILENO, "unset: `", 8);
			write(STDERR_FILENO, argv[i], ft_strlen(argv[i]));
			write(STDERR_FILENO, "': not a valid identifier\n", 27);
			status = BUILTIN_FAILURE;
		}
		else
		{
			remove_env_var(argv[i], minishell);
		}
		i++;
	}
	minishell->envp_arr = rebuild_env_array(minishell);
	return (status);
}
