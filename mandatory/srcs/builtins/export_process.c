/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:19:21 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 19:21:25 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

/* Check if option flag is provided and handle it */
int	handle_export_option(char *arg)
{
	write(STDERR_FILENO, "minishell: export: ", 20);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, ": invalid option\n", 17);
	write(STDERR_FILENO,
		"export: usage: export [-nf] [name[=value] ...] or export -p\n", 61);
	return (2);
}

/* Process a key=value export argument */
int	process_key_value_export(t_minishell *minishell, char *key_no_quotes,
		char *equal)
{
	char	*clean_value;
	t_env	*env;

	clean_value = remove_all_quotes(minishell, equal + 1);
	set_env_var(key_no_quotes, clean_value, minishell);
	env = find_env_var(key_no_quotes, minishell);
	if (env)
		env->is_export = true;
	return (BUILTIN_SUCCESS);
}

/* Process a key-only export argument */
int	process_key_only_export(t_minishell *minishell, char *key)
{
	t_env	*env;

	env = find_env_var(key, minishell);
	if (env)
		env->is_export = true;
	return (BUILTIN_SUCCESS);
}

/* Handle invalid identifier error */
int	handle_invalid_identifier(char *arg)
{
	write(STDERR_FILENO, "minishell: export: `", 21);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, "': not a valid identifier\n", 27);
	return (BUILTIN_FAILURE);
}

/* Process a single export argument */
int	process_export_arg(t_minishell *minishell, char *arg)
{
	char	*equal;
	char	*key;
	char	*key_no_quotes;

	if (arg[0] == '-' && arg[1] && ft_strcmp(arg, "--") != 0)
		return (handle_export_option(arg));
	equal = ft_strchr(arg, '=');
	if (equal)
		key = gc_substr(minishell->gc[GC_TEMP], arg, 0, equal - arg);
	else
		key = arg;
	key_no_quotes = remove_all_quotes(minishell, key);
	if (!is_valid_export_key(key_no_quotes))
		return (handle_invalid_identifier(arg));
	else if (equal)
		return (process_key_value_export(minishell, key_no_quotes, equal));
	else
		return (process_key_only_export(minishell, key));
}
