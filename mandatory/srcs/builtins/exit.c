/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:10:12 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 21:30:54 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Checks if a string is numeric.
static bool	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (false);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

// Cleans up memory and resets terminal mode.
void	cleanup_memory(t_minishell *minishell)
{
	int	i;

	i = 0;
	reset_terminal_mode();
	while (i < GC_COUNT)
	{
		gc_cleanup(&minishell->gc[i]);
		i++;
	}
}

// Cleans up memory and exits with the given code.
static void	print_exit_and_cleanup(t_minishell *minishell, int code)
{
	cleanup_memory(minishell);
	exit(code);
}

// Handles argument parsing and exit logic for the exit builtin.
static int	handle_exit_args(char **argv, t_minishell *minishell)
{
	long	code;

	if (!argv[COMMAND_ARGS_START])
		print_exit_and_cleanup(minishell, minishell->exit_code);
	if (!is_numeric(argv[COMMAND_ARGS_START]))
	{
		write(STDERR_FILENO, "minishell: exit: ", 18);
		write(STDERR_FILENO, argv[COMMAND_ARGS_START],
			ft_strlen(argv[COMMAND_ARGS_START]));
		write(STDERR_FILENO, ": numeric argument required\n", 29);
		print_exit_and_cleanup(minishell, EXIT_INVALID_NUMERIC);
	}
	if (argv[COMMAND_ARGS_START + 1])
	{
		write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
		return (BUILTIN_FAILURE);
	}
	code = ft_atol(argv[COMMAND_ARGS_START]);
	print_exit_and_cleanup(minishell, (unsigned char)code);
	return (BUILTIN_SUCCESS);
}

// Implementation of the exit builtin.
int	ft_exit(char **argv, t_minishell *minishell)
{
	if (minishell->in_subshell == false)
		write(STDERR_FILENO, "exit\n", 5);
	return (handle_exit_args(argv, minishell));
}
