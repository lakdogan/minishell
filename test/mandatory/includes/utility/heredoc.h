

/**
 * @file heredoc.h
 * @brief Provides functions for handling
 * here-document (<<) redirections.
 *        Supports reading input until a
 * specified delimiter, handling quotes,
 *        and storing temporary input for command execution.
 *
 * TODO:
 * - Implement handle_heredoc(t_infile *infile)
 *   → Read from STDIN until the delimiter is found
 *   → Support quoted delimiters (no expansion vs expansion)

 * - Manage temporary files or pipes for heredoc storage
 * - Expand variables (if delimiter is unquoted)
 * - Prevent heredoc from being saved in history
 * - Handle Ctrl-C during heredoc input
 * - Clean up all heredoc temp files during exit
 */
#ifndef HEREDOC_H
# define HEREDOC_H

#endif