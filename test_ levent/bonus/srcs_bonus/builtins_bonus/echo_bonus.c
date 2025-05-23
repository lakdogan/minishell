/**
 * @file echo_bonus.c
 * @brief Implementation of the echo built-in command
 *
 * This file contains the implementation of the echo built-in command which
 * outputs its arguments to standard output. It supports the -n option to
 * suppress the trailing newline.
 */

#include "../../includes_bonus/core_bonus/minishell_bonus.h"

 /**
  * @brief Checks if an argument is the -n option
  *
  * Determines if the given argument is a valid -n option for echo.
  * A valid -n option starts with '-' followed by one or more 'n' characters.
  *
  * @param arg The argument to check
  * @return int 1 if the argument is a valid -n option, 0 otherwise
  */
 static int	is_n_option(char *arg)
 {
     int	j;
 
     if (!arg || arg[0] != '-' || arg[1] != 'n')
         return (0);
     j = 1;
     while (arg[j] == 'n')
         j++;
     if (arg[j] != '\0')
         return (0);
     return (1);
 }
 
 /**
  * @brief Processes command options and determines output settings
  *
  * Examines the arguments to find all -n options and sets the newline flag
  * accordingly. Returns the index of the first non-option argument.
  *
  * @param argv Array of command arguments
  * @param newline Pointer to an integer that will be set 
  *     to 0 if -n option is found, 1 otherwise
  * @return int Index of the first non-option argument
  */
 static int	process_options(char **argv, int *newline)
 {
     int	i;
 
     i = 1;
     *newline = 1;
     while (argv[i] && is_n_option(argv[i]))
     {
         *newline = 0;
         i++;
     }
     return (i);
 }
 
 /**
  * @brief Prints the command arguments to standard output
  *
  * Outputs all arguments starting from the given index, separated by spaces.
  *
  * @param argv Array of command arguments
  * @param start_idx Index of the first argument to print
  */
 static void	print_arguments(char **argv, int start_idx)
 {
     int	i;
 
     i = start_idx;
     while (argv[i])
     {
         write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
         if (argv[i + 1])
             write(STDOUT_FILENO, " ", 1);
         i++;
     }
 }
 
 /**
  * @brief Implements the echo built-in command
  *
  * Outputs the given arguments to standard output, separated by spaces.
  * If the -n option is provided, suppresses the trailing newline.
  * Multiple -n options are supported (e.g., -nnn is equivalent to -n).
  *
  * @param argv Array of command arguments (argv[0] is "echo")
  * @return int Always returns 0 (success)
  */
 int	ft_echo(char **argv)
 {
     int	start_idx;
     int	newline;
 
     start_idx = process_options(argv, &newline);
     print_arguments(argv, start_idx);
     if (newline)
         write(STDOUT_FILENO, "\n", 1);
     return (0);
 }
