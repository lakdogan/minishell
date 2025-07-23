#include "../../../includes/core/minishell.h"

t_exec	*exec_init(t_gc *gc)
{
	t_exec	*exec;

	exec = gc_malloc(gc, sizeof(t_exec));
	exec->argv = NULL;
	exec->command = NULL;
	exec->infiles = NULL;
	exec->outfiles = NULL;
	exec->heredoc_fd = -1;
	exec->heredoc_prepared = false;

	return (exec);
}

t_command_tree	*node_init(t_node_type type, t_gc *gc)
{
	t_command_tree	*node;

	node = gc_calloc(gc, 1, sizeof(t_command_tree));
	if (!node)
		return (NULL);
	node->type = type;
	return (node);
}
