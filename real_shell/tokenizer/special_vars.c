/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 23:50:20 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/03 23:52:35 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

int	handle_exit_status(const char *input, size_t *i, t_minishell *shell,
		char **buf)
{
	char	*exit_str;

	if (input[*i] != '?')
		return (0);
	exit_str = int_to_str(shell->exit_code, shell);
	*buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, exit_str);
	(*i)++;
	return (1);
}

int	handle_pid_var(const char *input, size_t *i, t_minishell *shell, char **buf)
{
	int		pid;
	char	*pid_str;

	if (input[*i] != '$')
		return (0);
	pid = 42;
	pid_str = int_to_str(pid, shell);
	*buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, pid_str);
	(*i)++;
	return (1);
}

int	process_special_var(const char *input, size_t *i, t_minishell *shell,
		char **buf)
{
	if (handle_exit_status(input, i, shell, buf))
		return (1);
	if (handle_pid_var(input, i, shell, buf))
		return (1);
	return (0);
}
