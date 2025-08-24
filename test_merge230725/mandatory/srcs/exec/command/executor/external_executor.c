/**
 * @file external_executor.c
 * @brief Implementation of external command execution functionality
 *
 * This file contains the implementation for executing external (non-builtin)
 * commands in the shell. It handles resolving command paths, executing
 * the command via execve(),
 *	and proper error handling for various failure cases.
 *
 *
 */

#include "../../../../includes/core/minishell.h"

// Helper function: Apply environment variables to the current process


int ft_is_whitespace_str(const char *str)
{
    if (!str)
        return 1;  // NULL is considered whitespace

    while (*str) {
        if (!ft_isspace(*str))
            return 0;  // Found non-whitespace
        str++;
    }
    return 1;  // Only whitespace
}

void	handle_var_expansion_exec(t_minishell *shell, t_exec *exec)
{
	int			i;
	t_infile	*infile;
	t_outfile	*outfile;

	if (exec->argv)
	{
		i = 0;
		while (exec->argv[i])
		{
			if (!exec->no_expand_flags || !exec->no_expand_flags[i])
			{
				exec->argv[i] = expand_variables_with_quotes(exec->argv[i], shell);
				if (i == 0)
					exec->command = exec->argv[0];
			}
			i++;
		}
	}
	if (exec->argv[0] && (!*exec->argv[0] || ft_is_whitespace_str(exec->argv[0])))
	{
		i = 0;
		while (exec->argv[i + 1])
		{
			exec->argv[i] = exec->argv[i + 1];
			if (exec->no_expand_flags)
				exec->no_expand_flags[i] = exec->no_expand_flags[i + 1];
			i++;
		}
		exec->argv[i] = NULL;
		if (exec->argv[0])
			exec->command = exec->argv[0];
		else
			exec->command = NULL;
	}

	infile = exec->infiles;
	while (infile)
	{
		infile->name = expand_variables_with_quotes(infile->name, shell);
		infile = infile->next;
	}
	outfile = exec->outfiles;
	while (outfile)
	{
		outfile->name = expand_variables_with_quotes(outfile->name, shell);
		outfile = outfile->next;
	}
}

/**
 * @brief Executes an external command in a child process
 *
 * Resolves the absolute path of the command using
 * the PATH environment variable,
 * then attempts to execute it using execve(). Handles various error conditions
 * such as command not found and permission
 *	denied with appropriate error messages
 * and exit codes. This function never
 *  returns if successful as execve() replaces
 * the current process image.
 *
 * @param exec Pointer to the command execution
 * structure containing command and arguments
 * @param minishell Pointer to the shell state structure
 * containing environment variables
 * @note This function terminates the process on both success and failure
 */
// void	execute_command(t_exec *exec, t_minishell *minishell)
// {
// 	char	*abs_path;
// 	struct	stat path_stat;

// 	handle_var_expansion_exec(minishell, exec);
// 	if (!exec->command || !*exec->command || ft_is_whitespace_str(exec->command))
// 	{
// 		exit(CMD_NOT_FOUND);
// 	}
// 	abs_path = get_absolute_path(minishell, exec->command, minishell->envp_arr);
// 	if (!abs_path)
// 		exit_with_error(minishell, "command not found: ", exec->command,
// 			CMD_NOT_FOUND);
// 	if (stat(abs_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
// 		exit_with_error(minishell, "is a directory: ", exec->command, PERMISSION_DENIED);
// 	else if (errno == EACCES)
// 		exit_with_error(minishell, "permission denied: ", exec->command,
// 			PERMISSION_DENIED);

// 	execve(abs_path, exec->argv, minishell->envp_arr);
// 	if (errno == EISDIR)
// 		exit_with_error(minishell, "is a directory: ", exec->command, PERMISSION_DENIED);
// 	else
// 		exit_with_error(minishell, "execve", strerror(errno), CMD_NOT_FOUND);
// }

// void	execute_command(t_exec *exec, t_minishell *minishell)
// {
//     struct stat path_stat;
//     char *abs_path = NULL;

//     handle_var_expansion_exec(minishell, exec);
//     if (!exec->command || !*exec->command || ft_is_whitespace_str(exec->command))
//     {
//         exit(CMD_NOT_FOUND);
//     }

//     // FIX: If command contains '/', do not search PATH, use as-is
//     if (ft_strchr(exec->command, '/'))
//     {
//         abs_path = exec->command;
//         if (stat(abs_path, &path_stat) != 0)
//             exit_with_error(minishell, abs_path, "No such file or directory", CMD_NOT_FOUND);
//         if (S_ISDIR(path_stat.st_mode))
//             exit_with_error(minishell, abs_path, "is a directory", PERMISSION_DENIED);
//         if (access(abs_path, X_OK) != 0)
//             exit_with_error(minishell, abs_path, "Permission denied", PERMISSION_DENIED);
//     }
//     else
//     {
//         abs_path = get_absolute_path(minishell, exec->command, minishell->envp_arr);
//         if (!abs_path)
//             exit_with_error(minishell, "command not found: ", exec->command, CMD_NOT_FOUND);
//         if (stat(abs_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
//             exit_with_error(minishell, "is a directory: ", exec->command, PERMISSION_DENIED);
//         else if (errno == EACCES)
//             exit_with_error(minishell, "permission denied: ", exec->command, PERMISSION_DENIED);
//     }

//     execve(abs_path, exec->argv, minishell->envp_arr);
//     if (errno == EISDIR)
//         exit_with_error(minishell, "is a directory: ", exec->command, PERMISSION_DENIED);
//     else
//         exit_with_error(minishell, "execve", strerror(errno), CMD_NOT_FOUND);
// }
void	execute_command(t_exec *exec, t_minishell *minishell)
{
    struct stat path_stat;
    char *abs_path = NULL;

    handle_var_expansion_exec(minishell, exec);
    if (!exec->command || !*exec->command || ft_is_whitespace_str(exec->command))
        exit(CMD_NOT_FOUND);

    if (ft_strchr(exec->command, '/'))
    {
        abs_path = exec->command;
		if (!abs_path)
        	exit_with_error(minishell, "minishell: ", exec->command, ": command not found", CMD_NOT_FOUND); 
        if (stat(abs_path, &path_stat) != 0)
            exit_with_error(minishell, "minishell: ", abs_path, ": No such file or directory", PERMISSION_DENIED);
        if (S_ISDIR(path_stat.st_mode))
            exit_with_error(minishell, "minishell: ", abs_path, ": is a directory", PERMISSION_DENIED);
        if (access(abs_path, X_OK) != 0)
            exit_with_error(minishell, "minishell: ", abs_path, ": Permission denied", PERMISSION_DENIED);
    }
    else
    {
        abs_path = get_absolute_path(minishell, exec->command, minishell->envp_arr);
        if (!abs_path)
            exit_with_error(minishell, "minishell: ", exec->command, ": command not found", CMD_NOT_FOUND);
        if (stat(abs_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
            exit_with_error(minishell, "minishell: ", exec->command, ": is a directory", PERMISSION_DENIED);
        else if (errno == EACCES)
            exit_with_error(minishell, "minishell: ", exec->command, ": permission denied", PERMISSION_DENIED);
    }

    execve(abs_path, exec->argv, minishell->envp_arr);
    if (errno == EISDIR)
        exit_with_error(minishell, "minishell: ", exec->command, ": is a directory", PERMISSION_DENIED);
    else
        exit_with_error(minishell, "minishell: ", exec->command, ": execve error", CMD_NOT_FOUND);
}
