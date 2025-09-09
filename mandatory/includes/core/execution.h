/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:35:37 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/10 00:31:29 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# define BACKUP_FAILED 0
# define BACKUP_SUCCESS 1
# define FORK_FAILED 0
# define FORK_SUCCESS 1
# define IS_RIGHT_PID 1
# define IS_LEFT_PID 0
# define SKIP_BACKSLASH 1
# define GETCWD_AUTO_ALLOCATE 0
# define PIPE_FD_COUNT 2
# define INFINITE_LOOP 1
# define SUCCESSFUL_READ 0
# define READ_ERROR -1
# define FIRST_CHAR 0
# define NULL_TERMINATOR_INDEX 1
# define NEXT_CHAR_INDEX 1
# define DOLLAR_SIGN '$'
# define QUESTION_MARK '?'
# define CHAR_BUFFER_SIZE 2
# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '"'
# define NO_QUOTE 0
# define BACKSLASH '\\'
# define EMPTY_STRING ""
# define UNDERSCORE '_'
# define SPECIAL_VAR_LENGTH 2
# define NO_EXPANSION 0
# define INITIAL_CONSUMED_COUNT 0
# define SKIP_DOLLAR_SIGN 1
# define EQUALS_SIGN '='
# define EQUALS_SIGN_STR "="
# define SKIP_EQUALS_SIGN 1
# define NO_PROCESS 0
# define PATH_VAR_PREFIX "PATH="
# define PATH_PREFIX_LENGTH 5
# define PATH_DELIMITER ':'
# define IDENTIFIER_VALID 1
# define IDENTIFIER_INVALID 0
# define COMMAND_ARGS_START 1

typedef struct s_minishell	t_minishell;

typedef enum e_in_type
{
	INF_IN,
	INF_HEREDOC
}							t_in_type;

typedef enum e_out_type
{
	OUT_APPEND,
	OUT_TRUNC,
}							t_out_type;

typedef struct s_infile
{
	t_in_type				type;
	char					*name;
	char					*delimeter;
	bool					quoted_delimiter;
	struct s_infile			*next;
}							t_infile;

typedef struct s_outfile
{
	t_out_type				type;
	char					*name;
	int						fd_target;
	struct s_outfile		*next;

}							t_outfile;

typedef struct s_exec
{
	char					*command;
	char					**argv;
	char					**assignments;
	bool					*no_expand_flags;
	bool					*was_quoted_flags;
	char					**env_vars;
	t_infile				*infiles;
	t_outfile				*outfiles;
	int						heredoc_fd;
	bool					heredoc_prepared;
	bool					redirection_failed;
}							t_exec;

typedef struct s_pipe_state
{
	int						pipefd[2];
	pid_t					left_pid;
	pid_t					right_pid;
	int						stdout_backup;
	int						pipe_identity;
}							t_pipe_state;

typedef struct s_cmd_ctx
{
	const char				*input;
	char					*result;
	size_t					i;
	size_t					ri;
	bool					last_was_cmd;
	bool					after_pipe;
	const char				**cmds;
	int						cmd_count;
	char					quote;
}							t_cmd_ctx;

typedef struct s_pipe_fork_ctx
{
	t_exec					**execs;
	int						num_cmds;
	t_minishell				*shell;
	pid_t					*pids;
}							t_pipe_fork_ctx;

typedef struct s_error_context
{
	t_minishell				*shell;
	const char				*prefix;
	const char				*subject;
	const char				*message;
	int						exit_code;
}							t_error_context;

/* ------------------------------------------------------------------------- */
/* 								exec dir start 								*/
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* 								command dir start 							*/
/* ------------------------------------------------------------------------- */
/* 	~	command_dispatcher.c ~		*/
void						prepare_command_execution(t_minishell *shell,
								t_exec *exec);
void						handle_external(t_exec *exec, t_minishell *shell);
void						handle_builtin(t_exec *exec, t_minishell *shell);
/* 	~	command_utils.c ~ */
pid_t						create_child_process(t_minishell *shell);
void						process_child_status(int status,
								t_minishell *shell);
int							is_builtin(const char *cmd);
void						wait_for_process(pid_t pid, t_minishell *shell);
void						execute_builtin_or_external_cmd(t_minishell *shell,
								t_exec *exec);
/* ~ execute_left_cmd_prepare.c */
void						check_exec_outfile_and_prepare_pipes(
								t_minishell *shell,
								t_exec *exec, int *pipefd);
void						check_if_right_cmd_has_infile(t_command_tree *node,
								t_exec **right_exec, bool *right_has_input);
void						prepare_for_execute(t_minishell *shell,
								t_command_tree *node, t_exec *exec,
								int *pipefd);
/* ~ execute_left_cmd.c ~ */
void						handle_standard_left_cmds(t_minishell *shell,
								t_command_tree *node, int *pipefd);
void						setup_default_pipe_output(t_minishell *shell,
								int *pipefd);
void						execute_left_subtree(t_minishell *shell,
								t_command_tree *node, int *pipefd);
void						execute_left_cmd(t_command_tree *node,
								t_minishell *shell, int *pipefd);
