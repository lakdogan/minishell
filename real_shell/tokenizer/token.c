/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 06:46:52 by almatsch          #+#    #+#             */
/*   Updated: 2025/09/07 22:58:42 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

t_token	*resize_array(t_token *tokens, int *cap, t_gc *gc)
{
	t_token	*new;

	new = gc_realloc(gc, tokens, ((*cap) + 10) * sizeof(t_token));
	if (!new)
		return (NULL);
	(*cap) += 10;
	return (new);
}

static t_token	*allocate_token_array(t_minishell *shell, int cap)
{
	t_token	*tokens;

	tokens = gc_malloc(shell->gc[GC_COMMAND], (cap + 1) * sizeof(t_token));
	return (tokens);
}

static t_tokens	*create_token_array(t_token *tokens, int count,
		t_minishell *shell)
{
	t_tokens	*array;

	array = gc_malloc(shell->gc[GC_COMMAND], sizeof(t_tokens));
	if (!array)
		return (NULL);
	array->arr = tokens;
	array->count = count;
	return (array);
}

static bool	handle_lexer_error(int count, t_minishell *shell)
{
	if (count <= 0)
	{
		shell->exit_code = 2;
		return (true);
	}
	return (false);
}

t_tokens	*lexer(const char *cmd, t_minishell *shell)
{
	t_token		*tokens;
	t_tokens	*array;
	int			count;
	int			cap;

	cap = 10;
	tokens = allocate_token_array(shell, cap);
	if (!tokens)
		return (NULL);
	count = lexer_loop(&tokens, cmd, &cap, shell);
	if (handle_lexer_error(count, shell))
		return (NULL);
	array = create_token_array(tokens, count, shell);
	return (array);
}
