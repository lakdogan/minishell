/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:43:13 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 19:01:13 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

/* Update PWD and OLDPWD environment variables */
void	update_pwd_variables(t_minishell *minishell, const char *new_pwd,
		const char *old_pwd)
{
	set_env_var("PWD", new_pwd, minishell);
	if (old_pwd)
		set_env_var("OLDPWD", old_pwd, minishell);
	minishell->envp_arr = rebuild_env_array(minishell);
}

/* Handle 'cd -' command (change to OLDPWD) */
int	handle_cd_dash(t_minishell *minishell)
{
	char	*oldpwd;
	char	cwd[PATH_MAX];

	oldpwd = NULL;
	lookup_env_value("OLDPWD", minishell, &oldpwd);
	if (!oldpwd)
	{
		write(STDERR_FILENO, "cd: OLDPWD not set\n", 19);
		return (BUILTIN_FAILURE);
	}
	if (chdir(oldpwd) != SYSCALL_SUCCESS)
	{
		perror("cd");
		return (BUILTIN_FAILURE);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_putendl_fd(cwd, STDOUT_FILENO);
	update_pwd_variables(minishell, cwd, minishell->cwd);
	return (BUILTIN_SUCCESS);
}
