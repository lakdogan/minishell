/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_from_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:36:22 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:57:07 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Initializes an empty buffer for pipe reading.
static char	*init_pipe_buffer(t_gc *gc)
{
	return (gc_strdup(gc, EMPTY_STRING));
}

// Reads a chunk from the pipe into buffer.
static size_t	read_pipe_chunk(int pipe_fd, char *buffer, size_t buffer_size)
{
	ssize_t	bytes;

	bytes = read(pipe_fd, buffer, buffer_size - NULL_TERMINATOR_SIZE);
	if (bytes > SUCCESSFUL_READ)
		buffer[bytes] = NULL_TERMINATOR;
	return (bytes);
}

// Accumulates all content read from the pipe into a single string.
static char	*accumulate_pipe_content(t_minishell *shell, int pipe_fd,
		char *content)
{
	char			buffer[BUFFER_SIZE_TEE];
	ssize_t			bytes;
	t_error_context	err_ctx;

	bytes = read_pipe_chunk(pipe_fd, buffer, BUFFER_SIZE_TEE);
	while (bytes > SUCCESSFUL_READ)
	{
		content = gc_safe_strjoin(shell->gc[GC_TEMP], content, buffer);
		if (!content)
			return (NULL);
		bytes = read_pipe_chunk(pipe_fd, buffer, BUFFER_SIZE_TEE);
	}
	if (bytes == READ_ERROR)
	{
		err_ctx.shell = shell;
		err_ctx.prefix = "minishell: ";
		err_ctx.subject = "read from pipe";
		err_ctx.message = strerror(errno);
		err_ctx.exit_code = EXIT_FAILURE;
		exit_with_error(err_ctx);
	}
	return (content);
}

// Finalizes pipe content, moves it to main GC and cleans temp GC.
static char	*finalize_pipe_content(t_minishell *shell, char *temp_content)
{
	char	*final_content;

	if (temp_content && temp_content[FIRST_CHAR] != NULL_TERMINATOR)
	{
		final_content = gc_strdup(shell->gc[GC_MAIN], temp_content);
		gc_collect(shell->gc[GC_TEMP]);
		return (final_content);
	}
	gc_collect(shell->gc[GC_TEMP]);
	return (NULL);
}

// Reads all content from a pipe and returns it as a string.
char	*read_from_pipe(t_minishell *shell, int pipe_fd)
{
	char	*content;

	content = init_pipe_buffer(shell->gc[GC_TEMP]);
	if (!content)
		return (NULL);
	content = accumulate_pipe_content(shell, pipe_fd, content);
	if (!content)
		return (NULL);
	return (finalize_pipe_content(shell, content));
}
