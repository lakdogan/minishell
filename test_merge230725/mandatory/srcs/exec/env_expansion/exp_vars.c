#include "../../../includes/core/minishell.h"

static int	process_variable(const char *str, int pos, char **result,
		t_minishell *shell)
{
	char	*expanded;
	int		consumed;

	expanded = NULL;
	consumed = expand_single_var(&str[pos], &expanded, shell);
	*result = gc_strjoin(shell->gc[GC_EXPAND], *result, expanded);
	return (consumed);
}

static void	process_regular_char(char c, char **result, t_minishell *shell)
{
	char	tmp[CHAR_BUFFER_SIZE];

	tmp[FIRST_CHAR] = c;
	tmp[NULL_TERMINATOR_INDEX] = NULL_TERMINATOR;
	*result = gc_strjoin(shell->gc[GC_EXPAND], *result, tmp);
}

static bool	is_variable_reference(const char *str, int i)
{
	return (str[i] == DOLLAR_SIGN && str[i + NEXT_CHAR_INDEX]
		&& (is_var_char(str[i + NEXT_CHAR_INDEX])
			|| str[i + NEXT_CHAR_INDEX] == QUESTION_MARK
			|| str[i + NEXT_CHAR_INDEX] == DOLLAR_SIGN));
}

char	*expand_variables(const char *str, t_minishell *shell)
{
	char	*result;
	int		i;
	int		consumed;

	if (!str || (uintptr_t)str < 0x1000)
        return NULL;
	result = gc_strdup(shell->gc[GC_EXPAND], EMPTY_STRING);
	i = 0;
	while (str[i])
	{
		if (is_variable_reference(str, i))
		{
			consumed = process_variable(str, i, &result, shell);
			i += consumed;
		}
		else
		{
			process_regular_char(str[i], &result, shell);
			i++;
		}
	}
	return (result);
}
