#include "../../../includes/core/minishell.h"

char	*find_env_value(t_minishell *minishell, const char *name)
{
	t_env	*env;

	env = find_env_var(name, minishell);
	if (!env || !env->content)
		return (NULL);
	if (ft_strchr(env->content, EQUALS_SIGN))
		return (ft_strchr(env->content, EQUALS_SIGN) + SKIP_EQUALS_SIGN);
	return (env->content);
}
