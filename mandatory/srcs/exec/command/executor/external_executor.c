/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:55:07 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 20:01:40 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Handle variable expansion in execution context */
void	handle_var_expansion_exec(t_minishell *shell, t_exec *exec)
{
	expand_command_args(shell, exec);
	handle_empty_command(exec);
	expand_infile_names(shell, exec);
	expand_outfile_names(shell, exec);
}

/* Main command execution function */
void	execute_command(t_exec *exec, t_minishell *minishell)
{
	handle_special_cases(exec, minishell);
	if (ft_strchr(exec->command, '/'))
		execute_with_path(exec->command, exec, minishell);
	else
		execute_with_path_lookup(exec, minishell);
}
