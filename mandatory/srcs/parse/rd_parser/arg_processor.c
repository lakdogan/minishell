/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_processor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 23:30:13 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 20:02:49 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Processes redirection token.
static int	process_redirection(t_exec *exec, t_tokens *tokens, int *pos,
		t_minishell *shell)
{
	t_redir_ctx	redir_ctx;

	redir_ctx.exec = exec;
	redir_ctx.tokens = tokens;
	redir_ctx.pos = pos;
	redir_ctx.gc = shell->gc[GC_COMMAND];
	if (!handle_redir(redir_ctx))
	{
		(*pos) = -1;
		return (0);
	}
	return (1);
}

// Allocates and copies a word
char	*word_dup(const char *start, int len)
{
	char	*word;

	word = malloc(len + 1);
	if (!word)
		return (NULL);
	ft_strncpy(word, start, len);
	word[len] = '\0';
	return (word);
}

static void	process_arg_token(t_arg_lst **args, t_token token,
		t_minishell *shell, int *pos)
{
	char		*expanded;
	t_arg_flags	flags;
	char		**words;
	int			i;

	expanded = expand_token_value(token, shell);
	flags.no_expand = token.no_expand;
	flags.was_quoted = token.was_quoted;
	if (!token.was_quoted)
	{
		words = ft_split_charset(expanded, " \t\n");
		i = 0;
		while (words && words[i])
		{
			arg_to_list(args, words[i], flags, shell);
			i++;
		}
		if (words)
			ft_free_split(words);
	}
	else
	{
		arg_to_list(args, expanded, flags, shell);
	}
	(*pos)++;
}

static int	is_arg_token(t_token_type type)
{
	return (type == WORD || type == ASSIGNMENT);
}

// Collects arguments and handles redirections.
t_arg_lst	*get_args(t_exec *exec, t_tokens *tokens, int *pos,
		t_minishell *shell)
{
	t_arg_lst	*args;

	args = NULL;
	while ((*pos) < tokens->count && (is_arg_token(tokens->arr[(*pos)].type)
			|| is_redir(tokens->arr[(*pos)].type)))
	{
		if (is_redir(tokens->arr[(*pos)].type))
		{
			if (!process_redirection(exec, tokens, pos, shell))
				return (NULL);
		}
		else
			process_arg_token(&args, tokens->arr[(*pos)], shell, pos);
	}
	return (args);
}
