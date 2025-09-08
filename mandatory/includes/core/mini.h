/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:54:37 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/05 23:26:58 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_H
# define MINI_H
# include "minishell.h"

typedef enum e_gc_type
{
	GC_MAIN,
	GC_COMMAND,
	GC_TEMP,
	GC_ENV,
	GC_CWD,
	GC_EXPAND,
	GC_PROC_HERE,
	GC_PWD,
	GC_LVL,
	GC_COUNT
}					t_gc_type;

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
	t_gc			*gc[GC_COUNT];
	bool			in_nested_pipe;
	char			*last_executed_command;
	char			*last_command_arg;
	int				last_pid;
	bool			redirection_failed;
	char			*current_command;
	char			*line;
	int				is_heredoc;
	bool			source_mode;
	int				env_flags;

}					t_minishell;
#endif
