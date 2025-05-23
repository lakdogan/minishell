/**
 * @file unset.c
 * @brief Implementation of the unset built-in command
 *
 * This file contains the implementation of the unset built-in command which
 * removes environment variables from the shell environment. It handles
 * validation of variable names and proper memory cleanup during removal.
 */

 #include "../../includes/core/minishell.h"

 /**
  * @brief Validates if a string is a valid environment variable identifier
  *
  * Checks if the provided string meets the requirements for a valid shell
  * variable name:
  * - Must not be empty
  * - First character must be a letter or underscore (not a digit)
  * - All characters must be alphanumeric or underscore
  *
  * @param str The string to validate
  * @return int 1 if the string is a valid identifier, 0 otherwise
  */
 static int	is_valid_identifier(const char *str)
 {
     int	i;
 
     if (!str || !str[0] || ft_isdigit(str[0]))
         return (0);
     i = 0;
     while (str[i])
     {
         if (!ft_isalnum(str[i]) && str[i] != '_')
             return (0);
         i++;
     }
     return (1);
 }
 
 /**
  * @brief Frees memory allocated for an environment variable and its list node
  *
  * Properly deallocates all memory associated with an environment variable
  * including its value, content string, and the node itself.
  *
  * @param env Pointer to the environment variable structure to free
  * @param node Pointer to the list node containing the environment variable
  */
 static void	free_node(t_env *env, t_list *node)
 {
     free(env->value);
     free(env->content);
     free(env);
     free(node);
 }
 
 /**
  * @brief Removes an environment variable from the shell
  *
  * Searches for an environment variable by name and removes it from the
  * shell's environment list if found, freeing all associated memory.
  *
  * @param name Name of the environment variable to remove
  * @param minishell Pointer to the shell state structure
  * @return int Always returns 0 (success)
  */
 static int	remove_env_var(const char *name, t_minishell *minishell)
 {
     t_list	*prev;
     t_list	*node;
     t_env	*env;
 
     prev = NULL;
     node = minishell->envp;
     while (node)
     {
         env = (t_env *)node->content;
         if (env && ft_strncmp(env->value, name, SIZE_MAX) == 0)
         {
             if (prev)
                 prev->next = node->next;
             else
                 minishell->envp = node->next;
             free_node(env, node);
             return (0);
         }
         prev = node;
         node = node->next;
     }
     return (0);
 }
 
 /**
  * @brief Implements the unset built-in command
  *
  * Removes one or more environment variables from the shell environment.
  * For each argument, validates that it's a valid identifier and then
  * attempts to remove it from the environment. Displays an error message
  * for invalid identifiers.
  *
  * @param argv Array of command arguments (argv[0] is "unset")
  * @param minishell Pointer to the shell state structure
  * @return int Always returns 0 (success)
  */
 int	ft_unset(char **argv, t_minishell *minishell)
 {
     int	i;
 
     i = 1;
     while (argv[i])
     {
         if (!is_valid_identifier(argv[i]))
         {
             write(STDERR_FILENO, "unset: `", 8);
             write(STDERR_FILENO, argv[i], ft_strlen(argv[i]));
             write(STDERR_FILENO, "': not a valid identifier\n", 27);
         }
         else
             remove_env_var(argv[i], minishell);
         i++;
     }
     return (0);
 }
