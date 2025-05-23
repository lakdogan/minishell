/**
 * @file signal_handlers.c
 * @brief Signal handling for the shell
 *
 * This file contains functions for setting up and
 * handling signals in both the
 * parent shell process and child command processes.
 * It implements custom behavior
 * for SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\) signals
 * to provide a smooth user
 * experience during interactive shell usage.
 */

 #include "../../../includes/core/minishell.h"

 /**
  * @brief Sets up signal handling for child processes
  *
  * Configures child processes to use default signal
  * handling behavior for
  * SIGINT and SIGQUIT signals. This allows child
  * processes to be terminated
  * properly when receiving these signals.
  */
 void	setup_child_signals(void)
 {
     struct sigaction	sa;
 
     sa.sa_handler = SIG_DFL;
     sigemptyset(&sa.sa_mask);
     sa.sa_flags = SIG_NO_FLAGS;
     sigaction(SIGINT, &sa, NULL);
     sigaction(SIGQUIT, &sa, NULL);
 }
 
 /**
  * @brief Handles SIGINT signals in the parent shell
  *
  * Custom signal handler for SIGINT (Ctrl+C) in the parent shell process.
  * Displays a new prompt, clears the current input line, and allows the
  * user to continue entering commands without terminating the shell.
  *
  * @param sig Signal number (unused but required
  * by signal handler interface)
  */
 void	signal_handler(int sig)
 {
     const char	*msg = "\nMinishell$ ";
 
     (void)sig;
     write(STDERR_FILENO, "\n", 1);
     write(STDERR_FILENO, msg, 12);
     rl_on_new_line();
     rl_replace_line("", 0);
 }
 
 /**
  * @brief Sets up signal handling for the parent shell process
  *
  * Configures the parent shell process to use custom handling for SIGINT
  * and to ignore SIGQUIT signals. This allows the shell to remain running
  * when these signals are received, while providing appropriate feedback
  * to the user.
  */
 void	setup_parent_signals(void)
 {
     struct sigaction	sa;
 
     sa.sa_handler = signal_handler;
     sigemptyset(&sa.sa_mask);
     sa.sa_flags = SIG_NO_FLAGS;
     sigaction(SIGINT, &sa, NULL);
     sa.sa_handler = SIG_IGN;
     sigaction(SIGQUIT, &sa, NULL);
 }
 