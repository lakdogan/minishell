/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:02:13 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 07:14:48 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/minishell.h"

// Checks if the argument is a valid -n option.
static int	is_n_option(char *arg)
{
	int	j;

	if (!arg || arg[FIRST_CHAR] != DASH_CHAR
		|| arg[SECOND_CHAR] != OPTION_N_CHAR)
		return (OPTION_INVALID);
	j = SECOND_CHAR;
	while (arg[j] == OPTION_N_CHAR)
		j++;
	if (arg[j] != NULL_TERMINATOR)
		return (OPTION_INVALID);
	return (OPTION_VALID);
}

// Processes -n options and returns the start index for arguments.
static int	process_options(char **argv, int *newline, bool *no_expand_flags,
		bool *was_quoted_flags)
{
	int	i;

	i = COMMAND_ARGS_START;
	*newline = NEWLINE_ENABLE;
	while (argv[i] && is_n_option(argv[i]) && (!no_expand_flags
			|| !no_expand_flags[i]) && (!was_quoted_flags
			|| !was_quoted_flags[i]))
	{
		*newline = NEWLINE_DISABLED;
		i++;
	}
	return (i);
}

// Prints the arguments to stdout.
static void	print_arguments(char **argv, int start_idx)
{
	int	i;

	i = start_idx;
	while (argv[i])
	{
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		if (argv[i + NEXT_ELEMENT_INDEX])
			write(STDOUT_FILENO, SPACE_STR, 1);
		i++;
	}
}

// Implementation of the echo builtin.
int	ft_echo(char **argv, bool *no_expand_flags, bool *was_quoted_flags)
{
	int	start_idx;
	int	newline;

	start_idx = process_options(argv, &newline, no_expand_flags,
			was_quoted_flags);
	print_arguments(argv, start_idx);
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (BUILTIN_SUCCESS);
}
