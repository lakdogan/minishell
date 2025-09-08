/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:40:02 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 22:52:50 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

/* Handles environment variable case (contains '=') */
static void	handle_env_var(t_arg_lst **head, char *value, t_arg_flags flags,
		t_minishell *shell)
{
	t_arg_lst	*new_node;

	new_node = create_arg_node(shell, value, flags.no_expand, flags.was_quoted);
	add_node_to_list(head, new_node);
}

/* Handles splitting unquoted words with spaces */
static void	handle_unquoted_spaces(t_arg_lst **head, char *value,
		t_minishell *shell)
{
	char		**split;
	t_arg_lst	*new_node;
	int			i;

	split = gc_split(shell->gc[GC_MAIN], value, ' ');
	i = 0;
	while (split[i])
	{
		if (split[i][0] == '\0')
		{
			i++;
			continue ;
		}
		new_node = create_arg_node(shell, split[i], false, false);
		add_node_to_list(head, new_node);
		i++;
	}
}

/* Handles regular word case */
static void	handle_regular_word(t_arg_lst **head, char *value,
		t_arg_flags flags, t_minishell *shell)
{
	t_arg_lst	*new_node;

	new_node = create_arg_node(shell, value, flags.no_expand, flags.was_quoted);
	add_node_to_list(head, new_node);
}

/* Main function to add an argument to the linked list */
void	arg_to_list(t_arg_lst **head, char *value, t_arg_flags flags,
		t_minishell *shell)
{
	if (!value)
		return ;
	if (ft_strchr(value, '=') && (value < ft_strchr(value, '=')))
		handle_env_var(head, value, flags, shell);
	else if (!flags.was_quoted && ft_strchr(value, ' '))
		handle_unquoted_spaces(head, value, shell);
	else
		handle_regular_word(head, value, flags, shell);
}
