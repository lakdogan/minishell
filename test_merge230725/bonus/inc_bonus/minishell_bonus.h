#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

# include "../../mandatory/includes/core/minishell.h"

# define AND_OPERATOR 1
# define OR_OPERATOR 2

// Bonus function declarations
void	handle_and_operator(t_minishell *minishell, t_command_tree *node);
void	handle_or_operator(t_minishell *minishell, t_command_tree *node);

#endif
