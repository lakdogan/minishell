#ifndef PARSE_TYPES_H
# define PARSE_TYPES_H

// Include the core command tree definitions and avoid duplicate type definitions.
# include "../core/command_tree.h"  

// If t_command is missing in command_tree.h, define it here.
#ifndef T_COMMAND_DEFINED
# define T_COMMAND_DEFINED
typedef struct s_command {
    char    **argv;      // Command arguments.
    char    **env_vars;  // Environment variable assignments.
    // Add additional fields if needed.
} t_command;
#endif

// t_command_tree is assumed to be defined in core/command_tree.h.

#endif
