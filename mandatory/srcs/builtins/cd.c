/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:37:16 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:19:45 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Changes the current directory.
static int	change_directory(const char *path)
{
	if (chdir(path) != SYSCALL_SUCCESS)
	{
		perror("cd");
		return (BUILTIN_FAILURE);
	}
	return (BUILTIN_SUCCESS);
}

// Updates the current directory in minishell.
static int	update_current_directory(t_minishell *minishell)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("cd");
		return (BUILTIN_FAILURE);
	}
	update_pwd_variables(minishell, cwd, minishell->cwd);
	return (BUILTIN_SUCCESS);
}

/* Main cd command implementation */
int	ft_cd(char **argv, t_minishell *minishell)
{
	int		argc;
	char	*path;
	char	*old_pwd;

	argc = 0;
	while (argv[COMMAND_ARGS_START + argc])
		argc++;
	if (argv[COMMAND_ARGS_START] && ft_strcmp(argv[COMMAND_ARGS_START],
			"-") == 0)
		return (handle_cd_dash(minishell));
	old_pwd = NULL;
	lookup_env_value("PWD", minishell, &old_pwd);
	path = get_cd_target_path(argv);
	if (!path)
		return (BUILTIN_FAILURE);
	if (!*path)
	{
		write(STDERR_FILENO, "cd: path cannot be empty\n", 25);
		return (BUILTIN_FAILURE);
	}
	if (change_directory(path) != BUILTIN_SUCCESS)
		return (BUILTIN_FAILURE);
	return (update_current_directory(minishell));
}
