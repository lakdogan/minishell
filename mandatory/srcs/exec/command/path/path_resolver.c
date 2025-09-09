/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:04:58 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 20:53:35 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Checks if the command is an absolute or relative path.
static char	*check_absolute_path(t_minishell *shell, char *cmd)
{
	if (cmd[0] == ABSOLUTE_PATH_CHAR || (cmd[0] == RELATIVE_PATH_CHAR
			&& cmd[1] == PATH_SEPERATOR))
	{
		if (access(cmd, F_OK) == ACCESS_OK)
		{
			if (access(cmd, X_OK) == ACCESS_OK)
				return (gc_strdup(shell->gc[GC_COMMAND], cmd));
			else
			{
				errno = EACCES;
				return (gc_strdup(shell->gc[GC_COMMAND], cmd));
			}
		}
		return (NULL);
	}
	return (NULL);
}

// Finds the PATH variable in the environment.
static char	*find_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == STRINGS_EQUAL)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

// Searches for the command in the PATH directories.
static char	*find_in_path_pr(t_minishell *shell, char **paths, char *cmd)
{
	int		i;
	char	*path_with_slash;
	char	*full_path;
	char	*result;

	i = 0;
	result = NULL;
	while (paths && paths[i])
	{
		path_with_slash = gc_strjoin(shell->gc[GC_TEMP], paths[i], "/");
		if (!path_with_slash)
			return (NULL);
		full_path = gc_strjoin(shell->gc[GC_TEMP], path_with_slash, cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == ACCESS_OK)
		{
			result = gc_strdup(shell->gc[GC_COMMAND], full_path);
			gc_collect(shell->gc[GC_TEMP]);
			return (result);
		}
		i++;
	}
	gc_collect(shell->gc[GC_TEMP]);
	return (result);
}

// Gets the absolute path for a command.
char	*get_absolute_path(t_minishell *shell, char *cmd, char **envp)
{
	char	*abs_path;
	char	**paths;
	char	*path_env;
	char	*result;

	if (!cmd || (uintptr_t)cmd < 0x1000)
		return (NULL);
	abs_path = check_absolute_path(shell, cmd);
	if (abs_path)
		return (abs_path);
	path_env = find_path_env(envp);
	if (!path_env)
		return (NULL);
	paths = gc_split(shell->gc[GC_TEMP], path_env, PATH_DELIMITER);
	result = find_in_path_pr(shell, paths, cmd);
	return (result);
}
