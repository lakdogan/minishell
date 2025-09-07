/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:34:52 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 23:35:22 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# include "minishell.h"
# include <stdbool.h>

typedef struct s_minishell	t_minishell;

typedef struct s_env
{
	char					*value;
	char					*content;

	union
	{
		struct
		{
			bool			is_export;
			bool			printed;
			char			padding[6];
		};
		void				*allignment_placeholder;
	};
}							t_env;

typedef struct s_env_context
{
	char					**argv;
	int						cmd_index;
	char					**custom_env;
	int						flags;
	t_minishell				*minishell;
}							t_env_context;

char						**rebuild_env_array(t_minishell *minishell);
int							set_env_var(const char *name, const char *value,
								t_minishell *minishell);
int							lookup_env_value(const char *name,
								t_minishell *minishell, char **result);
void						init_environment(t_minishell *shell, char **envp);
t_env						*create_env_node(t_minishell *shell, char *env_str,
								char *equal_sign);
t_env						*find_env_var(const char *name,
								t_minishell *minishell);
char						*expand_tilde(const char *arg, t_minishell *shell);
#endif
