// ...existing includes and code...

typedef struct s_command
{
    char            **argv;      // Command arguments
    char            **env_vars;  // Environment variables for this command
    int             argc;        // Argument count
    t_redirections  *rd;         // Redirections
    int             fd_in;       // Input file descriptor
    int             fd_out;      // Output file descriptor
    int             pipe_in;     // Pipe input descriptor 
    int             pipe_out;    // Pipe output descriptor
    int             is_builtin;  // Is this a builtin command?
    t_builtin_id    builtin_id;  // Builtin identifier
    // ...other existing fields...
} t_command;

// ...rest of existing code...
