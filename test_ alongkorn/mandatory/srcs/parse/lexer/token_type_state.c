#include "../../include/core/minishell.h"
#include "../../include/core/token.h"
#include "../../include/utility/parser.h"

t_token_type	get_token_type(char *value)
{
	if (!value)
		return (WORD);
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
	return (WORD);
}

t_token_state	get_token_state(char *value)
{
	
}
