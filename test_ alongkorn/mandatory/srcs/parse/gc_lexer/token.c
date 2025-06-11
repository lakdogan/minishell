
#include "../../../includes/core/minishell.h"
#include "../../../includes/core/token.h"
#include "../../../includes/core/parser.h"


t_token	*resize_array(t_token *tokens, int *cap, t_gc *gc[GC_TOKEN])
{
	t_token	*new;

	new = gc_realloc(gc[GC_TOKEN], tokens, ((*cap) + 10) * sizeof(t_token));
	if (!new)
		return (NULL);
	(*cap) += 10;
	return (new);
}

static int	lexer_loop(t_token **tokens, const char *cmd, int *cap, t_gc *gc[GC_TOKEN])
{
	int		i;
	int		t_count;

	i = 0;
	t_count = 0;
	while (cmd[i])
	{
		while (ft_isspace(cmd[i]))
			i++;
		if (!cmd[i])
			break ;
		if (t_count >= (*cap))
		{
			(*tokens) = resize_array(*tokens, cap, gc);
			if (!(*tokens))
				return (-1);
		}
		(*tokens)[t_count] = init_token(cmd, &i, t_count, gc);
		if (!(*tokens)[t_count].value)
			return (-1);
		t_count++;
	}
	return (t_count);
}

t_token	*lexer(const char *cmd, t_minishell *shell)
{
	t_token	*tokens;
	int		count;
	int		cap;

	cap = 10;
	tokens = gc_malloc(shell->gc[GC_TOKEN], (cap + 1) * sizeof(t_token));
	if (!tokens)
		return (NULL);
	count = lexer_loop(&tokens, cmd, &cap, shell->gc);
	if (count <= 0)
		return (NULL);
	tokens[count].value = NULL;
	tokens[count].type = END;
	return (tokens);
}
