#include "../../../../includes/core/minishell.h"

int	get_path_from_env(t_minishell *shell, char **envp, char ***paths)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], PATH_VAR_PREFIX,
				PATH_PREFIX_LENGTH) == STRINGS_EQUAL)
		{
			*paths = gc_split(shell->gc[GC_MAIN], envp[i] + PATH_PREFIX_LENGTH,
					PATH_DELIMITER);
			return (OPERATION_SUCCESS);
		}
		i++;
	}
	*paths = NULL;
	return (OPERATION_FAILURE);
}
