#include "../../../includes/core/minishell.h"

t_token_type	get_tok_type(char *value)
{
	if (!value)
		return (WORD);
	if (ft_strncmp(value, ">>", 2) == 0)
		return (APPEND);
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
		return (L_PAREN);
	if (ft_strncmp(value, ")", 1) == 0)
		return (R_PAREN);
	if (ft_strncmp(value, "&", 1) == 0)
		return (AND_IF);
	return (WORD);
}

t_token_state	get_tok_state(char *value, int len)
{
	if (value[0] == '\'' || value[0] == '"')
	{
		if (value[0] == value[len - 1])
		{
			if (value[0] == '"')
				return (IN_DQUOTES);
			else
				return (IN_SQUOTES);
		}
		else
			return (UNCLOSED_QUOTES);
	}
	return (GENERAL);
}

t_token	init_token_default(void)
{
	t_token	new_token;

	new_token.value = NULL;
	new_token.type = END;
	new_token.pos = -1;
	new_token.state = GENERAL;
	return (new_token);
}

t_token	init_token(const char *cmd, int *i, const int t_count, t_gc *gc)
{
	int				len;
	t_token_type	type;
	t_token	new_token;

	new_token = init_token_default();
	len = tok_len(cmd, i);
	if (len == -1)
		return (new_token);
	new_token.value = gc_substr(gc, cmd, *i, len);
	if (!new_token.value)
		return (new_token);
	new_token.state = get_tok_state(new_token.value, len);
	if (new_token.state == UNCLOSED_QUOTES)
		return (init_token_default());
	new_token.type = get_tok_type(new_token.value);
	type = new_token.type;
	if (type != WORD && type != L_PAREN && type != R_PAREN)
	{
		if (!is_token_valid(new_token.value))
			return (init_token_default());
	}
	new_token.pos = t_count;
	(*i) += len;
	return (new_token);
}
