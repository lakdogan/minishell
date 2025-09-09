/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:31:40 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 01:15:50 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/core/minishell.h"

// Main entry point for minishell program.
int	main(int argc, char **argv, char **env)
{
	t_minishell	shell;
	int			dir_check;

	shell.shell_pid = 42;
	dir_check = check_directory_argument(argc, argv);
	if (dir_check != 0)
		return (dir_check);
	init_minishell(&shell, env);
	if (!increment_shlvl(&shell))
		return (1);
	setup_parent_signals();
	shell_loop(&shell, argc, argv);
	gc_cleanup(shell.gc);
	return (shell.exit_code);
}
