/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_chain_executor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:09:56 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 20:14:29 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Allocate resources for pipe processes */
static int	allocate_process_resources(int num_cmds, pid_t **pids_ptr)
{
	*pids_ptr = malloc(sizeof(pid_t) * num_cmds);
	if (!*pids_ptr)
		return (1);
	return (0);
}

// Forks and executes a single process in the pipeline.
static void	fork_and_exec_single_pipe(t_pipe_fork_ctx *ctx, int i, int prev_fd,
		int pipefd[])
{
	ctx->pids[i] = fork();
	if (ctx->pids[i] == 0)
	{
		setup_pipe_redirections(i, ctx->num_cmds, prev_fd, pipefd);
		if (((t_command_tree *)ctx->execs[i])->type == NODE_SUBSHELL)
		{
			execute_subshell((t_command_tree *)ctx->execs[i], ctx->shell);
			exit(ctx->shell->exit_code);
		}
		else
		{
			execute_command_from_pipe(ctx->execs[i], ctx->shell);
			exit(1);
		}
	}
}

// Forks all processes for the pipeline.
static int	fork_pipe_processes(t_pipe_fork_ctx *ctx)
{
	int	i;
	int	prev_fd;
	int	pipefd[2];

	prev_fd = -1;
	i = 0;
	while (i < ctx->num_cmds)
	{
		if (i < ctx->num_cmds - 1)
			pipe(pipefd);
		fork_and_exec_single_pipe(ctx, i, prev_fd, pipefd);
		update_parent_pipes(&prev_fd, pipefd, i == ctx->num_cmds - 1);
		i++;
	}
	return (0);
}

/* Execute a pipeline of commands */
int	execute_pipe_chain(t_exec **execs, int num_cmds, t_minishell *shell)
{
	pid_t			*pids;
	int				result;
	t_pipe_fork_ctx	ctx;

	if (allocate_process_resources(num_cmds, &pids))
		return (1);
	ctx.execs = execs;
	ctx.num_cmds = num_cmds;
	ctx.shell = shell;
	ctx.pids = pids;
	result = fork_pipe_processes(&ctx);
	if (result)
	{
		free(pids);
		return (1);
	}
	wait_for_pipe_children(pids, num_cmds, shell);
	return (0);
}
