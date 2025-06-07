

/**
 * @file builtins.h
 * @brief Header file for builtin shell commands and their helper functions
 *
 * This file contains declarations for all built-in shell commands:
 * - echo: display a line of text with optional newline suppression (-n)
 * - cd: change the current directory
 * - pwd: print the current working directory
 * - export: set or modify environment variables
 * - unset: remove environment variables
 * - env: display the environment variables
 * - exit: exit the shell with an optional status code
 *
 * Each builtin implements behavior similar to bash, with appropriate
 * argument handling and environment variable management.
 */
#ifndef BUILTINS_H
# define BUILTINS_H

# include "command_tree.h"
# include "environment.h"
# include "execution.h"
# include "minishell.h"
# include "token.h"

/* ------------------------------------------------------------------------- */
/*                              builtins                                     */
/* ------------------------------------------------------------------------- */
// builtins.c
int		exec_builtin(t_exec *cmd, t_minishell *minishell);
// cd.c
int		ft_cd(char **argv, t_minishell *minishell);
// echo.c
int		ft_echo(char **argv);
// env.c
int		ft_env(t_minishell *minishell);
// env_utils.c
// int find_env_value(const char *name, t_minishell *minishell, char **result);
// exit.c
void	cleanup_memory(t_minishell *minishell);
int		ft_exit(char **argv, t_minishell *minishell);
// export_print.c
void    print_export(t_minishell *minishell, t_list *envp);
// export_utils.c
void	add_new_env(char *key, char *value, t_minishell *minishell);
void	update_existing_env(t_minishell *minishell, t_env *env, char *key,
			char *value);
void	update_or_add_env(const char *arg, t_minishell *minishell);
// export.c
int		ft_export(char **argv, t_minishell *minishell);
// pwd.c
int		ft_pwd(void);
// unset.c
int		ft_unset(char **argv, t_minishell *minishell);
/* ------------------------------------------------------------------------- */
#endif
