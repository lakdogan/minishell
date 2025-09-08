/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_in_subshell_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:20:00 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:32:23 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/minishell_bonus.h"

// Handles pipe execution inside a subshell.
void	handle_pipe_in_subshell(t_minishell *shell, t_command_tree *node)
{
	t_exec	**execs;
	int		num_cmds;

	execs = build_exec_array_from_tree(node);
	if (!execs)
		return ;
	num_cmds = 0;
	while (execs[num_cmds])
		num_cmds++;
	execute_pipe_chain(execs, num_cmds, shell);
	free(execs);
}
