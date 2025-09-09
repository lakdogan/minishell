/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:36:24 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 01:23:49 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Allocates garbage collector structures for minishell.
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

// Initializes core data fields in minishell struct.
static void	init_data(t_minishell *shell, char **envp)
{
	shell->token_list = NULL;
	shell->root = NULL;
	shell->input = NULL;
	shell->exit_code = 0;
	shell->last_signal = 0;
	shell->in_nested_pipe = false;
	shell->redirection_failed = false;
	if (!envp)
	{
		shell->envp = NULL;
		shell->envp_arr = NULL;
	}
	else
		init_environment(shell, envp);
	shell->in_subshell = false;
}

// Initializes the minishell instance.
int	init_minishell(t_minishell *shell, char **envp)
{
	char	*temp;

	ft_bzero(shell, sizeof(t_minishell));
	if (allocate_gc_structs(shell) == GC_STRUCT_ALLOCATION_FAILED)
		return (SYSCALL_ERROR);
	init_data(shell, envp);
	temp = getcwd(NULL, GETCWD_AUTO_ALLOCATE);
	if (!temp)
		return (SYSCALL_ERROR);
	shell->cwd = gc_strdup(shell->gc[GC_CWD], temp);
	free(temp);
	if (!shell->cwd)
		return (SYSCALL_ERROR);
	return (OPERATION_SUCCESS);
}
