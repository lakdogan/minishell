/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_properties.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 00:36:44 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/04 00:56:03 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

void	process_token(t_token *token, t_minishell *shell)
{
	char	*new_value;

	new_value = process_token_full(token->value, shell);
	new_value = restore_escaped_dollars(new_value, shell);
	token->value = new_value;
}

char	*extract_token_value(t_extract_params params, t_minishell *shell)
{
	if ((params.state == IN_SQUOTES || params.state == IN_DQUOTES)
		&& params.len >= 2)
		return (gc_substr(shell->gc[GC_COMMAND], params.cmd, params.start + 1,
				params.len - 2));
	return (gc_substr(shell->gc[GC_COMMAND], params.cmd, params.start,
			params.len));
}

void	set_token_properties(t_token *token, t_token_state state)
{
	if (state == IN_SQUOTES || state == IN_DQUOTES)
		token->type = WORD;
	else
		token->type = get_tok_type(token->value);
	token->state = state;
	token->no_expand = (state == IN_SQUOTES);
	token->was_quoted = (state == IN_SQUOTES || state == IN_DQUOTES);
}

void	apply_token_expansion(t_token *token, t_minishell *shell)
{
	if (token->type == WORD && !shell->is_heredoc && token->state != IN_SQUOTES)
		process_token(token, shell);
}
