

/**
 * @file environment.h
 * @brief Defines the environment variable structure
 * and related flags used for export, unset, and env handling.
 *
 * TODO:
 * - Create functions to init, get, set, unset, and free env list
 * - Build env array from list (for execve)
 * - Handle export formatting with is_export and printed
 * - Implement env sorting for "export"
 */
#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# include "minishell.h"
# include <stdbool.h>

typedef struct s_env
{
	char *value;    // Name of the variable (e.g., "PATH")
	char *content;  // Value (e.g., "/usr/bin:/bin")

	union environment
	{
		struct
		{
			bool is_export; // Used for export command formatting
			bool printed;   // Used to avoid duplicate prints in export
			char	padding[6];
		};
		void	*allignment_placeholder;
	};
}		t_env;

// unset.c
char	**rebuild_env_array(t_minishell *minishell);
int		set_env_var(const char *name, const char *value,
			t_minishell *minishell);
int		get_env_value(const char *name, t_minishell *minishell, char **result);

#endif
