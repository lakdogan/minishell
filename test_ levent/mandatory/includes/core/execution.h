

/**
 * @file execution.h
 * @brief Contains definitions for execution-related structures,
 * such as input/output redirection and command execution.
 *
 * TODO:
 * - Create functions to handle infile/outfile logic
 * - Process HEREDOCs (<<) and file redirections (>, >>, <)
 * - Execute commands via execve
 * - Handle builtins and distinguish from binaries
 */
#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# define BUFFER_SIZE_TEE 4096
# define FORK_ERROR -1
# define CHILD_PROCESS 0
# define WAIT_BLOCK 0
# define INVALID_FD -1
# define CMD_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define HEREDOCS_PREPARED 1
# define HEREDOCS_NOT_PREPARED 0
# define VALIDATION_SUCCESS 1
# define VALIDATION_FAILURE 0
# define DEFAULT_FILE_PERMISSION 0644
# define WRITE_ERROR -1
# define OPERATION_FAILURE 0
# define OPERATION_SUCCESS 1
# define PIPE_WRITE_END 1
# define PIPE_READ_END 0
# define STRINGS_EQUAL 0
# define GENERAL_ERROR 1
# define SIGNAL_EXIT_BASE 128
# define FALSE 0
# define CLOSE_ERROR -1
# define ABSOLUTE_PATH_CHAR '/'
# define RELATIVE_PATH_CHAR '.'
# define PATH_SEPERATOR '/'
# define ACCESS_OK 0
# define SYSCALL_ERROR -1
# define NULL_TERMINATOR '\0'
# define SIG_NO_FLAGS 0

typedef enum e_in_type
{
	INF_IN,
	INF_HEREDOC
}						t_in_type;

typedef enum e_out_type
{
	OUT_APPEND,
	OUT_TRUNC
}						t_out_type;

typedef struct s_infile
{
	t_in_type			type;		// < or <<
	char				*name;		// File name
	char				*delimeter;	// For heredoc (<<)
	struct s_infile		*next;
}						t_infile;

typedef struct s_outfile
{
	t_out_type			type;		// > or >>
	char				*name;		// File name
	struct s_outfile	*next;

}						t_outfile;

typedef struct s_exec
{
	char				*command;	// Command name
	char				**argv;		// Arguments
	t_infile			*infiles;	// Input redirections
	t_outfile			*outfiles;	// Output redirections
	int					heredoc_fd;
	bool				heredoc_prepared;
}						t_exec;

#endif
