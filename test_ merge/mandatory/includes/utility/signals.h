

/**
 * @file signals.h
 * @brief Provides functions to handle Unix signals such as SIGINT and SIGQUIT
 *        for interactive shell behavior,
 * 	including disabling terminal echo control,
 *        resetting signal handlers, and ensuring proper user interaction.
 *
 * TODO:
 * - Implement init_signals(void)
 *   → Set up SIGINT and SIGQUIT handlers (interactive shell behavior)
 *
 * - Implement sigint_handler(int sig)
 *   → Handle Ctrl-C: reset line, show new prompt
 *
 * - Implement sigquit_handler(int sig)
 *   → Handle Ctrl-\\: usually ignored
 *
 * - Implement disable_echoctl(void)
 *   → Disable ^C and ^\\ being printed in the terminal
 *
 * - Optional: reset_terminal(void)
 *   → Restore terminal settings on exit or exec
 *
 * - Ensure signal behavior matches bash during:
 *   → normal prompt
 *   → during heredoc
 *   → during forked child execution
 */
#ifndef SIGNALS_H
# define SIGNALS_H

#endif