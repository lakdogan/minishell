

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
}						t_exec;

#endif
