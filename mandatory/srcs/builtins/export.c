/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:21:15 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 19:21:21 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

/* Validate if export key name follows identifier rules */
int	is_valid_export_key(const char *argv)
{
	int	i;

	i = 0;
	if (!argv || !argv[i] || !(ft_isalpha(argv[i]) || argv[i] == UNDERSCORE))
		return (IDENTIFIER_INVALID);
	i++;
	while (argv[i] && argv[i] != EQUALS_SIGN)
	{
		if (!ft_isalnum(argv[i]) && argv[i] != UNDERSCORE)
			return (IDENTIFIER_INVALID);
		i++;
	}
	return (IDENTIFIER_VALID);
}

/* Remove quotes from a string */
char	*remove_all_quotes(t_minishell *minishell, const char *value)
{
	size_t	len;
	char	*result;
	size_t	j;
	size_t	i;

	len = ft_strlen(value);
	result = gc_malloc(minishell->gc[GC_TEMP], len + 1);
	j = 0;
	i = 0;
	while (i < len)
	{
		if (value[i] != '"' && value[i] != '\'')
			result[j++] = value[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/* Main export command implementation */
int	ft_export(char **argv, t_minishell *minishell)
{
	int	i;
	int	status;
	int	arg_status;

	status = BUILTIN_SUCCESS;
	i = COMMAND_ARGS_START;
	if (!argv[i])
	{
		print_export(minishell, minishell->envp);
		return (BUILTIN_SUCCESS);
	}
	while (argv[i])
	{
		arg_status = process_export_arg(minishell, argv[i]);
		if (arg_status != BUILTIN_SUCCESS)
			status = arg_status;
		if (arg_status == 2)
			break ;
		i++;
	}
	minishell->envp_arr = rebuild_env_array(minishell);
	return (status);
}
