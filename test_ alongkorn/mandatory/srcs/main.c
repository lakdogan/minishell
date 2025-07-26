
#include "../includes/core/minishell.h"

t_command_tree	*setup_ast(char *line, t_minishell *shell)
{
	t_tokens		*tokens;
	t_command_tree	*ast;

	tokens = lexer(line, shell);
	if (!lexer)
		gc_cleanup(shell->gc[GC_COMMAND]);
	else
		ast = start_parser(tokens, shell->gc[GC_COMMAND], shell);
	if (!ast)
		return (NULL);
	return (ast);
}

void	start_exec(t_command_tree *ast, t_minishell *shell)
{

}

int	main(int argc, char **argv, char **env)
{
	char			*line;
	t_minishell		shell;

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
				start_exec(shell.root, &shell);
		}
		free(line);
	}
	gc_cleanup(&shell.gc);
	return (&shell.exit_code);
}
