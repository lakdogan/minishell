/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 07:48:08 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/23 15:00:47 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// New helper: Check if a token is a valid assignment (for example "NAME=VALUE")
static int	is_valid_assignment(const char *s)
{
	int	i;

	if (!s || (!(ft_isalpha(s[0]) || s[0] == '_')))
		return (0);
	i = 0;
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	if (s[i] != '=')
		return (0);
	return (1);
}

// Modified get_tok_type to use is_valid_assignment.
t_token_type	get_tok_type(char *value)
{
	// If token is a valid assignment, return ASSIGNMENT.
	if (value && is_valid_assignment(value))
		return (ASSIGNMENT);
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

#include "../../../includes/core/minishell.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/*
 * Helper: get_env_var
 * Returns the environment variable’s value (the part after '=')
 * for the given variable name.
 */
static char	*get_env_var(t_minishell *shell, const char *var)
{
	t_env	*env;
	char	*eq;

	env = find_env_var(var, shell);
	if (env && env->content)
	{
		eq = strchr(env->content, '=');
		if (eq)
			return (eq + 1);
		return (env->content);
	}
	return ("");
}

/*
 * Helper: expand a variable.
 */
static char	*expand_variable(const char *var, t_minishell *shell)
{
	char	*value;

	value = get_env_var(shell, var);
	if (!value)
		return (gc_strdup(shell->gc[GC_COMMAND], ""));
	return (gc_strdup(shell->gc[GC_COMMAND], value));
}

/*
 * Extra helper: should_expand_token
 * If the token starts with double quotes,
	count the consecutive double quotes at the beginning and end.
 * If both counts are nonzero and even then return 0 (do not expand variables),
 * otherwise return 1 (allow expansion).
 */
// static int	should_expand_token(const char *input)
// {
// 	char	quote;
// 	int		start;
// 	int		len;
// 	int		end;

// 	if (input[0] == '"' || input[0] == '\'')
// 	{
// 		quote = input[0];
// 		start = 0;
// 		while (input[start] && input[start] == quote)
// 			start++;
// 		len = (int)ft_strlen(input);
// 		end = 0;
// 		while (end < len && input[len - 1 - end] == quote)
// 			end++;
// 		if (start > 0 && end > 0 && (start % 2 == 0) && (end % 2 == 0))
// 			return (0); // Do NOT expand variables.
// 		return (1);     // Allow expansion.
// 	}
// 	return (1); // If token doesn't start with a quote, allow expansion.
// }

static char	*restore_escaped_dollars(char *s, t_minishell *shell)
{
    size_t i = 0;
    char *res = gc_strdup(shell->gc[GC_COMMAND], "");

    while (s[i])
    {
        if (s[i] == '\1')
            res = gc_strjoin(shell->gc[GC_COMMAND], res, "$");
        else
        {
            char tmp[2] = { s[i], '\0' };
            res = gc_strjoin(shell->gc[GC_COMMAND], res, tmp);
        }
        i++;
    }
    return (res);
}

// New helper: Process variable expansion after '$'
static void process_dollar(const char *input, size_t *i, t_minishell *shell, char **buf)
{
    char buffer[12];
    size_t var_start, var_len;
    char *var_name;
    char *var_value;

    (*i)++; // skip '$'
    if (input[*i] == '?')
    {
        snprintf(buffer, sizeof(buffer), "%d", shell->exit_code);
        *buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, buffer);
        (*i)++;
    }
    else
    {
        var_start = *i;
        while (input[*i] && (isalnum(input[*i]) || input[*i] == '_'))
            (*i)++;
        var_len = *i - var_start;
        var_name = gc_substr(shell->gc[GC_COMMAND], input, var_start, var_len);
        var_value = expand_variable(var_name, shell);
        *buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, var_value);
    }
}

