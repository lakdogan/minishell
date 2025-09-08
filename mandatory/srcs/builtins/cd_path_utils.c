/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:55:02 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 18:46:25 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

/* Handle the case when no argument is provided (cd with no args) */
static char	*handle_no_arg_case(void)
{
	char	*path;

	path = getenv("HOME");
	if (!path)
	{
		write(STDERR_FILENO, "cd: HOME not set\n", 18);
		return (NULL);
	}
	return (path);
}

/* Handle the tilde case (cd ~) */
static char	*handle_tilde_case(void)
{
	char	*path;

	path = getenv("HOME");
	if (!path)
	{
		write(STDERR_FILENO, "cd: HOME not set\n", 18);
		return (NULL);
	}
	return (path);
}

/* Validate the provided path */
static int	is_valid_path(char *path)
{
	if (!path)
		return (0);
	if ((uintptr_t)path < 0x1000)
		return (0);
	return (1);
}

/* Determine the target path for cd command based on arguments */
char	*get_cd_target_path(char **argv)
{
	char	*path;

	if (!argv[COMMAND_ARGS_START])
		return (handle_no_arg_case());
	else if (ft_strcmp(argv[COMMAND_ARGS_START], "~") == 0)
		return (handle_tilde_case());
	else
	{
		path = argv[COMMAND_ARGS_START];
		if (!is_valid_path(path))
			return (NULL);
	}
	return (path);
}
