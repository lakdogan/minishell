/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:14:37 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:09:40 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Handles execution of a pipe node.
void	handle_pipe(t_minishell *shell, t_command_tree *node)
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
