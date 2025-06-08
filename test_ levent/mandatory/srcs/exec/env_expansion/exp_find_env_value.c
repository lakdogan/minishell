#include "../../../includes/core/minishell.h"

char	*find_env_value(t_minishell *minishell, const char *name)
{
	t_env	*env;

	env = find_env_var(name, minishell);
	if (!env || !env->content)
		return (NULL);
	if (ft_strchr(env->content, '='))
		return (ft_strchr(env->content, '=') + 1);
	return (env->content);
}
