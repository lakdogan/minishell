/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:34:04 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 23:45:16 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_TREE_H
# define COMMAND_TREE_H

# include "minishell.h"
# include <stdbool.h>

// Define node types for the command tree.
typedef enum e_node_type
{
	NODE_CMD,
	NODE_ANDIF,
	NODE_OR,
	NODE_PIPE,
	NODE_SUBSHELL,
	NODE_EXEC,
	NODE_SEQUENCE,
}							t_node_type;

// Forward declaration of t_command if not defined.
# ifndef T_COMMAND_DEFINED
#  define T_COMMAND_DEFINED

typedef struct s_command
{
	char					**argv;
	char					**env_vars;

}							t_command;
# endif

// Define the command tree structure.
typedef struct s_command_tree
{
	t_node_type				type;
	t_command				*cmd;
	void					*data;
	struct s_command_tree	*left;
	struct s_command_tree	*right;
}							t_command_tree;

#endif
