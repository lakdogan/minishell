#include "../../../includes/core/minishell.h"
#include "../../../includes/core/token.h"
#include "../../../includes/core/parser.h"

t_token_type	get_tok_type(char *value)
{
	if (!value)
		return (WORD);
	if (ft_strncmp(value, ">>", 2) == 0)
		return (APPEND);
	if (ft_strncmp(value, "<<", 2) == 0)
		return (HEREDOC);
	if (ft_strncmp(value, "<<", 2) == 0)
		return (HEREDOC);
	if (ft_strncmp(value, "&&", 2) == 0)
		return (AND_IF);
	if (ft_strncmp(value, "||", 2) == 0)
		return (OR);
	if (ft_strncmp(value, "|", 1) == 0)
		return (PIPE);
	if (ft_strncmp(value, ">", 1) == 0)
		return (OUTFILE);
	if (ft_strncmp(value, "<", 1) == 0)
		return (INFILE);
	if (ft_strncmp(value, "(", 1) == 0)
		return (PARENTHESSIS);
	if (ft_strncmp(value, "&", 1) == 0)
		return (AND_IF);
	return (WORD);
}

char	is_special_char(char c)
{
	if (c == '|' || c == '>' || c == '<' || '"' || c == '\'')
		return (1);
	else
		return (0);
}

int	tok_len(const char *cmd, int *i)
{
	int len;

	len = 0;
	while (cmd[(*i) + len] && !ft_isspace(cmd[(*i) + len]))
	{
		if (is_special_char(cmd[(*i) + len]))
			break ; //TODO quote funktion um die laenge von den quote zu berechnen;
		len++;
	}
	return (len);
}

t_token	init_token(const char *cmd, int *i, const int t_count, t_gc *gc)
{
	int		len;
	int		index;
	t_token	new_token;

	index = 0;
	len = tok_len(cmd, i);
	new_token.value = gc_malloc(gc, (len + 1) * sizeof(char));
	if (!new_token.value)
	{
		new_token.type = END;
		new_token.pos = -1;
		new_token.state = GENERAL;
		return (new_token);
	}
	while (index < len)
	{
		new_token.value[index] = cmd[(*i) + index];
		index++;
	}
	new_token.value[len] = '\0';
	new_token.state = //TODO: get_tok_state;
	new_token.type = get_tok_type(new_token.value);
	new_token.pos = t_count;
	(*i) += len;
	return (new_token);
}
