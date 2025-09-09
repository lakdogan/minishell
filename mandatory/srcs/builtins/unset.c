/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:22:52 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 23:08:28 by lakdogan         ###   ########.fr       */
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

void	unset_env_var(const char *key, t_minishell *minishell)
{
	t_list	*prev;
	t_list	*curr;
	t_list	*next;
	t_env	*env;

	prev = NULL;
	curr = minishell->envp;
	if (!key)
		return ;
	while (curr)
	{
		next = curr->next;
		env = (t_env *)curr->content;
		if (env && env->value && ft_strcmp(env->value, key) == 0)
		{
			if (prev)
				prev->next = next;
			else
				minishell->envp = next;
			break ;
		}
		prev = curr;
		curr = next;
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
			unset_env_var(argv[i], minishell);
		}
		i++;
	}
	minishell->envp_arr = rebuild_env_array(minishell);
	return (status);
}
