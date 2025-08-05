/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 07:48:08 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/06 01:24:49 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

t_token_type	get_tok_type(char *value)
{
	if (!value)
		return (WORD);
	if (ft_strncmp(value, ">>", 2) == 0)
		return (APPEND);
	if (ft_strncmp(value, "<<", 2) == 0)
		return (HEREDOC);
	if (ft_strncmp(value, "&&", 2) == 0)
		return (AND_IF);
	if (ft_strncmp(value, "||", 2) == 0)
		return (OR);
	if (ft_strncmp(value, "|", 1) == 0)
		return (PIPE);
	if (ft_strncmp(value, ">", 1) == 0)
		return (OUTFILE);
	if (ft_strncmp(value, "<", 1) == 0)
		return (INFILE);
	if (ft_strncmp(value, "(", 1) == 0)
		return (L_PAREN);
	if (ft_strncmp(value, ")", 1) == 0)
		return (R_PAREN);
	if (ft_strncmp(value, "&", 1) == 0)
		return (AND_IF);
	return (WORD);
}

t_token_state	get_tok_state(char *value, int len)
{
	if (value[0] == '\'' || value[0] == '"')
	{
		if (value[0] == value[len - 1])
		{
			if (value[0] == '"')
				return (IN_DQUOTES);
			else
				return (IN_SQUOTES);
		}
		else
			return (UNCLOSED_QUOTES);
	}
	return (GENERAL);
}

t_token	init_token_default(void)
{
	t_token	new_token;

	new_token.value = NULL;
	new_token.type = END;
	new_token.pos = -1;
	new_token.state = GENERAL;
	return (new_token);
}

// t_token	get_token(const char *cmd, int i, int *len, t_gc *gc)
// {
// 	t_token	new_token;

// 	new_token = init_token_default();
// 	(*len) = tok_len(cmd, &i);
// 	if ((*len) == -1)
// 		return (new_token);
// 	new_token.value = gc_substr(gc, cmd, i, (*len));
// 	if (!new_token.value)
// 		return (new_token);
// 	new_token.type = get_tok_type(new_token.value);
// 	new_token.state = get_tok_state(new_token.value, (*len));
// 	new_token.no_expand = (new_token.state == IN_SQUOTES);
// 	return (new_token);
// }

// t_token	init_token(const char *cmd, int *i, int t_count, t_minishell *shell)
// {
// 	t_token	new_token;
// 	char	*o_value;
// 	int		len;

// 	len = 0;
// 	new_token = get_token(cmd, *i, &len, shell->gc[GC_COMMAND]);
// 	if (!new_token.value)
// 		return (new_token);
// 	o_value = new_token.value;
// 	if (new_token.type == WORD)
// 		new_token.value = remove_quotes(o_value, shell->gc[GC_COMMAND]);
// 	else if (new_token.type != L_PAREN && new_token.type != R_PAREN)
// 	{
// 		if (!is_token_valid(o_value, new_token.state))
// 			return (init_token_default());
// 	}
// 	new_token.pos = t_count;
// 	(*i) += len;
// 	return (new_token);
// }

void	process_token(t_token *token, t_minishell *shell)
{
	char	*o_value;
	char	*tmp;
	char	quote;

	o_value = token->value;
	quote = 0;
	if (o_value[0] == '\'' || o_value[0] == '"')
		quote = o_value[0];
	token->value = remove_quotes(o_value, shell->gc[GC_COMMAND]);
	if (!token->value)
		token->value = gc_strdup(shell->gc[GC_COMMAND], "");
	if (!token->no_expand && ft_strchr(token->value, '$'))
	{
		tmp = expand_variables(token->value, shell);
		if (tmp)
		{
			token->value = tmp;
			if (quote == '\'')
				token->value = reapply_quote(token->value, quote, shell->gc[GC_COMMAND]);
		}
		else
			token->value = gc_strdup(shell->gc[GC_COMMAND], "");
	}
}

t_token	init_token(const char *cmd, int *i, int t_count, t_minishell *shell)
{
	t_token	new_token;
	int		len;

	new_token = init_token_default();
	len = tok_len(cmd, i);
	if (len == -1)
		return (new_token);
	new_token.value = gc_substr(shell->gc[GC_COMMAND], cmd, *i, len);
	if (!new_token.value)
		return (new_token);
	new_token.type = get_tok_type(new_token.value);
	new_token.state = get_tok_state(new_token.value, len);
	new_token.no_expand = (new_token.state == IN_SQUOTES);
	if (new_token.type == WORD)
		process_token(&new_token, shell);
	else
	{
		if (new_token.type != L_PAREN && new_token.type != R_PAREN)
			if (!is_token_valid(new_token.value, new_token.state))
				return (init_token_default());
	}
	new_token.pos = t_count;
	(*i) += len;
	return (new_token);
}
