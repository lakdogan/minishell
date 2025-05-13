

/**
 * @file minishell.h
 * @brief Central header including all necessary
 * submodules and standard system headers for the minishell project.
 *
 * TODO:
 * - Implement main program logic in main.c
 * - Initialize all members of t_minishell
 * - Free everything properly at the end
 * - Handle signal setup and reset
 */
#ifndef MINISHELL_H
# define MINISHELL_H

# include "../../../libft/inc/libft.h"
# include "command_tree.h"
# include "environment.h"
# include "execution.h"
# include "token.h"
# include <errno.h>
# include <sys/signal.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_minishell
{
	t_list			*envp;
	char			**envp_arr;
	t_list			*token_list;
	t_command_tree	*root;
	char			*input;
	char			*cwd;
	int				exit_code;
	int				last_signal;
}					t_minishell;

#endif
