

/**
 * @file command_tree.h
 * @brief Contains the definition of the command
 * tree node used to build the abstract syntax tree (AST)
 * for parsed shell commands, including pipes, logical AND (&&), and logical OR (||).
 *
 * TODO:
 * - Implement tree-building logic from token list
 * - Define tree node content types (t_exec, t_pipe, etc.)
 * - Implement tree traversal for execution
 * - Add functions for printing/freeing the tree
 */
#ifndef COMMAND_TREE_H
# define COMMAND_TREE_H

# include "minishell.h"

typedef enum e_node_type
{
	N_EXEC,
	N_PIPE,
	N_ANDIF,
	N_OR,
}				t_node_type;

typedef struct s_command_tree
{
	t_node_type	type;
	void		*content;
}				t_command_tree;

#endif
