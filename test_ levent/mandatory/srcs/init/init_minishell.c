#include "../../includes/core/minishell.h"

static int	allocate_gc_structs(t_minishell *sh)
{
	int	i;

	i = 0;
	while (i < GC_COUNT)
	{
		sh->gc[i] = gc_create();
		if (!sh->gc[i])
			return (GC_STRUCT_ALLOCATION_FAILED);
		i++;
	}
	return (GC_STRUCT_ALLOCATION_SUCCESS);
}

static void	init_environment(t_minishell *sh, char **envp)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal_sign;

	sh->envp = NULL;
	i = 0;
	while (envp[i])
		i++;
	sh->envp_arr = gc_str_array_create(sh->gc[GC_MAIN], i + 1);
	i = 0;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
		{
			key = gc_malloc(sh->gc[GC_ENV], equal_sign - envp[i] + 1);
			ft_strlcpy(key, envp[i], equal_sign - envp[i] + 1);
			value = gc_strdup(sh->gc[GC_ENV], equal_sign + 1);
			add_new_env(key, value, sh);
		}
		sh->envp_arr[i] = gc_strdup(sh->gc[GC_MAIN], envp[i]);
		i++;
	}
	sh->envp_arr[i] = NULL;
}

static void	init_data(t_minishell *shell, char **envp)
{
	shell->token_list = NULL;
	shell->root = NULL;
	shell->input = NULL;
	shell->exit_code = 0;
	shell->last_signal = 0;
	shell->in_nested_pipe = false;
	if (!envp)
	{
		shell->envp = NULL;
		shell->envp_arr = NULL;
	}
	else
		init_environment(shell, envp);
}

int	init_minishell(t_minishell *shell, char **envp)
{
	char	*temp;

	ft_bzero(shell, sizeof(t_minishell));
	if (allocate_gc_structs(shell) == GC_STRUCT_ALLOCATION_FAILED)
		return (-1);
	init_data(shell, envp);
	temp = getcwd(NULL, 0);
	if (!temp)
		return (-1);
	shell->cwd = gc_strdup(shell->gc[GC_CWD], temp);
	free(temp);
	if (!shell->cwd)
		return (-1);
	return (0);
}
