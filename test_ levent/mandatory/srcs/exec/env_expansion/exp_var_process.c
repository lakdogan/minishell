#include "../../../includes/core/minishell.h"

static char	*expand_var_at_position(const char *str, int *i, t_minishell *shell)
{
	char	*expanded;
	int		consumed;

	expanded = NULL;
	consumed = expand_single_var(&str[*i], &expanded, shell);
	*i += consumed;
	return (expanded);
}

static char	*process_character(const char *str, int *i, char *quote_char,
		t_minishell *shell)
{
	char	new_quote_state;

	if (str[*i] == '\\' && str[*i + 1] != '\0' && *quote_char != '\'')
	{
		if (str[*i + 1] == '"' || str[*i + 1] == '\'' || str[*i + 1] == '\\'
			|| str[*i + 1] == '$')
		{
			(*i) += SKIP_BACKSLASH;
			return (append_char(NULL, str[*i], shell));
		}
		else
		{
			return (append_char(NULL, str[*i], shell));
		}
	}
	new_quote_state = handle_quote(str[*i], *quote_char);
	if (new_quote_state != *quote_char)
	{
		*quote_char = new_quote_state;
		return (append_char(NULL, str[*i], shell));
	}
	else if (should_expand_var(str, *i, *quote_char))
		return (expand_var_at_position(str, i, shell));
	else
		return (append_char(NULL, str[*i], shell));
}

char	*expand_variables_with_quotes(const char *str, t_minishell *shell)
{
	char	*result;
	char	*append_str;
	int		i;
	char	quote_char;
	int		old_i;

	if (!str)
        return NULL;
	result = gc_strdup(shell->gc[GC_EXPAND], "");
	i = 0;
	quote_char = 0;
	while (str[i])
	{
		old_i = i;
		append_str = process_character(str, &i, &quote_char, shell);
		result = gc_strjoin(shell->gc[GC_EXPAND], result, append_str);
		if (i == old_i)
			i++;
	}
	return (result);
}
