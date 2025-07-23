#include "../../../includes/core/minishell.h"
#include "../../../includes/core/token.h"
#include "../../../includes/core/parser.h"

int	is_special_char(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (1);
	else
		return (0);
}

int	check_next_char(const char *cmd, int *i, int *len)
{
	if (cmd[(*i) + (*len) + 1] != '\0')
	{
		if (cmd[(*i) + (*len) + 1] == cmd[(*i) + (*len)])
			return (1);
	}
	return (0);
}

int	tok_len(const char *cmd, int *i)
{
	int		len;
	char	quote;

	len = 0;
	while (cmd[(*i) + len])
	{
		if (cmd[(*i) + len] == '"' || cmd[(*i) + len] == '\'')
		{
			quote = cmd[(*i) + len];
			while (cmd[(*i) + len] && cmd[(*i) + len] != quote)
				len++;
			if (cmd[(*i) + len] == quote)
				len++;
		}
		else if (is_special_char(cmd[(*i) + len]))
		{
			if (check_next_char(cmd, i, &len))
				len++;
			len++;
			break ;
		}
		else if (ft_isspace(cmd[(*i) + len]))
			break ;
		else
			len++;
	}
	return (len);
}


