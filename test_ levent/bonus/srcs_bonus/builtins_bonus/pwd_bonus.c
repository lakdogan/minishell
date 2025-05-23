/**
 * @file pwd_bonus.c
 * @brief Implementation of the pwd built-in command
 *
 * This file contains the implementation of the pwd (print working directory)
 * built-in command, which displays the current working directory path to
 * standard output.
 */

#include "../../includes_bonus/core_bonus/minishell_bonus.h"

 /**
  * @brief Prints the current working directory
  *
  * Implements the pwd built-in command by retrieving the current working
  * directory using getcwd() and printing it to standard output. If the
  * current directory cannot be determined, an error message is printed.
  *
  * @return int 0 on success, 1 if the current directory could not be determined
  */
 int	ft_pwd(void)
 {
     char	cwd[PATH_MAX];
     int		len;
 
     if (getcwd(cwd, sizeof(cwd)) != NULL)
     {
         len = ft_strlen(cwd);
         write(STDOUT_FILENO, cwd, len);
         write(STDOUT_FILENO, "\n", 1);
         return (0);
     }
     else
     {
         perror("pwd");
         return (1);
     }
 }
