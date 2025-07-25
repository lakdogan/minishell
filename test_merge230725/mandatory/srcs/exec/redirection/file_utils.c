#include "../../../includes/core/minishell.h"

/**
 * @brief Opens a file for input redirection
 *
 * Opens the specified file in read-only mode and checks for potential
 * errors such as file not found or permission denied. If an error occurs,
 * appropriate error handling is performed.
 *
 * @param filename Name of the file to open
 * @return int File descriptor for the opened file
 * @note This function will terminate the program if the file cannot be opened
 */
// int open_infile(t_minishell *shell, char *filename)
// {
//     int fd;

//     fd = open(filename, O_RDONLY);
//     if (fd < 0) {
//         // Just set exit code but don't terminate
//         shell->exit_code = 1;
//     }
//     return (fd);
// }

// int open_infile(t_minishell *shell, char *filename)
// {
//     int fd;
    
//     if (!filename)
//         return (INVALID_FD);
        
//     fd = open(filename, O_RDONLY);
//     if (fd == -1)
//     {
//         // Format error message like bash does
//         write(STDERR_FILENO, "error: ", 8);
//         write(STDERR_FILENO, filename, ft_strlen(filename));
//         write(STDERR_FILENO, ": No such file or directory\n", 28);
        
//         // Set the exit code to 1 explicitly 
//         shell->exit_code = 1;
//         return (INVALID_FD);
//     }
//     return (fd);
// }

int open_infile(t_minishell *shell, char *filename)
{
    int fd;
    
    if (!filename)
        return (INVALID_FD);
        
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        // Format error message like bash actually does
        if (shell->current_command)
            write(STDERR_FILENO, shell->current_command, ft_strlen(shell->current_command));
        else
            write(STDERR_FILENO, "minishell", 9);
            
        write(STDERR_FILENO, ": ", 2);
        write(STDERR_FILENO, filename, ft_strlen(filename));
        write(STDERR_FILENO, ": No such file or directory\n", 28);
        
        // Set the exit code to 1 explicitly
        shell->exit_code = 1;
        return (INVALID_FD);
    }
    return (fd);
}