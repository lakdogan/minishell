
 #include "../../../includes/core/minishell.h"

int	is_token_valid(char *value, t_token_state state)
{
	char	c;
	int		i;

	if (state == IN_DQUOTES || state == IN_SQUOTES)
		return (1);
	i = 0;
	c = value[i];
	if (value[i + 1])
	{
		if (value[i + 1] != c)
		{
			ft_printf("minishell: syntax error near unexpected token '%c'\n", c);

			return (0);
		}
		if (value[i + 2])
		{
			ft_printf("minishell: syntax error near unexpected token '%c%c'\n", c, c);
			return (0);
		}
	}
	return (1);
}

// int	is_quote_empty(t_tokens *tokens)
// {
// 	int				i;
// 	char			*value;
// 	t_token_state	state;

// 	i = 0;
// 	value = tokens->arr[0].value;
// 	state = tokens->arr[0].state;
// 	if (state == IN_DQUOTES || state == IN_SQUOTES)
// 	{
// 		while (value[i])
// 		{
// 			if (value[i] != ' ' && value[i] != '	')
// 				return (0);
// 			i++;
// 		}
// 	}
// 	return (1);
// }
