
#include "../../../includes/core/minishell.h"

void	error_msg(t_tokens *tokens, int *pos, t_minishell *shell)
{
	printf("minishell: syntax error near unexpected token '%s'\n", tokens->arr[(*pos) - 1].value);
	shell->exit_code = 2;
}

void	error_msg_paren(void)
{
	printf("minishell: syntax error: unexpected end of file");
}
