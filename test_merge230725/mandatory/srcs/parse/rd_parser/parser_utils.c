
#include "../../../includes/core/minishell.h"

int	is_redir(t_token_type type)
{
	if (type == APPEND || type == OUTFILE || \
		type == INFILE || type == HEREDOC)
		return (1);
	else
		return (0);
}

void	arg_to_list(t_arg_lst **head, char *value, t_gc *gc)
{
	t_arg_lst	*new_node;
	t_arg_lst	*current;

	new_node = gc_malloc(gc, sizeof(t_arg_lst));
	if (!new_node)
		return ;
	new_node->arg = value;
	new_node->next = NULL;
	if ((*head) == NULL)
	{
		(*head) = new_node;
		return ;
	}
	current = (*head);
	while (current->next != NULL)
		current = current->next;
	current->next = new_node;
}

char	**lst_to_argv(t_arg_lst *head, t_gc *gc)
{
	t_arg_lst	*current;
	char		**argv;
	int			count;
	int			i;

	count = 0;
	current = head;
	while (current)
	{
		count++;
		current = current->next;
	}
	argv = gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	current = head;
	i = 0;
	while (i < count)
	{
		argv[i] = current->arg;
		current = current->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

int	add_outfile(t_exec *exec, t_token_type type, char *filename, t_gc *gc)
{
	t_outfile	*node;
	t_outfile	*current;

	node = gc_calloc(gc, 1, sizeof(t_outfile));
	if (!node)
		return (0);
	node->name = filename;
	if (type == APPEND)
		node->type = OUT_APPEND;
	else
		node->type = OUT_TRUNC;
	if (exec->outfiles == NULL)
		exec->outfiles = node;
	else
	{
		current = exec->outfiles;
		while (current->next)
			current = current->next;
		current->next = node;
	}
	return (1);
}

int	add_infile(t_exec *exec, t_token_type type, char *filename, t_gc *gc)
{
	t_infile	*node;
	t_infile	*current;

	node = gc_calloc(gc, 1, sizeof(t_infile));
	if (!node)
		return (0);
	if (type == HEREDOC)
	{
		node->type = INF_HEREDOC;
		node->name = filename;
	}
	else
	{
		node->type = INF_IN;
		node->name = filename;
	}
	if (exec->infiles == NULL)
		exec->infiles = node;
	else
	{
		current = exec->infiles;
		while (current->next)
			current = current->next;
		current->next = node;
	}
	return (1);
}
