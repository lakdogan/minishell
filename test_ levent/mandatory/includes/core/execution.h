

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
	t_in_type type;  // < or <<
	char *name;      // File name
	char *delimeter; // For heredoc (<<)
	struct s_infile		*next;
}						t_infile;

typedef struct s_outfile
{
	t_out_type type; // > or >>
	char *name;      // File name
	struct s_outfile	*next;

}						t_outfile;

typedef struct s_exec
{
	char *command;       // Command name
	char **argv;         // Arguments
	t_infile *infiles;   // Input redirections
	t_outfile *outfiles; // Output redirections
	int					heredoc_fd;
	bool				heredoc_prepared;
}						t_exec;

/* srcs dir start */
/* ------------------------------------------------------------------------- */
/* builtins */
// builtins.c
int						exec_builtin(t_exec *cmd, t_minishell *minishell);
// cd.c
int						ft_cd(char **argv, t_minishell *minishell);
// echo.c
int						ft_echo(char **argv);
// env.c
int						ft_env(t_minishell *minishell);
// exit.c
int						ft_exit(char **argv, t_minishell *minishell);
// export_print.c
void					print_export(t_list *envp);
// export_utils.c
void					add_new_env(char *key, char *value,
							t_minishell *minishell);
void					update_existing_env(t_env *env, char *key, char *value);
// export.c
int						ft_export(char **argv, t_minishell *minishell);
// pwd.c
int						ft_pwd(void);
// unset.c
int						ft_unset(char **argv, t_minishell *minishell);
/* ------------------------------------------------------------------------- */
/* exec dir start */
/* ------------------------------------------------------------------------- */
/* command dir start */
// command_dispatcher.c
void					handle_external(t_exec *exec, t_minishell *minishell);
void					handle_builtin(t_exec *exec, t_minishell *minishell);
// command_utils.c
pid_t					create_child_process(void);
void					process_child_status(int status,
							t_minishell *minishell);
int						is_builtin(const char *cmd);
void					wait_for_process(pid_t pid, t_minishell *minishell);
// external_executor.c
void					execute_command(t_exec *exec, t_minishell *minishell);
// path_resolver.c
char					*get_absolute_path(char *cmd, char **envp);
// path_utils.c
int						get_path_from_env(char **envp, char ***paths);
/* command dir end */
/* ------------------------------------------------------------------------- */
/* core dir start */

/* core dir end */
/* ------------------------------------------------------------------------- */
/* error dir start */

/* error dir end */
/* ------------------------------------------------------------------------- */
/* io_utils dir start */
// fd_operations.c
void					safe_dup2(int oldfd, int newfd, const char *error_msg);
void					safe_close(int fd, const char *error_msg);
void					check_fd_error(int fd, const char *filename);
int						redirect_stdin_with_backup(int new_fd,
							const char *error_msg);
void					restore_std_fds(int stdin_backup, int stdout_backup);
// pipe_operations.c
char					*read_from_pipe(int pipe_fd);
void					close_pipe(int *pipefd);
int						create_pipe(int *pipefd);
/* io_utils dir end */
/* ------------------------------------------------------------------------- */
/* redirection dir start */
// handle_redirections.c
void					setup_input_redirections(t_exec *exec);
void					setup_output_redirections(t_exec *exec);
// heredoc_content.c
void					save_heredoc_contents(t_exec *exec);
char					*collect_heredoc_content(t_exec *exec,
							t_infile *infile);
// heredoc.c
int						process_heredoc(const char *delimiter);
void					prepare_heredocs(t_command_tree *node);
void					write_heredoc_to_fd(t_exec *exec, char *content);
// redirection_files.c
int						create_temp_file(void);
int						open_infile(char *filename);
/* redirection dir end */
/* ------------------------------------------------------------------------- */
/* signals dir start */
// signal_handlers.c
void					setup_child_signals(void);
void					signal_handler(int sig);
void					setup_parent_signals(void);
/* signals dir end */
/* ------------------------------------------------------------------------- */

#endif
