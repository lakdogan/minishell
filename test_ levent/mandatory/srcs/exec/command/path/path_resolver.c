/**
 * @file path_resolver.c
 * @brief Command path resolution functionality
 *
 * This file contains functions for resolving command paths in the shell.
 * It handles both absolute and relative paths, as well as searching for
 * commands in the directories specified by the PATH environment variable.
 */

#include "../../../../includes/core/minishell.h"

/**
 * @brief Checks if the command is already an absolute path
 *
 * Determines if the provided command string is already a valid absolute
 * or relative path that can be executed directly. Verifies both existence
 * and execution permissions.
 *
 * @param cmd The command to check
 * @return char* Duplicated command string if it's a
 * valid path, NULL otherwise
 * @note Sets errno to EACCES if the command
 * exists but lacks execute permission
 */
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

/**
 * @brief Finds the PATH environment variable
 *
 * Searches through the environment variables to locate the PATH variable
 * which contains the directories to search for executable files.
 *
 * @param envp Array of environment variable strings
 * @return char* Pointer to the PATH value (without "PATH="), NULL if not found
 */
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

/**
 * @brief Searches for a command in the provided paths
 *
 * Iterates through each directory in the paths array, checking if the
 * command exists and is executable in that location.
 *
 * @param paths NULL-terminated array of directory paths to search
 * @param cmd Command name to find
 * @return char* Full path to the command if
 * found (caller must free), NULL otherwise
 */
static char	*find_in_path(t_minishell *shell, char **paths, char *cmd)
{
	int		i;
	char	*path_with_slash;
	char	*full_path;
	char	*result;

	i = 0;
	result = NULL;
	while (paths[i])
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

/**
 * @brief Resolves the absolute path of a command
 *
 * Main function that determines the absolute path of a command by:
 * 1. Checking if it's already an absolute/relative path
 * 2. If not, searching in directories specified by
 * the PATH environment variable
 *
 * @param cmd Command name or path to resolve
 * @param envp Array of environment variable strings
 * @return char* Absolute path to the command if found
 * (caller must free), NULL otherwise
 */
char	*get_absolute_path(t_minishell *shell, char *cmd, char **envp)
{
	char	*abs_path;
	char	**paths;
	char	*path_env;
	char	*result;

	abs_path = check_absolute_path(shell, cmd);
	if (abs_path)
		return (abs_path);
	path_env = find_path_env(envp);
	if (!path_env)
		return (NULL);
	paths = gc_split(shell->gc[GC_TEMP], path_env, PATH_DELIMITER);
	result = find_in_path(shell, paths, cmd);
	return (result);
}
