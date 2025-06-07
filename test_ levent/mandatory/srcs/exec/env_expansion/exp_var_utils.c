#include "../../../includes/core/minishell.h"

char	handle_quote(char c, char quote_char)
{
	if ((c == '\'' || c == '"') && (quote_char == 0 || quote_char == c))
	{
		if (quote_char == 0)
			return (c);
		else
			return (0);
	}
	return (quote_char);
}

char	*append_char(char *result, char c, t_minishell *shell)
{
	char	tmp[2];
	char	*joined;

	tmp[0] = c;
	tmp[1] = '\0';
	if (!result)
		return (gc_strdup(shell->gc[GC_EXPAND], tmp));
	joined = gc_strjoin(shell->gc[GC_EXPAND], result, tmp);
	if (!joined)
		return (result);
	return (joined);
}

bool	should_expand_var(const char *str, int i, char quote_char)
{
	return (str[i] == '$' && str[i + 1] && (quote_char == 0
			|| quote_char == '"') && (is_var_char(str[i + 1])
			|| str[i + 1] == '?' || str[i + 1] == '$'));
}
