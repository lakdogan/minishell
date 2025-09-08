/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:35:32 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:52:29 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Allocates and initializes a new environment node.
static t_env	*allocate_env_node(t_minishell *shell)
{
	t_env			*env;
	t_error_context	err_ctx;

	env = gc_malloc(shell->gc[GC_MAIN], sizeof(t_env));
	if (!env)
	{
		err_ctx.shell = shell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = "env struct";
		err_ctx.message = "Memory allocation failed";
		err_ctx.exit_code = EXIT_FAILURE;
		exit_with_error(err_ctx);
	}
	ft_bzero(env, sizeof(t_env));
	return (env);
}

// Sets the key part of the environment variable.
static void	set_env_key(t_minishell *shell, t_env *env, char *env_str,
		int key_len)
{
	t_error_context	err_ctx;

	env->value = gc_malloc(shell->gc[GC_MAIN], key_len + NULL_TERMINATOR_SIZE);
	if (!env->value)
	{
		err_ctx.shell = shell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = "env value";
		err_ctx.message = "Memory allocation failed";
		err_ctx.exit_code = EXIT_FAILURE;
		exit_with_error(err_ctx);
	}
	ft_memcpy(env->value, env_str, key_len);
	env->value[key_len] = NULL_TERMINATOR;
}

// Sets the content (full string) of the environment variable.
static void	set_env_content(t_minishell *shell, t_env *env, char *env_str)
{
	t_error_context	err_ctx;

	env->content = gc_strdup(shell->gc[GC_MAIN], env_str);
	if (!env->content)
	{
		err_ctx.shell = shell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = "env content";
		err_ctx.message = "Memory allocation failed";
		err_ctx.exit_code = EXIT_FAILURE;
		exit_with_error(err_ctx);
	}
}

// Initializes export and printed flags for the environment node.
static void	initialize_env_flags(t_env *env)
{
	env->is_export = true;
	env->printed = false;
	ft_bzero(env->padding, sizeof(env->padding));
}

// Creates a new environment node from a string and '=' position.
t_env	*create_env_node(t_minishell *shell, char *env_str, char *equal_sign)
{
	t_env	*env;
	int		key_len;

	key_len = equal_sign - env_str;
	env = allocate_env_node(shell);
	set_env_key(shell, env, env_str, key_len);
	set_env_content(shell, env, env_str);
	initialize_env_flags(env);
	return (env);
}
