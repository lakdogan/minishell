#include "../../includes/core/minishell.h"

/**
 * Allocate memory for the environment array
 */
static void	allocate_env_array(t_minishell *shell, int count)
{
	shell->envp_arr = gc_malloc(shell->gc[GC_MAIN], sizeof(char *) * (count
				+ NULL_TERMINATOR_SIZE));
	if (!shell->envp_arr)
		exit_with_error(shell, "Memory allocation failed", "envp_arr",
			EXIT_FAILURE);
}

/**
 * Process a single environment variable
 */
static void	process_env_var(t_minishell *shell, char *env_str, int index)
{
	char	*equal_sign;
	t_env	*env;

	shell->envp_arr[index] = gc_strdup(shell->gc[GC_MAIN], env_str);
	if (!shell->envp_arr[index])
		exit_with_error(shell, "Memory allocation failed", "env string",
			EXIT_FAILURE);
	equal_sign = ft_strchr(env_str, '=');
	if (equal_sign)
	{
		env = create_env_node(shell, env_str, equal_sign);
		gc_lstadd_back(shell->gc[GC_MAIN], &shell->envp, env);
	}
}

/**
 * Initialize the shell environment from the provided environment variables
 */
void	init_environment(t_minishell *shell, char **envp)
{
	size_t	count;
	int		i;

	shell->envp = NULL;
	count = ft_strarr_len(envp);
	i = 0;
	allocate_env_array(shell, count);
	while (envp[i])
	{
		process_env_var(shell, envp[i], i);
		i++;
	}
	shell->envp_arr[i] = NULL;
}
