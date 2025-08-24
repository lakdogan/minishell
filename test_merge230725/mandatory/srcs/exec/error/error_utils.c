/**
 * @file error_utils.c
 * @brief Error handling utility functions
 *
 * This file contains utility functions for handling and reporting errors
 * in the shell. It provides standardized error reporting and program
 * termination functionality.
 *
 * @author lakdogan <lakdogan@student.42.fr>
 * @date 2025-05-19
 */

#include "../../../includes/core/minishell.h"

// void	exit_with_error(t_minishell *shell, const char *prefix,
// 		const char *message, int exit_code)
// {
// 	ft_putstr_fd((char *)prefix, STDERR_FILENO);
// 	ft_putstr_fd((char *)message, STDERR_FILENO);
// 	ft_putstr_fd("\n", STDERR_FILENO);
// 	if (shell)
// 		cleanup_memory(shell);
// 	exit(exit_code);
// }

void	exit_with_error(t_minishell *shell, const char *prefix,
	const char *subject, const char *message, int exit_code)
{
ft_putstr_fd((char *)prefix, STDERR_FILENO);
ft_putstr_fd((char *)subject, STDERR_FILENO);
ft_putstr_fd(": ", STDERR_FILENO);
ft_putstr_fd((char *)message, STDERR_FILENO);
ft_putstr_fd("\n", STDERR_FILENO);
if (shell)
	cleanup_memory(shell);
exit(exit_code);
}
