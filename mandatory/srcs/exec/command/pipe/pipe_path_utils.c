/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_path_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 20:46:44 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 20:49:12 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

// Helper: Find the value of PATH in envp
char	*get_path_value(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

// Helper: Join a directory and filename into a full path
char	*join_dir_and_file(const char *dir, const char *slash_cmd)
{
	char	*path;

	path = ft_strjoin(dir, slash_cmd);
	return (path);
}

char	*try_path_dir(const char *dir, const char *slash_cmd)
{
	char	*candidate;

	candidate = join_dir_and_file(dir, slash_cmd);
	if (candidate && access(candidate, X_OK) == 0)
		return (candidate);
	free(candidate);
	return (NULL);
}

char	*search_path_dirs(const char *paths, const char *slash_cmd)
{
	int		i;
	int		start;
	int		end;
	char	*dir;
	char	*candidate;

	i = 0;
	while (paths[i])
	{
		start = i;
		while (paths[i] && paths[i] != ':')
			i++;
		end = i;
		dir = ft_substr(paths, start, end - start);
		if (dir)
		{
			candidate = try_path_dir(dir, slash_cmd);
			free(dir);
			if (candidate)
				return (candidate);
		}
		if (paths[i] == ':')
			i++;
	}
	return (NULL);
}
