

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
	char	*value;
	char	*content;
	bool	is_export;
	bool	printed;
}			t_env;

#endif
