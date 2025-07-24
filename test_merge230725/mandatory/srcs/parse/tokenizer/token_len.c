
#include "../../../includes/core/minishell.h"

int check_for_paren(const char *cmd, int *i, int len)
{
	if (cmd[(*i) + len] == '(' || cmd[(*i) + len] == ')')
		return (1);
	else
		return (0);
}

int	check_for_qoutes(const char *cmd, int *i, int *len)
{
	char	quote;

	quote = cmd[(*i) + (*len)];
	(*len)++;
	while (cmd[(*i) + (*len)] && cmd[(*i) + (*len)] != quote)
		(*len)++;
	if (cmd[(*i) + (*len)] == quote)
	{
		(*len)++;
		return (1);
	}
	return (0);
}

int	tok_len(const char *cmd, int *i)
{
	int		len;

	len = 0;

	if (cmd[*i] == '>' || cmd[*i] == '<')
	{
		if (cmd[*i] == cmd[*i + 1])
			return (2);
		else
			return (1);
	}
	while (cmd[(*i) + len])
	{
		if (cmd[(*i) + len] == '"' || cmd[(*i) + len] == '\'')
		{
			if (!check_for_qoutes(cmd, i, &len))
				break ;
		}
		else if (ft_isspace(cmd[(*i) + len]))
			break ;
		else if (check_for_paren(cmd, i, len) && len == 0)
		{
			len++;
			break ;
		}
		else if (check_for_paren(cmd, i, len) && len > 0)
			break ;
		else
			len++;
	}
	return (len);
}
