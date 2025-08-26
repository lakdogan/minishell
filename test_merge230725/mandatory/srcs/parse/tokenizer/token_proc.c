/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_proc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 13:21:45 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/26 13:40:03 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

static void	process_dollar(const char *input, size_t *i,
			t_minishell *shell, char **buf)
{
	char	buffer[12];
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*var_value;

	(*i)++; // skip '$'
	if (input[*i] == '?')
	{
		snprintf(buffer, sizeof(buffer), "%d", shell->exit_code);
		*buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, buffer);
		(*i)++;
	}
	else
	{
		var_start = *i;
		while (input[*i] && (isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;
		var_len = *i - var_start;
		var_name = gc_substr(shell->gc[GC_COMMAND], input, var_start, var_len);
		var_value = expand_variable(var_name, shell);
		*buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, var_value);
	}
}

static void	process_bslashes(const char *input, size_t *i,
			t_minishell *shell, char **buf)
{

}

static char	*proc_single(const char *input, size_t *i, t_minishell *shell)
{
	size_t	start;
	char	*seg;

	(*i)++; // skip opening '
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	seg = gc_substr(shell->gc[GC_COMMAND], input, start, *i - start);
	if (input[*i] == '\'')
		(*i)++; // skip closing '
	return (seg);
}

static char	*proc_double(const char *input, size_t *i, t_minishell *shell)
{
	char *res;

	res = gc_strdup(shell->gc[GC_COMMAND], "");
	(*i)++; // skip opening "
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '\\')
		{
			process_bslashes(input, i, shell, &res);
			continue;
		}
		if (input[*i] == '$')
		{
			process_dollar(input, i, shell, &res);
			continue;
		}
		{
			char tmp[2] = { input[*i], '\0' };
			res = gc_strjoin(shell->gc[GC_COMMAND], res, tmp);
			(*i)++;
		}
	}
	if (input[*i] == '"')
		(*i)++; // skip closing "
	return (res);
}

static char	*proc_unquoted(const char *input, size_t *i, t_minishell *shell)
{
	char *res;

	res = gc_strdup(shell->gc[GC_COMMAND], "");
	while (input[*i] && input[*i] != '\'' && input[*i] != '"')
	{
		if (input[*i] == '\\')
		{
			process_bslashes(input, i, shell, &res);
			continue;
		}
		if (input[*i] == '$')
		{
			process_dollar(input, i, shell, &res);
			continue;
		}
		{
			char tmp[2] = { input[*i], '\0' };
			res = gc_strjoin(shell->gc[GC_COMMAND], res, tmp);
			(*i)++;
		}
	}
	return (res);
}
