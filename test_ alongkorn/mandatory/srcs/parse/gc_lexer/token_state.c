#include "../../include/core/minishell.h"
#include "../../include/core/token.h"
#include "../../include/utility/parser.h"

int	is_closed(char *value, char c)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == c)
			return (1);
		i++;
	}
	return (0);
}

