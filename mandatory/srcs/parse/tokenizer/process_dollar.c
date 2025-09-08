/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_dollar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 23:48:13 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:38:32 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Expands environment variable value.
static char	*expand_variable(const char *var, t_minishell *shell)
{
	char	*value;

	value = get_env_var(shell, var);
	if (!value)
		return (gc_strdup(shell->gc[GC_COMMAND], ""));
	return (gc_strdup(shell->gc[GC_COMMAND], value));
}

// Extracts variable name from input.
static char	*extract_var_name_local(const char *input, size_t *i,
		t_minishell *shell)
{
	size_t	var_start;
	size_t	var_len;

	var_start = *i;
	while (input[*i] && is_var_char(input[*i]))
		(*i)++;
	var_len = *i - var_start;
	return (gc_substr(shell->gc[GC_COMMAND], input, var_start, var_len));
}

// Processes $ for variable expansion.
void	process_dollar(const char *input, size_t *i, t_minishell *shell,
		char **buf)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (process_special_var(input, i, shell, buf))
		return ;
	var_name = extract_var_name_local(input, i, shell);
	var_value = expand_variable(var_name, shell);
	*buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, var_value);
}
