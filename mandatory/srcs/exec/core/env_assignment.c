/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_assignment.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:10:38 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 00:05:51 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Handles assignment-only commands.
void	handle_assignment_command(t_command *cmd, t_minishell *shell)
{
	int		i;
	char	*name;
	char	*value;
	char	*eq_pos;

	if (!cmd->env_vars)
		return ;
	i = 0;
	while (cmd->env_vars[i])
	{
		eq_pos = ft_strchr(cmd->env_vars[i], '=');
		if (eq_pos)
		{
			*eq_pos = '\0';
			name = cmd->env_vars[i];
			value = eq_pos + 1;
			set_env_var(name, value, shell);
			*eq_pos = '=';
		}
		i++;
	}
	shell->envp_arr = rebuild_env_array(shell);
	shell->exit_code = 0;
}
