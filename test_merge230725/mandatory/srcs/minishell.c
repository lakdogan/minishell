
#include "../includes/core/minishell.h"

t_command_tree	*setup_ast(char *line, t_minishell *shell)
{
	t_tokens		*tokens;
	t_command_tree	*ast;

	tokens = lexer(line, shell);
	if (!tokens)
	{
		error_msg_paren();
		return (NULL);
	}
	ast = start_parser(tokens, shell->gc[GC_COMMAND]);
	if (!ast)
		return (NULL);
	return (ast);
}

int	main(int argc, char **argv, char **env)
{
	char			*line;
	t_minishell		shell;
	(void)argc;
	(void)argv;

	init_minishell(&shell, env);
	while (1)
	{
		setup_parent_signals();
		line = readline("minishell$ ");
		if (!line)
		{
			ft_printf("exit\n");
			break ;
		}
		if (line[0] != '\0')
		{
			add_history((const char *)line);
			shell.root = setup_ast(line, &shell);
			if (shell.root)
				execute_tree(shell.root, &shell);
		}
		free(line);
	}
	return (gc_cleanup(shell.gc), shell.exit_code);
}
