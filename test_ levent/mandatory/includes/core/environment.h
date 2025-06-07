

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
	char			*value;
	char			*content;

	union
	{
		struct
		{
			bool	is_export;
			bool	printed;
			char	padding[6];
		};
		void		*allignment_placeholder;
	};
}					t_env;

// unset.c
char				**rebuild_env_array(t_minishell *minishell);
int					set_env_var(const char *name, const char *value,
						t_minishell *minishell);
int					lookup_env_value(const char *name, t_minishell *minishell,
						char **result);
void init_environment(t_minishell *shell, char **envp);

#endif
