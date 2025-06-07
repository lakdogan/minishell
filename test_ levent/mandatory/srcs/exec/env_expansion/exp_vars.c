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
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	*result = gc_strjoin(shell->gc[GC_EXPAND], *result, tmp);
}

static bool	is_variable_reference(const char *str, int i)
{
	return (str[i] == '$' && str[i + 1] && (is_var_char(str[i + 1])
			|| str[i + 1] == '?' || str[i + 1] == '$'));
}

char	*expand_variables(const char *str, t_minishell *shell)
{
	char	*result;
	int		i;
	int		consumed;

	result = gc_strdup(shell->gc[GC_EXPAND], "");
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
