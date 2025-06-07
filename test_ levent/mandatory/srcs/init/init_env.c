#include "../../includes/core/minishell.h"

// void	init_environment(t_minishell *shell, char **envp)
// {
// 	int i;
// 	char *equal_sign;

// 	// Reset the environment list
// 	shell->envp = NULL;

// 	// Count environment variables
// 	for (i = 0; envp[i]; i++)
// 		;

// 	// Allocate array for environment variables
// 	shell->envp_arr = gc_malloc(shell->gc[GC_MAIN], sizeof(char *) * (i + 1));
// 	if (!shell->envp_arr)
// 		exit_with_error(shell, "Memory allocation failed", "envp_arr",
// 			EXIT_FAILURE);

// 	// Process each environment variable
// 	for (i = 0; envp[i]; i++)
// 	{
// 		// Print each environment variable during loading (debug)
// 		printf("Loading env: %s\n", envp[i]);

// 		// Make a safe copy
// 		shell->envp_arr[i] = gc_strdup(shell->gc[GC_MAIN], envp[i]);
// 		if (!shell->envp_arr[i])
// 			exit_with_error(shell, "Memory allocation failed", "env string",
// 				EXIT_FAILURE);

// 		// Add to linked list if it contains '='
// 		// Add to linked list if it contains '='
// 		equal_sign = ft_strchr(envp[i], '=');
// 		if (equal_sign)
// 		{
// 			// Key length calculation
// 			int key_len = equal_sign - envp[i];

// 			// Create environment variable node
// 			t_env *env = gc_malloc(shell->gc[GC_MAIN], sizeof(t_env));
// 			if (!env)
// 				exit_with_error(shell, "Memory allocation failed", "env struct",
// 					EXIT_FAILURE);

// 			// Initialize ALL memory to zeros first
// 			ft_memset(env, 0, sizeof(t_env));

// 			// Copy variable name
// 			env->value = gc_malloc(shell->gc[GC_MAIN], key_len + 1);
// 			if (!env->value)
// 				exit_with_error(shell, "Memory allocation failed", "env value",
// 					EXIT_FAILURE);

// 			// Copy name part
// 			ft_memcpy(env->value, envp[i], key_len);
// 			env->value[key_len] = '\0';

// 			// Copy full environment string
// 			env->content = gc_strdup(shell->gc[GC_MAIN], envp[i]);
// 			if (!env->content)
// 				exit_with_error(shell, "Memory allocation failed",
// 					"env content", EXIT_FAILURE);

// 			// Explicitly initialize ALL union fields
// 			env->is_export = true;
// 			env->printed = false;
// 			// Zero out padding bytes to be extra safe
// 			for (int j = 0; j < 6; j++)
// 			{
// 				env->padding[j] = 0;
// 			}

			

// 			// Add to the list
// 			gc_lstadd_back(shell->gc[GC_MAIN], &shell->envp, env);
// 		}
// 	}

// 	// Null-terminate the environment array
// 	shell->envp_arr[i] = NULL;
// }

#include "../../includes/core/minishell.h"

/**
 * Count the number of environment variables
 */
static int count_env_vars(char **envp)
{
    int i;
    
    for (i = 0; envp[i]; i++)
        ;
    return i;
}

/**
 * Allocate memory for the environment array
 */
static void allocate_env_array(t_minishell *shell, int count)
{
    shell->envp_arr = gc_malloc(shell->gc[GC_MAIN], sizeof(char *) * (count + 1));
    if (!shell->envp_arr)
        exit_with_error(shell, "Memory allocation failed", "envp_arr",
            EXIT_FAILURE);
}

/**
 * Create and initialize an environment variable node
 */
static t_env *create_env_node(t_minishell *shell, char *env_str, char *equal_sign)
{
    t_env *env;
    int key_len = equal_sign - env_str;
    
    // Create environment variable node
    env = gc_malloc(shell->gc[GC_MAIN], sizeof(t_env));
    if (!env)
        exit_with_error(shell, "Memory allocation failed", "env struct",
            EXIT_FAILURE);
    
    // Initialize ALL memory to zeros first
    ft_memset(env, 0, sizeof(t_env));
    
    // Copy variable name
    env->value = gc_malloc(shell->gc[GC_MAIN], key_len + 1);
    if (!env->value)
        exit_with_error(shell, "Memory allocation failed", "env value",
            EXIT_FAILURE);
    
    // Copy name part
    ft_memcpy(env->value, env_str, key_len);
    env->value[key_len] = '\0';
    
    // Copy full environment string
    env->content = gc_strdup(shell->gc[GC_MAIN], env_str);
    if (!env->content)
        exit_with_error(shell, "Memory allocation failed",
            "env content", EXIT_FAILURE);
    
    // Explicitly initialize ALL union fields
    env->is_export = true;
    env->printed = false;
    
    // Zero out padding bytes to be extra safe
    for (int j = 0; j < 6; j++)
        env->padding[j] = 0;
    
    return env;
}

/**
 * Process a single environment variable
 */
static void process_env_var(t_minishell *shell, char *env_str, int index)
{
    char *equal_sign;
    
    // Print each environment variable during loading (debug)
    printf("Loading env: %s\n", env_str);
    
    // Make a safe copy
    shell->envp_arr[index] = gc_strdup(shell->gc[GC_MAIN], env_str);
    if (!shell->envp_arr[index])
        exit_with_error(shell, "Memory allocation failed", "env string",
            EXIT_FAILURE);
    
    // Add to linked list if it contains '='
    equal_sign = ft_strchr(env_str, '=');
    if (equal_sign)
    {
        t_env *env = create_env_node(shell, env_str, equal_sign);
        
        // Add to the list
        gc_lstadd_back(shell->gc[GC_MAIN], &shell->envp, env);
    }
}

/**
 * Initialize the shell environment from the provided environment variables
 */
void init_environment(t_minishell *shell, char **envp)
{
    int count;
    int i;
    
    // Reset the environment list
    shell->envp = NULL;
    
    // Count and allocate
    count = count_env_vars(envp);
    allocate_env_array(shell, count);
    
    // Process each environment variable
    for (i = 0; envp[i]; i++)
        process_env_var(shell, envp[i], i);
    
    // Null-terminate the environment array
    shell->envp_arr[i] = NULL;
}
