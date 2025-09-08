/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore_escaped_dollars.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 23:33:55 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:38:56 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Appends a single character to result string.
static char	*append_single_char(t_minishell *shell, char *res, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (gc_strjoin(shell->gc[GC_COMMAND], res, tmp));
}

// Processes escape character for dollar restoration.
static char	*process_escape_char(t_minishell *shell, char *res, char c)
{
	if (c == '\1')
		return (gc_strjoin(shell->gc[GC_COMMAND], res, "$"));
	return (append_single_char(shell, res, c));
}

// Restores escaped dollars in string.
char	*restore_escaped_dollars(char *s, t_minishell *shell)
{
	size_t	i;
	char	*res;

	if (!s)
		return (gc_strdup(shell->gc[GC_COMMAND], ""));
	i = 0;
	res = gc_strdup(shell->gc[GC_COMMAND], "");
	while (s[i])
	{
		res = process_escape_char(shell, res, s[i]);
		i++;
	}
	return (res);
}
