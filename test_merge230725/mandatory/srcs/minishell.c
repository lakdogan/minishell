
#include "../includes/core/minishell.h"

t_command_tree *setup_ast(char *line, t_minishell *shell)
{
	t_tokens *tokens;
	t_command_tree *ast;

	tokens = lexer(line, shell);
	if (!tokens)
	{
		//error_msg_paren();
        shell->exit_code = 2;
		return (NULL);
	}
	ast = start_parser(tokens, shell->gc[GC_COMMAND], shell);
	if (!ast)
    {
        shell->exit_code = 2;
		return (NULL);
    }
	return (ast);
}

// int main(int argc, char **argv, char **env)
// {
// 	t_minishell shell;
// 	(void)argc;
// 	(void)argv;

// 	init_minishell(&shell, env);
// 	while (1)
// 	{
// 		setup_parent_signals();
// 		if (isatty(fileno(stdin)))
// 		{
// 			shell.line = readline("minishell$ ");
// 			if (!shell.line)
// 			{
// 				// ft_printf("exit\n");
// 				break;
// 			}
// 			if (shell.line[0] != '\0')
// 			{
// 				add_history((const char *)shell.line);
// 				shell.root = setup_ast(shell.line, &shell);
// 				if (shell.root)
// 					execute_tree(shell.root, &shell);
// 			}
// 		}
// 		free(shell.line);
// 	}
// 	return (gc_cleanup(shell.gc), shell.exit_code);
// }

int main(int argc, char **argv, char **env)
{
    t_minishell shell;
    // Du brauchst eine Variable für die Zeile, die von gnl kommt
    char *gnl_line;

    (void)argc;
    (void)argv;

    init_minishell(&shell, env);
    while (1)
    {
        setup_parent_signals();

        // ==========================================================
        // NEUE LOGIK FÜR DEN TESTER
        // ==========================================================
        if (isatty(fileno(stdin)))
        {
            // Interaktiver Modus: Wir sind im Terminal
            shell.input = readline("minishell$ ");
        }
        else
        {
            // Nicht-interaktiver Modus: Input kommt von einer Pipe (dem Tester)
            gnl_line = get_next_line(fileno(stdin));
            if (gnl_line)
            {
                // Entferne das \n am Ende, das gnl mitliefert
                shell.input = ft_strtrim(gnl_line, "\n");
                free(gnl_line);
            }
            else
            {
                shell.input = NULL; // GNL ist am Ende angekommen
            }
        }
        // ==========================================================

        if (!shell.input) // Ctrl+D im interaktiven Modus oder Ende der Tester-Eingabe
        {
            // ft_putstr_fd("exit\n", STDOUT_FILENO); // Wie gefordert auskommentiert
            break;
        }

        if (shell.input[0] != '\0')
        {
            add_history((const char *)shell.input);
            shell.root = setup_ast(shell.input, &shell);
            if (shell.root)
                execute_tree(shell.root, &shell);
        }

        // Korrektes Aufräumen für die nächste Runde
        free(shell.input);
    }
	gc_cleanup(shell.gc);
    return (shell.exit_code);
}
