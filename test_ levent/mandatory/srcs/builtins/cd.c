/**
 * @file cd.c
 * @brief Implementation of the cd (change directory) built-in command
 *
 * This file contains the implementation of the cd built-in command which
 * changes the current working directory of the shell. It supports changing
 * to a specified directory or to the user's home directory when no argument
 * is provided.
 */

#include "../../includes/core/minishell.h"

/**
 * @brief Ermittelt den Zielpfad für den cd-Befehl
 *
 * Wenn kein Pfad angegeben ist, wird das HOME-Verzeichnis verwendet.
 * Andernfalls wird der angegebene Pfad zurückgegeben.
 *
 * @param argv Array der Befehlsargumente
 * @return char* Zielpfad oder NULL bei Fehler
 */
static char	*get_cd_target_path(char **argv)
{
	char	*path;

	if (!argv[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			write(STDERR_FILENO, "cd: HOME not set\n", 18);
			return (NULL);
		}
	}
	else
		path = argv[1];
	return (path);
}

/**
 * @brief Changes to the target directory
 *
 * @param path Path to change to
 * @return int 0 on success, 1 on failure
 */
static int	change_directory(const char *path)
{
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

/**
 * @brief Updates the current working directory in minishell struct
 *
 * @param minishell Pointer to the shell state structure
 * @return int 0 on success, 1 on failure
 */
static int	update_current_directory(t_minishell *minishell)
{
	char	*temp_cwd;

	temp_cwd = getcwd(NULL, 0);
	if (!temp_cwd)
		return (1);
	gc_collect(minishell->gc[GC_CWD]);
	gc_register(minishell->gc[GC_CWD], temp_cwd, ft_strlen(temp_cwd));
	minishell->cwd = temp_cwd;
	return (0);
}

/**
 * @brief Updates PWD and OLDPWD environment variables
 *
 * @param minishell Pointer to the shell state structure
 * @param new_pwd New PWD value
 * @param old_pwd Old PWD value
 */
static void	update_pwd_variables(t_minishell *minishell, const char *new_pwd,
		const char *old_pwd)
{
	set_env_var("PWD", new_pwd, minishell);
	if (old_pwd)
		set_env_var("OLDPWD", old_pwd, minishell);
	minishell->envp_arr = rebuild_env_array(minishell);
}

/**
 * @brief Implements the cd built-in command
 *
 * Changes the current working directory and updates related environment
 * variables (PWD and OLDPWD).
 *
 * @param argv Array of command arguments (argv[0] is "cd")
 * @param minishell Pointer to the shell state structure
 * @return int 0 on success, 1 on failure
 */
int	ft_cd(char **argv, t_minishell *minishell)
{
	char	*path;
	char	*old_pwd;

	old_pwd = NULL;
	get_env_value("PWD", minishell, &old_pwd);
	path = get_cd_target_path(argv);
	if (!path)
		return (1);
	if (change_directory(path) != 0)
		return (1);
	if (update_current_directory(minishell) != 0)
		return (1);
	update_pwd_variables(minishell, minishell->cwd, old_pwd);
	return (0);
}
