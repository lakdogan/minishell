/**
 * @file env_bonus.c
 * @brief Implementation of the env built-in command
 *
 * This file contains the implementation of the env built-in command which
 * displays all environment variables currently defined in the shell.
 * It prints each variable in the format "NAME=VALUE".
 */

#include "../../includes_bonus/core_bonus/minishell_bonus.h"

 /**
  * @brief Displays all environment variables
  *
  * Implements the built-in env command which prints all environment variables
  * that are marked for export and have a defined value. Each variable is printed
  * on a separate line in the format "NAME=VALUE".
  *
  * @param minishell Pointer to the shell state containing environment variables
  * @return int Always returns 0 (success)
  */
 int	ft_env(t_minishell *minishell)
 {
     t_list	*node;
     t_env	*env;
 
     node = minishell->envp;
     while (node)
     {
         env = (t_env *)node->content;
         if (env->is_export && env->content)
             printf("%s\n", env->content);
         node = node->next;
     }
     return (0);
 }
