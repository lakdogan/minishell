/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 08:49:46 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/05 23:57:50 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include "../../../libft/garbage_collector/inc/garbage_collector.h"
# include "../../../libft/inc/libft.h"
# include "command_tree.h"
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <termios.h>
# include "mini.h"
# include "builtins.h"
# include "environment.h"
# include "execution.h"
# include "parser.h"
# include "command_tree.h"
# include "token.h"

# define GC_STRUCT_ALLOCATION_FAILED 1
# define GC_STRUCT_ALLOCATION_SUCCESS 0

int		init_minishell(t_minishell *sh, char **envp);
void	validate_memory(void *ptr, const char *message);
#endif
