/**
 * @file export_utils.c
 * @brief Utility functions for the export built-in command
 *
 * This file contains helper functions used by the export built-in command
 * to manipulate environment variables, including extracting keys and values
 * from arguments and adding or updating environment variables.
 */

 #include "../../includes/core/minishell.h"

 /**
  * @brief Extracts the key (name) part from an environment variable string
  *
  * Parses an environment variable string in the format "KEY=VALUE" or "KEY"
  * and returns only the KEY portion.
  *
  * @param arg String containing the environment variable assignment
  * @return char* Extracted key (caller must free)
  */
 static char	*extract_key(const char *arg)
 {
     int	len;
 
     len = 0;
     while (arg[len] && arg[len] != '=')
         len++;
     return (ft_substr(arg, 0, len));
 }
 
 /**
  * @brief Extracts the value part from an environment variable string
  *
  * Parses an environment variable string in the format "KEY=VALUE" and
  * returns only the VALUE portion. If no equals sign is present, returns NULL.
  *
  * @param arg String containing the environment variable assignment
  * @return char* Extracted value or NULL if no equals sign (caller must free)
  */
 static char	*extract_value(const char *arg)
 {
     char	*equal;
 
     equal = ft_strchr(arg, '=');
     if (!equal)
         return (NULL);
     return (ft_strdup(equal + 1));
 }
 
 /**
  * @brief Adds a new environment variable to the shell
  *
  * Creates a new environment variable with the given key and value
  * and adds it to the shell's environment variable list. The variable
  * is automatically marked for export.
  *
  * @param key The name of the environment variable
  * @param value The value of the environment variable (can be NULL)
  * @param minishell Pointer to the shell state structure
  */
 void	add_new_env(char *key, char *value, t_minishell *minishell)
 {
     t_env	*env;
 
     env = malloc(sizeof(t_env));
     env->value = ft_strdup(key);
     if (value)
         env->content = ft_strjoin_3(key, "=", value);
     else
         env->content = NULL;
     env->is_export = true;
     env->printed = false;
     ft_lstadd_back(&minishell->envp, ft_lstnew(env));
     free(key);
     free(value);
 }
 
 /**
  * @brief Updates an existing environment variable
  *
  * Updates the value of an existing environment variable and marks it
  * for export. If the provided value is NULL, only marks the variable
  * for export without changing its value.
  *
  * @param env Pointer to the environment variable to update
  * @param key The name of the environment variable (will be freed)
  * @param value The new value for the variable (will be freed, can be NULL)
  */
 void	update_existing_env(t_env *env, char *key, char *value)
 {
     if (value)
     {
         free(env->content);
         free(env->value);
         env->value = ft_strdup(key);
         env->content = ft_strjoin_3(key, "=", value);
     }
     env->is_export = true;
     free(key);
     free(value);
 }
 
 /**
  * @brief Updates an existing variable or adds a new one if it doesn't exist
  *
  * Parses an environment variable assignment string, finds if the variable
  * already exists, and either updates it or adds a new one as appropriate.
  *
  * @param arg String in the format "KEY=VALUE" or "KEY"
  * @param minishell Pointer to the shell state structure
  */
 void	update_or_add_env(const char *arg, t_minishell *minishell)
 {
     t_list	*node;
     t_env	*env;
     char	*key;
     char	*value;
 
     node = minishell->envp;
     key = extract_key(arg);
     value = extract_value(arg);
     while (node)
     {
         env = (t_env *)node->content;
         if (ft_strncmp(env->value, key, SIZE_MAX) == 0)
         {
             update_existing_env(env, key, value);
             return ;
         }
         node = node->next;
     }
     add_new_env(key, value, minishell);
 }
