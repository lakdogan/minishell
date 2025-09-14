/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:37:16 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 07:16:52 by lakdogan         ###   ########.fr       */
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

static int	handle_cd_args(char **argv)
{
	int	argc;

	argc = 0;
	while (argv[COMMAND_ARGS_START + argc])
		argc++;
	if (argv[COMMAND_ARGS_START] && ft_strcmp(argv[COMMAND_ARGS_START],
			"-") == 0)
		return (-1);
	if (argv[COMMAND_ARGS_START] && ft_strcmp(argv[COMMAND_ARGS_START],
			"--") == 0 && !argv[COMMAND_ARGS_START + 1])
	{
		argv[COMMAND_ARGS_START] = NULL;
		argc = 0;
	}
	return (argc);
}

static int	cd_change_and_update(char **argv, t_minishell *minishell)
{
	char	*path;
	char	*old_pwd;

	old_pwd = NULL;
	lookup_env_value("PWD", minishell, &old_pwd);
	path = get_cd_target_path(argv, minishell);
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

int	ft_cd(char **argv, t_minishell *minishell)
{
	int	argc;

	argc = handle_cd_args(argv);
	if (argc == -1)
		return (handle_cd_dash(minishell));
	return (cd_change_and_update(argv, minishell));
}
