/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 06:46:52 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/06 01:39:30 by almatsch         ###   ########.fr       */
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

static int	lexer_loop(t_token **tokens, const char *cmd, int *cap, t_minishell *shell)
{
	int		i;
	int		t_count;

	i = 0;
	t_count = 0;
	while (cmd[i])
	{
		while (ft_isspace(cmd[i]))
			i++;
		if (!cmd[i])
			break ;
		if (t_count >= (*cap))
		{
			(*tokens) = resize_array(*tokens, cap, shell->gc[GC_COMMAND]);
			if (!(*tokens))
				return (-1);
		}
		(*tokens)[t_count] = init_token(cmd, &i, t_count, shell);
		if (!(*tokens)[t_count].value)
			return (-1);
		t_count++;
	}
	return (t_count);
}

t_tokens	*lexer(const char *cmd, t_minishell *shell)
{
	t_token		*tokens;
	t_tokens	*array;
	int			count;
	int			cap;

	cap = 10;
	tokens = gc_malloc(shell->gc[GC_COMMAND], (cap + 1) * sizeof(t_token));
	if (!tokens)
		return (NULL);
	array = gc_malloc(shell->gc[GC_COMMAND], sizeof(t_tokens));
	if (!array)
		return (NULL);
	count = lexer_loop(&tokens, cmd, &cap, shell);
	if (count <= 0)
	{
		shell->exit_code = 2;
		return (NULL);
	}
	array->arr = tokens;
	array->count = count;
	return (array);
}
