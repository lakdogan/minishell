/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:50:55 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:18:38 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Checks if the string contains only whitespace.
int	ft_is_whitespace_str(const char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (!ft_isspace(*str))
			return (0);
		str++;
	}
	return (1);
}

/* Expand variables in command arguments */
void	expand_command_args(t_minishell *shell, t_exec *exec)
{
	int	i;

	if (!exec->argv)
		return ;
	i = 0;
	while (exec->argv[i])
	{
		if (!exec->no_expand_flags || !exec->no_expand_flags[i])
		{
			exec->argv[i] = expand_variables_with_quotes(exec->argv[i], shell);
			if (i == 0)
				exec->command = exec->argv[0];
		}
		i++;
	}
}

/* Handle empty commands after expansion by shifting arguments */
void	handle_empty_command(t_exec *exec)
{
	int	i;

	if (!exec->argv || !exec->argv[0])
		return ;
	if (!*exec->argv[0] || ft_is_whitespace_str(exec->argv[0]))
	{
		i = 0;
		while (exec->argv[i + 1])
		{
			exec->argv[i] = exec->argv[i + 1];
			if (exec->no_expand_flags)
				exec->no_expand_flags[i] = exec->no_expand_flags[i + 1];
			i++;
		}
		exec->argv[i] = NULL;
		if (exec->argv[0])
			exec->command = exec->argv[0];
		else
			exec->command = NULL;
	}
}
