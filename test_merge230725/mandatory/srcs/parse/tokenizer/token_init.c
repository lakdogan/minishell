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
	char *orginal_value;

	new_token = init_token_default();
	// printf("%s\n", new_token.value);
	len = tok_len(cmd, i);
	if (len == -1)
		return (new_token);
	new_token.value = gc_substr(gc, cmd, *i, len);
	// printf("%s\n", new_token.value);
	if (!new_token.value)
		return (new_token);
	// changes
	orginal_value = new_token.value;
	new_token.type = get_tok_type(new_token.value);
	new_token.state = get_tok_state(new_token.value, len);
	new_token.no_expand = (new_token.state == IN_SQUOTES);
	if (new_token.type == WORD)
    {
        new_token.value = remove_quotes(orginal_value, gc);
        // DO NOT reassign the type after removing quotes!
        // Keep it as WORD
    }
    else
    {
        // For non-WORD tokens, validate if needed
        if (!is_token_valid(new_token.value, new_token.state))
            return (init_token_default());
    }
	// if (get_tok_type(orginal_value) == WORD)
	// 	new_token.value = remove_quotes(orginal_value, gc);
	// if (new_token.state == UNCLOSED_QUOTES)
	// 	return (init_token_default());
	// new_token.type = get_tok_type(new_token.value);
	// printf("Token type: %d\n", new_token.type);
	type = new_token.type;
	if (type != WORD && type != L_PAREN && type != R_PAREN)
	{
		if (!is_token_valid(new_token.value, new_token.state))
			return (init_token_default());
	}
	new_token.pos = t_count;
	(*i) += len;
	return (new_token);
}

char *remove_quotes(char *token_value, t_gc *gc)
{
    int len = ft_strlen(token_value);
    char *cleaned = gc_malloc(gc, len + 1);
    int i = 0, j = 0;
	
    if (!cleaned)
        return token_value;
    while (token_value[i]) {
        // Skip quote characters
        if (token_value[i] == '\'' || token_value[i] == '"') {
            char quote = token_value[i++];
            // Copy characters until matching quote
            while (token_value[i] && token_value[i] != quote) {
                cleaned[j++] = token_value[i++];
            }
            if (token_value[i] == quote)
                i++; // Skip closing quote
        } else {
            // Copy non-quoted characters
            cleaned[j++] = token_value[i++];
        }
    }
    cleaned[j] = '\0';
    return cleaned;
}
