#include "../../../includes/core/minishell.h"
#include "../../../includes/core/token.h"
#include "../../../includes/core/parser.h"

t_token	*resize_buffer(t_token **tokens, int *cap, int t_count)
{
	t_token	*new_token;
	int		new_cap;

	new_cap = (*cap) * 2;
	new_token = malloc(new_cap * sizeof(t_token));
	if (!new_token)
	{
		if ((*tokens))
			free((*tokens));
		return (NULL);
	}
	ft_memcpy(new_token, (*tokens), t_count * sizeof(t_token));
	free((*tokens));
	(*cap) = new_cap;
	return (new_token);
}

t_token	*free_tokens(t_token *tokens, int t_count)
{
	int i;

	i = 0;
	if (!tokens)
		return (NULL);
	while (i < t_count)
	{
		if(tokens[i].value)
			free(tokens[i].value);
		i++;
	}
	free(tokens);
	return (NULL);
}

t_token	*lexer(const char *cmd)
{
	t_token		*tokens;
	int			cap;
	int			t_count;
	int			i;

	cap = 10;
	t_count = 0;
	i = 0;
	tokens = malloc(cap * sizeof(t_token));
	if (!tokens)
		return (NULL);
	while (cmd[i])
	{
		while (ft_isspace(cmd[i]))
			i++;
		if (!cmd[i])
			break ;
		if (t_count >= cap)
		{
			tokens = resize_buffer(&tokens, &cap, t_count);
			if (!tokens)
				return (NULL);
		}
		tokens[t_count] = init_token(cmd, &i, t_count);
		if (!tokens || !tokens[t_count].value)
			return (free_tokens(tokens, t_count));
		t_count++;
	}
	return (tokens);
}
