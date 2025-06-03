

/**
 * @file error_utils_bonus.c
 * @brief Error handling utility functions
 *
 * This file contains utility functions for handling and reporting errors
 * in the shell. It provides standardized error reporting and program
 * termination functionality.
 *
 * @author lakdogan <lakdogan@student.42.fr>
 * @date 2025-05-19
 */

#include "../../../includes_bonus/core_bonus/minishell_bonus.h"

/**
 * @brief Prints an error message and exits the program
 *
 * Outputs an error message to stderr with the given prefix and message,
 * then terminates the program with the specified exit code. This is used
 * for fatal errors that prevent the shell from continuing execution.
 *
 * @param prefix The context or command that encountered the error
 * @param message The specific error message to display
 * @param exit_code The exit code to terminate the program with
 * @note This function does not return as it calls exit()
 */
void	exit_with_error(const char *prefix, const char *message, int exit_code)
{
	ft_putstr_fd((char *)prefix, 2);
	ft_putstr_fd((char *)message, 2);
	ft_putstr_fd("\n", 2);
	exit(exit_code);
}
