#ifndef COMMAND_TREE_H
# define COMMAND_TREE_H

# include "minishell.h"

// Define node types for the command tree.
typedef enum e_node_type {
	NODE_CMD,
	NODE_ANDIF,
	NODE_OR,
	NODE_PIPE,
	NODE_SUBSHELL, // Added for subshell nodes.
	// ...other types...
} t_node_type;

// Forward declaration of t_command if not defined.
#ifndef T_COMMAND_DEFINED
# define T_COMMAND_DEFINED
typedef struct s_command {
	char	**argv;
	char	**env_vars;
	// ...other fields...
} t_command;
#endif

// Define the command tree structure.
typedef struct s_command_tree {
	t_node_type				type;
	t_command				*cmd;
	void					*data;      // Added for additional node data (e.g. redirection info for subshell)
	struct s_command_tree	*left;
	struct s_command_tree	*right;
} t_command_tree;

#endif
