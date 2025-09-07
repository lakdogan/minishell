/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:33:06 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 23:33:28 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "command_tree.h"
# include "environment.h"
# include "execution.h"
# include "minishell.h"
# include "token.h"

# define BUILTIN_SUCCESS 0
# define BUILTIN_FAILURE 1
# define ARRAY_NULL_TERMINATOR_SIZE 1
# define ARRAY_LAST_ELEMENT_OFFSET 1
# define STRING_COMPARISON_GREATER 0
# define NEXT_ELEMENT_INDEX 1
# define EXPORT_EQUALS_QUOTE "=\""
# define SKIP_EQUALS_SIGN 1
# define EXPORT_CLOSING_QUOTE "\""
# define EXIT_INVALID_NUMERIC 255
# define OPTION_VALID 1
# define OPTION_INVALID 0
# define DASH_CHAR '-'
# define OPTION_N_CHAR 'n'
# define SECOND_CHAR 1
# define NEWLINE_ENABLE 1
# define NEWLINE_DISABLED 0
# define SPACE_STR " "
# define SYSCALL_SUCCESS 0
# define AUTO_BUFFER_SIZE 0
# define BUILTIN_NOT_FOUND 1
# define ENV_FLAG_EMPTY 1

/* ------------------------------------------------------------------------- */
/*                              builtins                                     */
/* ------------------------------------------------------------------------- */
// builtins.c
int		exec_builtin(t_exec *cmd, t_minishell *minishell);
// cd.c
int		ft_cd(char **argv, t_minishell *minishell);
// echo.c
int		ft_echo(char **argv, bool *no_expand_flags, bool *was_quoted_flags);
// env.c
int		ft_env(t_minishell *minishell, char **argv);
// exit.c
void	cleanup_memory(t_minishell *minishell);
int		ft_exit(char **argv, t_minishell *minishell);
// export_print.c
void	print_export(t_minishell *minishell, t_list *envp);
// export.c
int		ft_export(char **argv, t_minishell *minishell);
// pwd.c
int		ft_pwd(void);
// unset.c
int		ft_unset(char **argv, t_minishell *minishell);
/* ------------------------------------------------------------------------- */
char	*get_cd_target_path(char **argv);
void	update_pwd_variables(t_minishell *minishell, const char *new_pwd,
			const char *old_pwd);
int		handle_cd_dash(t_minishell *minishell);
void	update_pwd_variables(t_minishell *minishell, const char *new_pwd,
			const char *old_pwd);
int		parse_env_args(char **argv, int *cmd_index);
char	**build_env_for_command(t_minishell *minishell, char **argv, int flags,
			int cmd_index);
void	free_env_array(char **env_array, int count);
int		execute_env_command(char **argv, int cmd_index, char **custom_env,
			t_minishell *minishell);
t_env	*find_env_by_key(t_minishell *minishell, const char *key);
char	*extract_key(t_minishell *minishell, const char *arg);
char	*extract_value(t_minishell *minishell, const char *arg);
void	add_new_env(char *key, char *value, t_minishell *minishell);
void	update_existing_env(t_minishell *minishell, t_env *env, char *key,
			char *value);
void	update_or_add_env(const char *arg, t_minishell *minishell);
int		is_valid_export_key(const char *argv);
char	*remove_all_quotes(t_minishell *minishell, const char *value);
int		process_export_arg(t_minishell *minishell, char *arg);
int		handle_export_option(char *arg);
int		process_key_value_export(t_minishell *minishell, char *key_no_quotes,
			char *equal);
int		process_key_only_export(t_minishell *minishell, char *key);
int		handle_invalid_identifier(char *arg);
#endif
