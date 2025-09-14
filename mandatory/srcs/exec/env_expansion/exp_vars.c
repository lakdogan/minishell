/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:20:29 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 06:01:05 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Expands a single variable from the string at position pos.
static int	process_variable(const char *str, int pos, char **result,
		t_minishell *shell)
{
	char	*expanded;
	int		consumed;

	expanded = NULL;
	consumed = expand_single_var(&str[pos], &expanded, shell);
	*result = gc_strjoin(shell->gc[GC_EXPAND], *result, expanded);
	return (consumed);
}

// Expands tilde (~) to home directory if present.
char	*expand_tilde(const char *arg, t_minishell *shell)
{
	char	*home;
	size_t	len;
	char	*expanded;

	if (arg[0] == '~' && (arg[1] == '\0' || arg[1] == '/'))
	{
		home = find_env_value(shell, "HOME");
		if (!home)
			return (gc_strdup(shell->gc[GC_COMMAND], arg));
		len = ft_strlen(home) + ft_strlen(arg);
		expanded = gc_malloc(shell->gc[GC_COMMAND], len);
		ft_strlcpy(expanded, home, ft_strlen(home) + 1);
		ft_strlcat(expanded, arg + 1, len);
		return (expanded);
	}
	return (gc_strdup(shell->gc[GC_COMMAND], arg));
}

static void	handle_variable_or_dollar(const char *str, int *i, char **result,
		t_minishell *shell)
{
	int	consumed;

	if (is_variable_reference(str, *i))
	{
		consumed = process_variable(str, *i, result, shell);
		*i += consumed;
	}
	else if (str[*i] == DOLLAR_SIGN)
	{
		process_regular_char(str[*i], result, shell);
		(*i)++;
	}
}

static void	handle_regular_char(const char *str, int *i, char **result,
		t_minishell *shell)
{
	process_regular_char(str[*i], result, shell);
	(*i)++;
}

// Expands variables in a string using the shell environment.
char	*expand_variables(const char *str, t_minishell *shell)
{
	char	*result;
	int		i;

	if (!str || (uintptr_t)str < 0x1000)
		return (NULL);
	result = expand_tilde_if_needed(str, shell);
	if (result)
		return (result);
	result = gc_strdup(shell->gc[GC_EXPAND], EMPTY_STRING);
	i = 0;
	while (str[i])
	{
		if (is_variable_reference(str, i) || str[i] == DOLLAR_SIGN)
			handle_variable_or_dollar(str, &i, &result, shell);
		else
			handle_regular_char(str, &i, &result, shell);
	}
	return (result);
}
