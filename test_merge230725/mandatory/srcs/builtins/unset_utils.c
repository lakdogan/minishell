#include "../../includes/core/minishell.h"

/**
 * @brief Counts the number of exportable environment variables
 *
 * @param envp Linked list of environment variables
 * @return Number of variables marked for export
 */
static int	count_exportable_env_vars(t_list *envp)
{
	t_list	*env_node;
	t_env	*env_var;
	int		count;

	count = 0;
	env_node = envp;
	while (env_node)
	{
		env_var = (t_env *)env_node->content;
		if (env_var->is_export)
			count++;
		env_node = env_node->next;
	}
	return (count);
}

/**
 * @brief Populates the environment array with variables from the linked list
 *
 * @param minishell Pointer to the minishell structure
 * @param env_array The pre-allocated environment array to populate
 * @return The populated array or NULL if an error occurred
 */
static char	**populate_env_array(t_minishell *minishell, char **env_array)
{
	t_list	*env_node;
	t_env	*env_var;
	int		i;

	env_node = minishell->envp;
	i = 0;
	while (env_node)
	{
		env_var = (t_env *)env_node->content;
		if (env_var->is_export)
		{
			env_array[i] = gc_strjoin_3(minishell->gc[GC_ENV], env_var->value,
					EQUALS_SIGN_STR, env_var->content);
			if (!env_array[i])
				return (NULL);
			i++;
		}
		env_node = env_node->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

/**
 * @brief Creates a new environment array from the environment linked list
 *
 * @param minishell The minishell structure containing the environment list
 * @return The new environment array or NULL if allocation fails
 */
static char	**create_env_array(t_minishell *minishell)
{
	char	**new_env;
	int		env_count;

	env_count = count_exportable_env_vars(minishell->envp);
	new_env = gc_str_array_create(minishell->gc[GC_ENV], env_count
			+ ARRAY_NULL_TERMINATOR_SIZE);
	if (!new_env)
		return (NULL);
	return (populate_env_array(minishell, new_env));
}

/**
 * @brief Rebuilds the environment array from the linked list
 *
 * @param minishell Pointer to the minishell structure
 * @return The new environment array or NULL if allocation fails
 */
char	**rebuild_env_array(t_minishell *minishell)
{
	return (create_env_array(minishell));
}
