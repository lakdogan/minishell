
#include "../../../includes/core/minishell.h"

void	error_msg(t_tokens *tokens, int *pos)
{
	printf("minishell$ syntax error near unexpected token '%s'\n", tokens->arr[(*pos) - 1].value);
}

void	error_msg_paren(void)
{
	printf("minishell$ syntax error: unexpected end of file\n");
}