/* ~ execute_right_cmd_prepare.c ~ */
void						prepare_pipe_ends(t_minishell *shell, int *pipefd);
void						setup_default_pipe_input(t_minishell *shell,
								int *pipefd);
void						check_if_infile_exists(t_minishell *shell,
								t_exec *exec, int *pipefd);
void						setup_in_and_output_redirections(t_minishell *shell,
								t_exec *exec);
void						check_exec_has_valid_data(t_minishell *shell,
								t_exec *exec);
/* ~ execute_right_cmd.c ~ */
void						handle_nested_cmds_recursively(t_minishell *shell,
								t_command_tree *node, int *pipefd);
void						handle_standard_cmds(t_minishell *shell,
								t_command_tree *node, int *pipefd);
void						execute_right_subtree(t_minishell *shell,
								t_command_tree *node, int *pipefd);
void						execute_right_cmd(t_command_tree *node,
								t_minishell *shell, int *pipefd);
/* 	~	external_executor.c ~ */
void						execute_command(t_exec *exec,
								t_minishell *minishell);
void						handle_var_expansion_exec(t_minishell *shell,
								t_exec *exec);
/* ~ handle_pipe_utils.c ~ */
void						close_file_descriptors(t_minishell *shell,
								t_pipe_state *state);
void						wait_for_children(t_minishell *shell,
								pid_t left_pid, pid_t right_pid);
void						restore_stdout(t_pipe_state *state);
void						cleanup_pipe(t_minishell *shell,
								t_pipe_state *state);
/* ~ handle_pipe.c ~ */
void						handle_pipe(t_minishell *shell,
								t_command_tree *node);
/* ~ nested_pipe_process.c ~ */
void						handle_nested_pipe(t_command_tree *node,
								t_minishell *shell);
/* ~ nested_pipe_setup.c ~ */
void						setup_nested_pipe_output(t_minishell *shell,
								int *pipefd);
void						setup_nested_pipe_input(t_minishell *shell,
								int *pipefd);
void						execute_nested_cmds(t_minishell *shell,
								t_exec *exec);
/* 	~	node_executor.c ~		*/
void						handle_exec(t_command_tree *node,
								t_minishell *minishell);
/* 	~	path_resolver.c ~		*/
char						*get_absolute_path(t_minishell *shell, char *cmd,
								char **envp);
/* 	~	path_utils.c ~			*/
int							get_path_from_env(t_minishell *shell, char **envp,
								char ***paths);
/* ------------------------------------------------------------------------- */
/* 								command dir end 							*/
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* 								core dir start 								*/
/* ------------------------------------------------------------------------- */
/* 	~	execute_tree.c ~		*/
void						execute_tree(t_command_tree *root,
								t_minishell *minishell);
/* ------------------------------------------------------------------------- */
/* 							core dir end 									*/
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* 							env_expansion dir start 						*/
/* ------------------------------------------------------------------------- */
/* ~  exp_find_env_value  ~ */
char						*find_env_value(t_minishell *minishell,
								const char *name);
/* ~  exp_single_var  ~ */
int							expand_single_var(const char *start,
								char **expanded, t_minishell *shell);
/* ~  exp_utils.c  ~ */
bool						is_var_char(char c);
int							get_var_name_len(const char *str);
char						*extract_var_name(const char *str,
								t_minishell *shell);
/* ~  exp_var_process.c  ~ */
char						*expand_variables_with_quotes(const char *str,
								t_minishell *shell);
/* ~  exp_var_utils.c  ~ */
bool						should_expand_var(const char *str, int i,
								char quote_char);
char						*append_char(char *result, char c,
								t_minishell *shell);
char						handle_quote(char c, char quote_char);
/* ~  exp_vars.c  ~ */
char						*expand_variables(const char *str,
								t_minishell *shell);
/* ------------------------------------------------------------------------- */
/* 							env_expansion dir end							*/
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* 							error dir start 								*/
/* ------------------------------------------------------------------------- */
/* 	~	error_utils_bonus.c ~		*/
void						exit_with_error(t_error_context ctx);
/* ------------------------------------------------------------------------- */
/* 							error dir end 									*/
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* 							io_utils dir start 								*/
/* ------------------------------------------------------------------------- */
/* 	~	fd_operations.c ~		*/
void						safe_dup2(t_minishell *shell, int oldfd, int newfd,
								const char *error_msg);
void						safe_close(t_minishell *shell, int fd,
								const char *error_msg);
void						check_fd_error(t_minishell *shell, int fd,
								const char *filename);
int							redirect_stdin_with_backup(t_minishell *shell,
								int new_fd, const char *error_msg);
void						restore_std_fds(t_minishell *shell,
								int stdin_backup, int stdout_backup,
								int stderr_backup);
/* 	~	pipe_operations.c ~		*/
void						close_pipe(t_minishell *shell, int *pipefd);
int							create_pipe(t_minishell *shell, int *pipefd);
/* ~ read_from_pipe.c ~ */
char						*read_from_pipe(t_minishell *shell, int pipe_fd);
/* ------------------------------------------------------------------------- */
/* 							io_utils dir end 								*/
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* 							redirection dir start 							*/
/* ------------------------------------------------------------------------- */
/* 	~	file_utils.c  ~ */
int							open_infile(t_minishell *shell, char *filename);
/* 	~	handle_redirections.c ~		*/
void						setup_input_redirections(t_minishell *shell,
								t_exec *exec, bool in_child_process);
