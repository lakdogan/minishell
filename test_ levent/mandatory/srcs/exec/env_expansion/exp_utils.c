#include "../../../includes/core/minishell.h"

bool	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	get_var_name_len(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && is_var_char(str[i]))
		i++;
	return (i);
}

char	*extract_var_name(const char *str, t_minishell *shell)
{
	int	name_len;

	name_len = get_var_name_len(str);
	return (gc_substr(shell->gc[GC_EXPAND], str, 0, name_len));
}
