/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:20:29 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:00:49 by lakdogan         ###   ########.fr       */
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

// Appends a regular character to the result buffer.
static void	process_regular_char(char c, char **result, t_minishell *shell)
{
	char	tmp[CHAR_BUFFER_SIZE];

	tmp[FIRST_CHAR] = c;
	tmp[NULL_TERMINATOR_INDEX] = NULL_TERMINATOR;
	*result = gc_strjoin(shell->gc[GC_EXPAND], *result, tmp);
}

// Checks if the current position in the string is a variable reference.
static bool	is_variable_reference(const char *str, int i)
{
	return (str[i] == DOLLAR_SIGN && str[i + NEXT_CHAR_INDEX]
		&& (is_var_char(str[i + NEXT_CHAR_INDEX]) || str[i
				+ NEXT_CHAR_INDEX] == QUESTION_MARK || str[i
				+ NEXT_CHAR_INDEX] == DOLLAR_SIGN));
}

// Expands variables in a string using the shell environment.
char	*expand_variables(const char *str, t_minishell *shell)
{
	char	*result;
	int		i;
	int		consumed;

	if (str && str[0] == '~' && (str[1] == '\0' || str[1] == '/'))
		return (gc_strdup(shell->gc[GC_COMMAND], str));
	if (!str || (uintptr_t)str < 0x1000)
		return (NULL);
	result = gc_strdup(shell->gc[GC_EXPAND], EMPTY_STRING);
	i = 0;
	while (str[i])
	{
		if (is_variable_reference(str, i))
		{
			consumed = process_variable(str, i, &result, shell);
			i += consumed;
		}
		else
		{
			process_regular_char(str[i], &result, shell);
			i++;
		}
	}
	return (result);
}

// Expands tilde (~) to home directory if present.
char	*expand_tilde(const char *arg, t_minishell *shell)
{
	char	*home;
	size_t	len;
	char	*expanded;

	if (arg[0] == '~' && (arg[1] == '\0' || arg[1] == '/'))
	{
		home = getenv("HOME");
		if (!home)
			return (gc_strdup(shell->gc[GC_COMMAND], arg));
		len = ft_strlen(home) + ft_strlen(arg);
		expanded = gc_malloc(shell->gc[GC_COMMAND], len);
		strcpy(expanded, home);
		strcat(expanded, arg + 1);
		return (expanded);
	}
	return (gc_strdup(shell->gc[GC_COMMAND], arg));
}
