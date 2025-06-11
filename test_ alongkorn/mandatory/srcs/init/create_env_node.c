#include "../../includes/core/minishell.h"

static t_env	*allocate_env_node(t_minishell *shell)
{
	t_env	*env;

	env = gc_malloc(shell->gc[GC_MAIN], sizeof(t_env));
	if (!env)
		exit_with_error(shell, "Memory allocation failed", "env struct",
			EXIT_FAILURE);
	ft_bzero(env, sizeof(t_env));
	return (env);
}

static void	set_env_key(t_minishell *shell, t_env *env, char *env_str,
		int key_len)
{
	env->value = gc_malloc(shell->gc[GC_MAIN], key_len + NULL_TERMINATOR_SIZE);
	if (!env->value)
		exit_with_error(shell, "Memory allocation failed", "env value",
			EXIT_FAILURE);
	ft_memcpy(env->value, env_str, key_len);
	env->value[key_len] = NULL_TERMINATOR;
}

static void	set_env_content(t_minishell *shell, t_env *env, char *env_str)
{
	env->content = gc_strdup(shell->gc[GC_MAIN], env_str);
	if (!env->content)
		exit_with_error(shell, "Memory allocation failed", "env content",
			EXIT_FAILURE);
}

static void	initialize_env_flags(t_env *env)
{
	env->is_export = true;
	env->printed = false;
	ft_bzero(env->padding, sizeof(env->padding));
}

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
