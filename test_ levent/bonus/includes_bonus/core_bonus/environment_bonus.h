

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
#ifndef ENVIRONMENT_BONUS_H
# define ENVIRONMENT_BONUS_H

# include "minishell_bonus.h"
# include <stdbool.h>

typedef struct s_env
{
	char	*value;		// Name of the variable (e.g., "PATH")
	char	*content;	// Value (e.g., "/usr/bin:/bin")
	bool	is_export;	// Used for export command formatting
	bool	printed;	// Used to avoid duplicate prints in export
}			t_env;

#endif
