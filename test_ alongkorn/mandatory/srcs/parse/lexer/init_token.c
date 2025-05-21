#include "../../include/core/minishell.h"
#include "../../include/core/token.h"
#include "../../include/utility/parser.h"

char	is_special_char(char c)
{
	if (c == '|' || c == '>' || c == '<' || '"' || c == '\'')
		return (1);
	else
		return (0);
}

int	get_token_len(const char *cmd, int *i)
{
	int len;

	len = 0;
	while (cmd[(*i) + len] && !ft_isspace(cmd[(*i) + len]))
	{
		if (is_special_char(cmd((*i))))
			break ;
		len++;
	}
	return (len);
}

t_token	*init_token(const char *cmd, int *i, const int t_count)
{
	int		len;
	int		index;
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	index = -1;
	len = get_token_len(cmd, i);
	new_token->value = malloc((len + 1) * sizeof(char));
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	while (++index < len)
		new_token->value[index] = cmd[(*i) + index];
	new_token->value[len] = '\0';
	(*i) += len;
	new_token->type = //TODO: get_token_type function for the typ of the token.
	new_token->state = //TODO: get_token_state function for the state of the token.
	new_token->pos = t_count;
	return (new_token);
}
