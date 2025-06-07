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
