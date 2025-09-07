/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 08:49:46 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/05 23:27:05 by lakdogan         ###   ########.fr       */
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
# include "builtins.h"
# include "command_tree.h"
# include "environment.h"
# include "execution.h"
# include "mini.h"
# include "parser.h"
# include "token.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>

# define GC_STRUCT_ALLOCATION_FAILED 1
# define GC_STRUCT_ALLOCATION_SUCCESS 0

// Handles Abstract Syntax Tree setup
t_command_tree	*setup_ast(char *line, t_minishell *shell, int arc, char **arv);
// Handles environment variables
char			*get_shlvl(const char *lvl, t_gc *gc);
int				increment_shlvl(t_minishell *shell);
// Handles shell input reading and processing
char			*read_shell_input(t_minishell *shell);
void			process_command(t_minishell *shell, int argc, char **argv);
void			shell_loop(t_minishell *shell, int argc, char **argv);
// Handles argument validation
int				check_directory_argument(int argc, char **argv);

int				init_minishell(t_minishell *sh, char **envp);
void			validate_memory(void *ptr, const char *message);
char			*inject_pipes_between_commands(const char *input,
					const char **cmds, int cmd_count);

char			*expand_tilde(const char *arg, t_minishell *shell);
#endif
