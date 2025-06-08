#include "../../../includes/core/minishell.h"

char	handle_quote(char c, char quote_char)
{
	if ((c == SINGLE_QUOTE || c == DOUBLE_QUOTE) && (quote_char == NO_QUOTE
			|| quote_char == c))
	{
		if (quote_char == NO_QUOTE)
			return (c);
		else
			return (NO_QUOTE);
	}
	return (quote_char);
}

char	*append_char(char *result, char c, t_minishell *shell)
{
	char	tmp[CHAR_BUFFER_SIZE];
	char	*joined;

	tmp[FIRST_CHAR] = c;
	tmp[NULL_TERMINATOR_INDEX] = NULL_TERMINATOR;
	if (!result)
		return (gc_strdup(shell->gc[GC_EXPAND], tmp));
	joined = gc_strjoin(shell->gc[GC_EXPAND], result, tmp);
	if (!joined)
		return (result);
	return (joined);
}

bool	should_expand_var(const char *str, int i, char quote_char)
{
	return (str[i] == DOLLAR_SIGN && str[i + NEXT_CHAR_INDEX]
		&& (quote_char == NO_QUOTE || quote_char == DOUBLE_QUOTE)
		&& (is_var_char(str[i + NEXT_CHAR_INDEX])
			|| str[i + NEXT_CHAR_INDEX] == QUESTION_MARK
			|| str[i + NEXT_CHAR_INDEX] == DOLLAR_SIGN));
}