// Existing helper: Process consecutive backslashes and handle '$'
static void process_bslashes(const char *input, size_t *i, t_minishell *shell, char **buf)
{
    int bs_count = 0;
    // Count all consecutive backslashes
    while (input[*i] && input[*i] == '\\')
    {
        bs_count++;
        (*i)++;
    }
    if (input[*i] == '$')
    {
        bs_count--; // Adjust for the '$'
        if (bs_count % 2 == 0)
        {
            for (int j = 0; j < bs_count / 2; j++)
                *buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, "\\");
            // Let caller process the '$'
        }
        else
        {
            for (int j = 0; j < bs_count / 2; j++)
                *buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, "\\");
            *buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, "\1");
            (*i)++; // Skip the '$'
        }
    }
    else
    {
        for (int j = 0; j < bs_count; j++)
            *buf = gc_strjoin(shell->gc[GC_COMMAND], *buf, "\\");
    }
}

// New helper: Process single-quoted segment
static char *proc_single(const char *input, size_t *i, t_minishell *shell)
{
    size_t start;
    char *seg;
    (*i)++; // skip opening '
    start = *i;
    while (input[*i] && input[*i] != '\'')
        (*i)++;
    seg = gc_substr(shell->gc[GC_COMMAND], input, start, *i - start);
    if (input[*i] == '\'')
        (*i)++; // skip closing '
    return seg;
}

// New helper: Process double-quoted segment (expanding \$)
static char *proc_double(const char *input, size_t *i, t_minishell *shell)
{
    char *res = gc_strdup(shell->gc[GC_COMMAND], "");
    (*i)++; // skip opening "
    while (input[*i] && input[*i] != '"')
    {
        if (input[*i] == '\\')
        {
            process_bslashes(input, i, shell, &res);
            continue;
        }
        if (input[*i] == '$')
        {
            process_dollar(input, i, shell, &res);
            continue;
        }
        {
            char tmp[2] = { input[*i], '\0' };
            res = gc_strjoin(shell->gc[GC_COMMAND], res, tmp);
            (*i)++;
        }
    }
    if (input[*i] == '"')
        (*i)++; // skip closing "
    return res;
}

// New helper: Process unquoted segment
static char *proc_unquoted(const char *input, size_t *i, t_minishell *shell)
{
    char *res = gc_strdup(shell->gc[GC_COMMAND], "");
    while (input[*i] && input[*i] != '\'' && input[*i] != '"')
    {
        if (input[*i] == '\\')
        {
            process_bslashes(input, i, shell, &res);
            continue;
        }
        if (input[*i] == '$')
        {
            process_dollar(input, i, shell, &res);
            continue;
        }
        {
            char tmp[2] = { input[*i], '\0' };
            res = gc_strjoin(shell->gc[GC_COMMAND], res, tmp);
            (*i)++;
        }
    }
    return res;
}

// Updated: process_token_full now calls the small helper functions
static char *process_token_full(const char *input, t_minishell *shell)
{
    size_t i = 0;
    char *result = gc_strdup(shell->gc[GC_COMMAND], "");
    while (input[i])
    {
        if (input[i] == '\'')
            result = gc_strjoin(shell->gc[GC_COMMAND], result, proc_single(input, &i, shell));
        else if (input[i] == '"')
            result = gc_strjoin(shell->gc[GC_COMMAND], result, proc_double(input, &i, shell));
        else
            result = gc_strjoin(shell->gc[GC_COMMAND], result, proc_unquoted(input, &i, shell));
    }
    return result;
}

void process_token(t_token *token, t_minishell *shell)
{
    char *new_value;
    new_value = process_token_full(token->value, shell);
    new_value = restore_escaped_dollars(new_value, shell);
    token->value = new_value;
}

t_token	init_token(const char *cmd, int *i, int t_count, t_minishell *shell)
{
	t_token new_token;
	int len;

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
	// Process expansion only for WORD tokens; ASSIGNMENT tokens remain unchanged.
	if (new_token.type == WORD)
		process_token(&new_token, shell);
	// Bypass syntax validation for WORD and ASSIGNMENT tokens.
	if (new_token.type != WORD && new_token.type != ASSIGNMENT &&
	    new_token.type != L_PAREN && new_token.type != R_PAREN)
	{
		if (!is_token_valid(new_token.value, new_token.state))
			return (init_token_default());
	}
	new_token.pos = t_count;
	(*i) += len;
	return (new_token);
}
