/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 20:44:16 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 20:49:42 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

void	exec_external_command(t_exec *exec, t_minishell *shell)
{
	char	**envp_arr;

	envp_arr = list_to_envp_array(shell->gc[GC_MAIN], shell->envp);
	ft_execvp(exec->command, exec->argv, envp_arr);
	perror("minishell");
	exit(127);
}

// Main function: Find file in PATH
char	*find_in_path(const char *file, char **envp)
{
	char	*paths;
	char	*slash_cmd;
	char	*candidate;

	paths = get_path_value(envp);
	if (!paths)
		return (NULL);
	slash_cmd = ft_strjoin("/", file);
	if (!slash_cmd)
		return (NULL);
	candidate = search_path_dirs(paths, slash_cmd);
	free(slash_cmd);
	return (candidate);
}

int	ft_execvp(const char *file, char *const argv[], char *const envp[])
{
	char	*path;

	if (ft_strchr(file, '/'))
	{
		execve(file, argv, envp);
		return (-1);
	}
	path = find_in_path(file, (char **)envp);
	if (path)
	{
		execve(path, argv, envp);
		free(path);
	}
	return (-1);
}
