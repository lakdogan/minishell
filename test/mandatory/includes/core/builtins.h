

/**
 * @file builtins.h
 * @brief Declares all builtin shell commands and helper functions
 *        such as echo, cd, env, export, unset, pwd, and exit.
 *
 * TODO:
 * - Implement each builtin command to match bash behavior
 *   - echo: handle -n, multiple args, newline suppression
 *   - cd: handle HOME, OLDPWD, error handling
 *   - pwd: print current working directory
 *   - export: add/update env variables with correct formatting
 *   - unset: remove variables safely
 *   - env: print current environment
 *   - exit: exit shell with proper code, handle numeric args
 *
 * - Implement:
 *   - is_builtin(): check if command is a builtin
 *   - exec_builtin(): dispatch and execute builtin from AST/exec node
 */
#ifndef BUILTINS_H
# define BUILTINS_H

#endif
