/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_var_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:20:59 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 06:13:55 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

bool	is_dollar_without_var(const char *str, int i)
{
	return (str[i] == '$' && (!str[i + 1] || !(ft_isalnum(str[i + 1]) || str[i
					+ 1] == '_' || str[i + 1] == '?' || str[i + 1] == '$')));
}

static t_expvar_case	get_expvar_case(const char *str, int i, char quote_char)
{
	if (is_bare_question_mark(str, i, quote_char))
		return (EXPVAR_BARE_QUESTION);
	if (str[i] == '$' && str[i + 1] == '?')
		return (EXPVAR_DOLLAR_QUESTION);
	if (is_dollar_without_var(str, i))
		return (EXPVAR_DOLLAR_NO_VAR);
	return (EXPVAR_NONE);
}

static int	handle_special_cases_expvar(t_expvar_ctx *ctx)
{
	t_expvar_case	case_type;
	char			*exit_str;

	case_type = get_expvar_case(ctx->str, ctx->i, ctx->quote_char);
	if (case_type == EXPVAR_BARE_QUESTION)
	{
		*(ctx->result) = gc_strjoin(ctx->shell->gc[GC_EXPAND], *(ctx->result),
				"1");
		return (1);
	}
	if (case_type == EXPVAR_DOLLAR_QUESTION)
	{
		exit_str = int_to_str(ctx->shell->exit_code, ctx->shell);
		*(ctx->result) = gc_strjoin(ctx->shell->gc[GC_EXPAND], *(ctx->result),
				exit_str);
		return (2);
	}
	if (case_type == EXPVAR_DOLLAR_NO_VAR)
	{
		*(ctx->result) = append_char(*(ctx->result), '$', ctx->shell);
		return (1);
	}
	return (0);
}

void	expand_loop_iteration(t_expvar_ctx *ctx)
{
	int		old_i;
	char	*append_str;
	int		consumed;

	consumed = handle_special_cases_expvar(ctx);
	if (consumed)
	{
		ctx->i += consumed;
		return ;
	}
	old_i = ctx->i;
	append_str = process_character(ctx->str, &ctx->i, &ctx->quote_char,
			ctx->shell);
	*(ctx->result) = gc_strjoin(ctx->shell->gc[GC_EXPAND], *(ctx->result),
			append_str);
	if (ctx->i == old_i)
		ctx->i++;
}

char	*expand_variables_with_quotes(const char *str, t_minishell *shell)
{
	char	*result;

	if (!str)
		return (NULL);
	result = gc_strdup(shell->gc[GC_EXPAND], EMPTY_STRING);
	expand_loop(str, shell, &result);
	return (result);
}
