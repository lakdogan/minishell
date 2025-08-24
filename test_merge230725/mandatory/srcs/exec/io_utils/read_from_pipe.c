#include "../../../includes/core/minishell.h"

static char	*init_pipe_buffer(t_gc *gc)
{
	return (gc_strdup(gc, EMPTY_STRING));
}

static size_t	read_pipe_chunk(int pipe_fd, char *buffer, size_t buffer_size)
{
	ssize_t	bytes;

	bytes = read(pipe_fd, buffer, buffer_size - NULL_TERMINATOR_SIZE);
	if (bytes > SUCCESSFUL_READ)
		buffer[bytes] = NULL_TERMINATOR;
	return (bytes);
}

static char	*accumulate_pipe_content(t_minishell *shell, int pipe_fd,
	char *content)
{
char	buffer[BUFFER_SIZE_TEE];
ssize_t	bytes;

bytes = read_pipe_chunk(pipe_fd, buffer, BUFFER_SIZE_TEE);
while (bytes > SUCCESSFUL_READ)
{
	content = gc_safe_strjoin(shell->gc[GC_TEMP], content, buffer);
	if (!content)
		return (NULL);
	bytes = read_pipe_chunk(pipe_fd, buffer, BUFFER_SIZE_TEE);
}
if (bytes == READ_ERROR)
	exit_with_error(shell, "minishell: ", "read from pipe", strerror(errno), EXIT_FAILURE);
return (content);
}

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
