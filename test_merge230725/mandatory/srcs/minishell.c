/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 09:29:33 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/24 18:56:51 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/core/minishell.h"

t_command_tree	*setup_ast(char *line, t_minishell *shell, int arc, char **arv)
{
	t_tokens		*tokens;
	t_command_tree	*ast;

	(void)arc;
	(void)arv;
	tokens = lexer(line, shell);
	if (!tokens)
	{
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

char	*get_shlvl(const char *lvl, t_gc *gc)
{
	int		shlvl;
	char	*tmp;
	char	*new_lvl;

	shlvl = ft_atoi(lvl + 6) + 1;
	if (shlvl == 1)
		shlvl = 2;
	tmp = ft_itoa(shlvl);
	if (!tmp)
		return (NULL);
	new_lvl = gc_strjoin(gc, "SHLVL=", tmp);
	free(tmp);
	return (new_lvl);
}

int	increment_shlvl(t_minishell *shell)
{
	int		i;
	char	**env;
	char	*shlvl;

	env = shell->envp_arr;
	i = 0;
	while (env[i])
	{
			if (strncmp(env[i], "SHLVL=", 6) == 0)
			{
				shlvl = get_shlvl(env[i], shell->gc[GC_LVL]);
				if (!shlvl)
					return (0);
				env[i] = shlvl;
				return (1);
			}
			i++;
		}
	return (0);
}

int	non_interactive_mode(t_minishell *shell, int arc, char **arv)
{
	char *line;

	while (1)
	{
		line = get_next_line(fileno(stdin));
		if (!line)
			break ;
		shell->input = ft_strtrim(line, "\n");
		free(line);
		if (!shell->input || shell->input[0] == '\0')
			shell->input = NULL;
		shell->root = setup_ast(shell->input, shell, arc, arv);
		if (!shell->root)
			return (shell->exit_code);
		execute_tree(shell->root, shell);
	}
	return (shell->exit_code);
}

int main(int arc, char **arv, char **env)
{
	t_minishell	shell;

	init_minishell(&shell, env);
	if (!increment_shlvl(&shell))
		return (1);
	setup_parent_signals();
	if (isatty(fileno(stdin)))
	{
		while(1)
		{
			shell.input = readline("minishell$ ");
			if (!shell.input)
				break ;
			if (shell.input[0] != '\0')
				add_history((const char *)shell.input);
			shell.root = setup_ast(shell.input, &shell, arc, arv);
			if (!shell.root)
			return (shell.exit_code);
		execute_tree(shell.root, &shell);
		}
	}
	else
		shell.exit_code = non_interactive_mode(&shell, arc, arv);
	gc_cleanup(shell.gc);
	return (shell.exit_code);
}
