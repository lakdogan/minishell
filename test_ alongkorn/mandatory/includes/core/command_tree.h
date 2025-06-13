#ifndef COMMAND_TREE_H
# define COMMAND_TREE_H

# include "minishell.h"

typedef enum e_node_type
{
	N_EXEC,
	N_PIPE,
	N_ANDIF,
	N_OR,
}							t_node_type;

typedef struct s_command_tree
{
	t_node_type				type;
	void					*data;
	struct s_command_tree	*left;
	struct s_command_tree	*right;
}							t_command_tree;

#endif