void						setup_output_redirections(t_minishell *shell,
								t_exec *exec);
/* 	~	heredoc_content.c ~		*/
void						save_heredoc_contents(t_minishell *shell,
								t_exec *exec);
char						*collect_heredoc_content(t_minishell *shell,
								t_exec *exec, t_infile *infile);
/* ~  process_heredoc.c  ~ */
int							process_heredoc(t_minishell *shell,
								const char *delimiter, bool quoted_delimiter);
/* 	~	heredoc.c ~			*/
void						prepare_heredocs(t_minishell *shell,
								t_command_tree *node);
void						write_heredoc_to_fd(t_minishell *shell,
								t_exec *exec, char *content);
/* ------------------------------------------------------------------------- */
/* 								redirection dir end 						*/
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* 								signals dir start 							*/
/* ------------------------------------------------------------------------- */
/* 	~	signal_handlers.c ~		*/
void						setup_child_signals(void);
void						signal_handler(int sig);
void						setup_parent_signals(void);
/* ------------------------------------------------------------------------- */
/* 							signals dir end 								*/
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/*							exec dir end									*/
/* ------------------------------------------------------------------------- */

int							execute_pipe_chain(t_exec **execs, int num_cmds,
								t_minishell *shell);
t_exec						**build_exec_array_from_tree(t_command_tree *node);
int							setup_heredoc_redirection(t_minishell *shell,
								t_exec *exec);
void						restore_stdin(t_minishell *shell, int stdin_backup);
void						expand_command_args(t_minishell *shell,
								t_exec *exec);
void						handle_empty_command(t_exec *exec);
void						expand_infile_names(t_minishell *shell,
								t_exec *exec);
void						expand_outfile_names(t_minishell *shell,
								t_exec *exec);
int							ft_is_whitespace_str(const char *str);
void						handle_special_cases(t_exec *exec,
								t_minishell *minishell);
void						execute_with_path(char *path, t_exec *exec,
								t_minishell *minishell);
void						execute_with_path_lookup(t_exec *exec,
								t_minishell *minishell);

int							handle_empty_cmd_with_redir(t_exec *exec,
								t_minishell *minishell);
void						expand_tilde_in_args(t_exec *exec,
								t_minishell *minishell);
int							execute_builtin_cmd(t_exec *exec,
								t_minishell *minishell);
void						execute_command_from_pipe(t_exec *exec,
								t_minishell *shell);
void						setup_pipe_redirections(int i, int num_cmds,
								int prev_fd, int *pipefd);
void						update_parent_pipes(int *prev_fd, int *pipefd,
								int is_last_cmd);
void						wait_for_pipe_children(pid_t *pids, int num_cmds,
								t_minishell *shell);
bool						is_command_token(const char *str, const char **cmds,
								int cmd_count);
char						handle_cmd_quote_state(char current_quote,
								char input_char);
void						handle_pipe_character(t_cmd_ctx *ctx);

int							validate_command_tree(t_command_tree *root);
void						handle_invalid_tree(t_command_tree *root,
								t_minishell *minishell);
bool						is_assignment_only_command(t_command_tree *root);

bool						has_heredoc_redirection(t_exec *exec);
bool						has_heredoc_in_tree(t_minishell *minishell,
								t_command_tree *node);
void						prepare_heredocs_if_needed(t_minishell *minishell,
								t_command_tree *root);
void						prepare_tree_execution(t_command_tree *root,
								t_minishell *minishell);

void						execute_node_by_type(t_command_tree *node,
								t_minishell *minishell);
void						wait_for_processes(t_minishell *minishell);

void						handle_assignment_command(t_command *cmd,
								t_minishell *shell);
void						execute_subshell(t_command_tree *node,
								t_minishell *minishell);
void						setup_input_redirections(t_minishell *shell,
								t_exec *exec, bool is_pipe);
void						setup_output_redirections(t_minishell *shell,
								t_exec *exec);
void						handle_redirections_and_exit(t_exec *exec,
								t_minishell *shell);
void						handle_builtin_and_exit(t_exec *exec,
								t_minishell *shell);
void						exec_external_command(t_exec *exec,
								t_minishell *shell);
int							ft_execvp(const char *file, char *const argv[],
								char *const envp[]);
char						*find_in_path(const char *file, char **envp);
char						**list_to_envp_array(t_gc *gc, t_list *envp);
char						*get_path_value(char **envp);
char						*join_dir_and_file(const char *dir,
								const char *slash_cmd);
char						*try_path_dir(const char *dir,
								const char *slash_cmd);
char						*search_path_dirs(const char *paths,
								const char *slash_cmd);
void						child_subshell(t_command_tree *node,
								t_minishell *minishell, int pipe_fd[2]);
void						unset_env_var(const char *key,
								t_minishell *minishell);
#endif
