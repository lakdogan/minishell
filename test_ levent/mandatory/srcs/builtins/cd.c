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

int	ft_cd(char **argv, t_minishell *minishell)
{
	char	*path;
	char	*temp_cwd;

	path = get_cd_target_path(argv);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	temp_cwd = getcwd(NULL, 0);
	if (temp_cwd)
	{
		gc_collect(minishell->gc[GC_CWD]);
		gc_register(minishell->gc[GC_CWD], temp_cwd, ft_strlen(temp_cwd));
		minishell->cwd = temp_cwd;
	}
	return (0);
}
