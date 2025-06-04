/**
 * @file path_utils.c
 * @brief Utility functions for handling PATH environment variable
 *
 * This file contains utility functions for extracting and processing the
 * PATH environment variable, which is used to locate executable files
 * during command resolution in the shell.
 */

 #include "../../../includes/core/minishell.h"

 /**
  * @brief Extracts and parses the PATH environment variable
  *
  * Searches through the environment variables array for the PATH variable,
  * and splits it into individual directory paths using the colon separator.
  * The resulting array is stored in the provided output parameter.
  *
  * @param envp NULL-terminated array of environment variable strings
  * @param paths Pointer to a string array that will store the parsed paths
  * @return int OPERATION_SUCCESS if the PATH was found and parsed successfully,
  *             OPERATION_FAILURE if the PATH variable was not found
  * @note The caller is responsible for freeing the memory allocated for *paths
  */
 int	get_path_from_env(t_minishell *shell, char **envp, char ***paths)
 {
     int	i;
 
     i = 0;
     while (envp[i])
     {
         if (ft_strncmp(envp[i], "PATH=", 5) == STRINGS_EQUAL)
         {
             *paths = gc_split(shell->gc[GC_MAIN], envp[i] + 5, ':');
             return (OPERATION_SUCCESS);
         }
         i++;
     }
     *paths = NULL;
     return (OPERATION_FAILURE);
 }
