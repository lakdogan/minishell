/**
 * @file cd_bonus.c
 * @brief Implementation of the cd (change directory) built-in command
 *
 * This file contains the implementation of the cd built-in command which
 * changes the current working directory of the shell. It supports changing
 * to a specified directory or to the user's home directory when no argument
 * is provided.
 */

#include "../../includes_bonus/core_bonus/minishell_bonus.h"

 /**
  * @brief Changes the current working directory
  *
  * Implements the built-in cd command which changes the shell's current
  * working directory. If no path is specified (argv[1] is NULL), it attempts
  * to change to the user's home directory as specified by the HOME environment
  * variable. If a path is specified, it attempts to change to that directory.
  * After changing the directory, it updates the shell's cwd variable.
  *
  * @param argv Array of command arguments (argv[0] is "cd",
  *        argv[1] is the target path)
  * @param minishell Pointer to the shell state structure
  * @return int 0 on success, 1 on failure (directory not found or HOME not set)
  */
 int	ft_cd(char **argv, t_minishell *minishell)
 {
     char	*path;
 
     if (!argv[1])
     {
         path = getenv("HOME");
         if (!path)
         {
             write(STDERR_FILENO, "cd: HOME not set\n", 18);
             return (1);
         }
     }
     else
         path = argv[1];
     if (chdir(path) != 0)
     {
         perror("cd");
         return (1);
     }
     free(minishell->cwd);
     minishell->cwd = getcwd(NULL, 0);
     return (0);
 }
