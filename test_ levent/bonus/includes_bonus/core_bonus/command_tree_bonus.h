

/**
 * @file command_tree_bonus.h
 * @brief Defines the binary tree structure used to
 * represent the abstract syntax tree (AST)
 *        for parsed shell commands, including execution nodes,
 * pipelines, and logical operators.
 *
 * The AST is built from the token list and allows
 * recursive execution of complex command sequences.
 * Each node in the tree represents either a
 * command (N_EXEC), a pipe (N_PIPE),
 * a logical AND (N_ANDIF), or a logical OR (N_OR).
 *
 * TODO:
 * - Build AST from token list respecting
 * operator precedence and parentheses
 * - Create utility functions: create_node(), free_tree(), print_tree()
 * - Traverse the tree recursively to execute commands in correct order
 * - Handle each node type during execution (exec, pipe, and/or)
 */
#ifndef COMMAND_TREE_BONUS_H
# define COMMAND_TREE_BONUS_H

# include "minishell_bonus.h"

typedef enum e_node_type
{
	N_EXEC,		// Single command
	N_PIPE,		// Pipe (|)
	N_ANDIF,	// &&
	N_OR,		// ||
}							t_node_type;

typedef struct s_command_tree
{
	t_node_type				type;
	void					*data;		// Usually a t_exec*
	struct s_command_tree	*left;
	struct s_command_tree	*right;
}							t_command_tree;

#endif
