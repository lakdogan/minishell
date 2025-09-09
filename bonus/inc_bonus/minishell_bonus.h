/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 21:09:21 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 00:22:10 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

# include "../../mandatory/includes/core/minishell.h"
# include <dirent.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

void	handle_and_operator(t_minishell *minishell, t_command_tree *node);
void	handle_or_operator(t_minishell *minishell, t_command_tree *node);
void	execute_node_by_type_bonus(t_command_tree *node,
			t_minishell *minishell);
void	execute_subshell(t_command_tree *node, t_minishell *minishell);
void	print_command_tree(t_command_tree *node, int depth);
void	handle_pipe_in_subshell(t_minishell *shell, t_command_tree *node);
char	**expand_wildcards(char **argv, bool *no_expand_flags,
			t_minishell *shell);
char	*process_token_full(t_token *token, t_minishell *shell);
bool	has_unquoted_star(const char *s);
bool	match_star_recursive(const char *p, const char *s);
bool	match_star(const char *pattern, const char *str);
void	add_pattern_if_no_match(char **result, int *j, char *pattern,
			t_minishell *shell);
void	check_path_accessibility(char *path, t_minishell *minishell);
void	check_if_directory(char *path, t_minishell *minishell);
void	handle_special_cases(t_exec *exec, t_minishell *minishell);
void	handle_dot_command(t_exec *exec);
#endif
