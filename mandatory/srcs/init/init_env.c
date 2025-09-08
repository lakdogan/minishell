/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:36:08 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 22:58:55 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

/**
 * Allocate memory for the environment array
 */
static void	allocate_env_array(t_minishell *shell, int count)
{
	t_error_context	err_ctx;

	shell->envp_arr = gc_malloc(shell->gc[GC_MAIN], sizeof(char *) * (count
				+ NULL_TERMINATOR_SIZE));
	if (!shell->envp_arr)
	{
		err_ctx.shell = shell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = "envp_arr";
		err_ctx.message = "Memory allocation failed";
		err_ctx.exit_code = EXIT_FAILURE;
		exit_with_error(err_ctx);
	}
}

/**
 * Process a single environment variable
 */
static void	process_env_var(t_minishell *shell, char *env_str, int index)
{
	char			*equal_sign;
	t_env			*env;
	t_error_context	err_ctx;

	shell->envp_arr[index] = gc_strdup(shell->gc[GC_MAIN], env_str);
	if (!shell->envp_arr[index])
	{
		err_ctx.shell = shell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = "env string";
		err_ctx.message = "Memory allocation failed";
		err_ctx.exit_code = EXIT_FAILURE;
		exit_with_error(err_ctx);
	}
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
	if (!find_env_var("TERM", shell))
		set_env_var("TERM", "xterm-256color", shell);
}
