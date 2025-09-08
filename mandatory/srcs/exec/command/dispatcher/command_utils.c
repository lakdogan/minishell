/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:38:12 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 19:44:46 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Count the number of arguments in argv array */
int	count_args(char **argv)
{
	int	count;

	count = 0;
	if (!argv)
		return (0);
	while (argv[count])
		count++;
	return (count);
}

/* Apply special preprocessing to commands as needed */
void	preprocess_command(t_exec *exec, t_minishell *shell)
{
	if (exec && exec->command && exec->argv && ft_strcmp(exec->command,
			"cd") == STRINGS_EQUAL && exec->argv[COMMAND_ARGS_START])
	{
		if (ft_strcmp(exec->argv[COMMAND_ARGS_START], "$PWD") == STRINGS_EQUAL)
		{
			exec->no_expand_flags = gc_malloc(shell->gc[GC_COMMAND], sizeof(int)
					* (count_args(exec->argv) + 1));
			if (exec->no_expand_flags)
				exec->no_expand_flags[COMMAND_ARGS_START] = 1;
		}
	}
}

/* Setup command execution environment */
void	prepare_command_execution(t_minishell *shell, t_exec *exec)
{
	preprocess_command(exec, shell);
	setup_child_signals();
	setup_input_redirections(shell, exec, true);
	setup_output_redirections(shell, exec);
}
