/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:19:21 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 00:30:22 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

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

int	process_key_only_export(t_minishell *minishell, char *key)
{
	t_env	*env;

	env = find_env_var(key, minishell);
	if (env)
	{
		env->is_export = true;
	}
	else
	{
		set_env_var(key, "", minishell);
		env = find_env_var(key, minishell);
		if (env)
			env->is_export = true;
	}
	minishell->envp_arr = rebuild_env_array(minishell);
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

int	process_export_append(t_minishell *minishell, char *key, char *append_value)
{
	t_env	*env;
	char	*new_value;
	char	*clean_append;
	char	*old_val;

	env = find_env_var(key, minishell);
	clean_append = remove_all_quotes(minishell, append_value);
	if (env && env->content)
	{
		old_val = ft_strchr(env->content, '=');
		if (old_val)
			old_val++;
		else
			old_val = "";
		new_value = gc_strjoin(minishell->gc[GC_ENV], old_val, clean_append);
		set_env_var(key, new_value, minishell);
	}
	else
	{
		set_env_var(key, clean_append, minishell);
	}
	env = find_env_var(key, minishell);
	if (env)
		env->is_export = true;
	return (BUILTIN_SUCCESS);
}

int	process_export_arg(t_minishell *minishell, char *arg)
{
	char	*equal;
	char	*plus;
	char	*key;
	char	*key_no_quotes;

	plus = ft_strchr(arg, '+');
	equal = ft_strchr(arg, '=');
	if (plus && equal && plus + 1 == equal)
	{
		key = gc_substr(minishell->gc[GC_TEMP], arg, 0, plus - arg);
		key_no_quotes = remove_all_quotes(minishell, key);
		return (process_export_append(minishell, key_no_quotes, equal + 1));
	}
	if (equal)
		key = gc_substr(minishell->gc[GC_TEMP], arg, 0, equal - arg);
	else
		key = gc_strdup(minishell->gc[GC_TEMP], arg);
	key_no_quotes = remove_all_quotes(minishell, key);
	if (!is_valid_export_key(arg))
		return (handle_invalid_identifier(arg));
	else if (equal)
		return (process_key_value_export(minishell, key_no_quotes, equal));
	else
		return (process_key_only_export(minishell, key_no_quotes));
}
