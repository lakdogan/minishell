/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 07:48:08 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/11 21:03:36 by lakdogan         ###   ########.fr       */
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



// NEW
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
static int	should_expand_token(const char *input)
{
	char	quote;
	int		start;
	int		len;
	int		end;

	if (input[0] == '"' || input[0] == '\'')
	{
		quote = input[0];
		start = 0;
		while (input[start] && input[start] == quote)
			start++;
		len = (int)ft_strlen(input);
		end = 0;
		while (end < len && input[len - 1 - end] == quote)
			end++;
		if (start > 0 && end > 0 && (start % 2 == 0) && (end % 2 == 0))
			return (0); // Do NOT expand variables.
		return (1);     // Allow expansion.
	}
	return (1); // If token doesn't start with a quote, allow expansion.
}

// static char	*restore_escaped_dollars(char *s, t_minishell *shell)
// {
//     size_t	i = 0;
//     char	*res = gc_strdup(shell->gc[GC_COMMAND], "");

//     while (s[i])
//     {
//         if (s[i] == '\1')
//             res = gc_strjoin(shell->gc[GC_COMMAND], res, "$");
//         else
//         {
//             char tmp[2] = { s[i], '\0' };
//             res = gc_strjoin(shell->gc[GC_COMMAND], res, tmp);
//         }
//         i++;
//     }
//     return (res);
// }

/*
 * Segmented process_token_full:
 *  - Single-quoted segments are copied literally.
 *  - Double-quoted segments: if expansion is allowed,
	remove quotes and expand variables;
 *    otherwise, copy literally.
 *  - Unquoted segments are processed similarly.
 */
// static char *process_token_full(const char *input, t_minishell *shell)
// {
//     size_t	i = 0;
//     char	*result = gc_strdup(shell->gc[GC_COMMAND], "");
//     int		do_expand = should_expand_token(input);
//     size_t	start;
//     char	*segment;
//     char	*dsegment;
//     char	*useg;
//     char	buffer[12];
//     size_t	var_start, var_len;
//     char	*var_name;
//     char	*var_value;
//     int		bs_count;

//     while (input[i])
//     {
//         if (input[i] == '\'')
//         {
//             /* Single-quoted: copy literally */
//             i++; // skip opening quote
//             start = i;
//             while (input[i] && input[i] != '\'')
//                 i++;
//             segment = gc_substr(shell->gc[GC_COMMAND], input, start, i - start);
//             result = gc_strjoin(shell->gc[GC_COMMAND], result, segment);
//             if (input[i] == '\'')
//                 i++; // skip closing quote
//         }
//         else if (input[i] == '"')
//         {
//             /* Double-quoted: expand unless prevented */
//             i++; // skip opening "
//             dsegment = gc_strdup(shell->gc[GC_COMMAND], "");
//             while (input[i] && input[i] != '"')
//             {
//                 if (do_expand && input[i] == '\\' && input[i + 1] == '$')
//                 {
//                     /* Count consecutive backslashes */
//                     bs_count = 0;
//                     while (input[i] && input[i] == '\\')
//                     {
//                         bs_count++;
//                         i++;
//                     }
//                     if (input[i] == '$')
//                     {
//                         if (bs_count % 2 == 1)
//                         {
//                             /* Odd: output half the backslashes then insert marker */
//                             for (int j = 0; j < bs_count / 2; j++)
//                                 dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, "\\");
//                             dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, "\1");
//                             i++; // skip '$'
//                         }
//                         else
//                         {
//                             /* Even: output half the backslashes, then expand variable */
//                             for (int j = 0; j < bs_count / 2; j++)
//                                 dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, "\\");
//                             i++; // skip '$'
//                             if (input[i] == '?')
//                             {
//                                 snprintf(buffer, sizeof(buffer), "%d", shell->exit_code);
//                                 dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, buffer);
//                                 i++;
//                             }
//                             else
//                             {
//                                 var_start = i;
//                                 while (input[i] && (isalnum(input[i]) || input[i] == '_'))
//                                     i++;
//                                 var_len = i - var_start;
//                                 var_name = gc_substr(shell->gc[GC_COMMAND], input, var_start, var_len);
//                                 var_value = expand_variable(var_name, shell);
//                                 dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, var_value);
//                             }
//                         }
//                     }
//                 }
//                 else if (do_expand && input[i] == '$')
//                 {
//                     i++; // skip '$'
//                     if (input[i] == '?')
//                     {
//                         snprintf(buffer, sizeof(buffer), "%d", shell->exit_code);
//                         dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, buffer);
//                         i++;
//                     }
//                     else
//                     {
//                         var_start = i;
//                         while (input[i] && (isalnum(input[i]) || input[i] == '_'))
//                             i++;
//                         var_len = i - var_start;
//                         var_name = gc_substr(shell->gc[GC_COMMAND], input, var_start, var_len);
//                         var_value = expand_variable(var_name, shell);
//                         dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, var_value);
//                     }
//                 }
//                 else
//                 {
//                     char tmp[2] = { input[i], '\0' };
//                     dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, tmp);
//                     i++;
//                 }
//             }
//             if (input[i] == '"')
//                 i++; // skip closing "
//             result = gc_strjoin(shell->gc[GC_COMMAND], result, dsegment);
//         }
//         else
//         {
//             /* Unquoted segment */
//             useg = gc_strdup(shell->gc[GC_COMMAND], "");
//             while (input[i] && input[i] != '\'' && input[i] != '"')
//             {
//                 if (do_expand && input[i] == '\\' && input[i + 1] == '$')
//                 {
//                     bs_count = 0;
//                     while (input[i] && input[i] == '\\')
//                     {
//                         bs_count++;
//                         i++;
//                     }
//                     if (input[i] == '$')
//                     {
//                         if (bs_count % 2 == 1)
//                         {
//                             for (int j = 0; j < bs_count / 2; j++)
//                                 useg = gc_strjoin(shell->gc[GC_COMMAND], useg, "\\");
//                             /* Insert marker so that later expansion does not occur */
//                             useg = gc_strjoin(shell->gc[GC_COMMAND], useg, "\1");
//                             i++; // skip '$'
//                         }
//                         else
//                         {
//                             for (int j = 0; j < bs_count / 2; j++)
//                                 useg = gc_strjoin(shell->gc[GC_COMMAND], useg, "\\");
//                             i++; // skip '$'
//                             if (input[i] == '?')
//                             {
//                                 snprintf(buffer, sizeof(buffer), "%d", shell->exit_code);
//                                 useg = gc_strjoin(shell->gc[GC_COMMAND], useg, buffer);
//                                 i++;
//                             }
//                             else
//                             {
//                                 var_start = i;
//                                 while (input[i] && (isalnum(input[i]) || input[i] == '_'))
//                                     i++;
//                                 var_len = i - var_start;
//                                 var_name = gc_substr(shell->gc[GC_COMMAND], input, var_start, var_len);
//                                 var_value = expand_variable(var_name, shell);
//                                 useg = gc_strjoin(shell->gc[GC_COMMAND], useg, var_value);
//                             }
//                         }
//                     }
//                 }
//                 else if (do_expand && input[i] == '$')
//                 {
//                     i++; // skip '$'
//                     if (input[i] == '?')
//                     {
//                         snprintf(buffer, sizeof(buffer), "%d", shell->exit_code);
//                         useg = gc_strjoin(shell->gc[GC_COMMAND], useg, buffer);
//                         i++;
//                     }
//                     else
//                     {
//                         var_start = i;
//                         while (input[i] && (isalnum(input[i]) || input[i] == '_'))
//                             i++;
//                         var_len = i - var_start;
//                         var_name = gc_substr(shell->gc[GC_COMMAND], input, var_start, var_len);
//                         var_value = expand_variable(var_name, shell);
//                         useg = gc_strjoin(shell->gc[GC_COMMAND], useg, var_value);
//                     }
//                 }
//                 else
//                 {
//                     char tmp[2] = { input[i], '\0' };
//                     useg = gc_strjoin(shell->gc[GC_COMMAND], useg, tmp);
//                     i++;
//                 }
//             }
//             result = gc_strjoin(shell->gc[GC_COMMAND], result, useg);
//         }
//     }
//     return (result);
// }

// void process_token(t_token *token, t_minishell *shell)
// {
//     char *new_value;

//     new_value = process_token_full(token->value, shell);
//     /* Restore markers back to literal '$' */
//     new_value = restore_escaped_dollars(new_value, shell);
//     token->value = new_value;
// }

#ifdef DEBUG
#include <stdio.h>
#endif

static char	*restore_escaped_dollars(char *s, t_minishell *shell)
{
    size_t i = 0;
    char *res = gc_strdup(shell->gc[GC_COMMAND], "");

    while (s[i])
    {
#ifdef DEBUG
        fprintf(stderr, "restore: s[%zu]=%c\n", i, s[i]);
#endif
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

static char *process_token_full(const char *input, t_minishell *shell)
{
    size_t	i = 0;
    char	*result = gc_strdup(shell->gc[GC_COMMAND], "");
    int		do_expand = should_expand_token(input);
    size_t	start;
    char	*segment;
    char	*dsegment;
    char	*useg;
    char	buffer[12];
    size_t	var_start, var_len;
    char	*var_name;
    char	*var_value;
    int		bs_count;

#ifdef DEBUG
    fprintf(stderr, "process_token_full: input = \"%s\"\n", input);
    fprintf(stderr, "do_expand = %d\n", do_expand);
#endif

    while (input[i])
    {
        if (input[i] == '\'')
        {
#ifdef DEBUG
            fprintf(stderr, "Found single quote at index %zu\n", i);
#endif
            /* Single-quoted: copy literally */
            i++; // skip opening quote
            start = i;
            while (input[i] && input[i] != '\'')
                i++;
            segment = gc_substr(shell->gc[GC_COMMAND], input, start, i - start);
#ifdef DEBUG
            fprintf(stderr, "Single-quoted segment: \"%s\"\n", segment);
#endif
            result = gc_strjoin(shell->gc[GC_COMMAND], result, segment);
            if (input[i] == '\'')
                i++; // skip closing quote
        }
        else if (input[i] == '"')
        {
#ifdef DEBUG
            fprintf(stderr, "Found double quote at index %zu\n", i);
#endif
            /* Double-quoted: expand unless prevented */
            i++; // skip opening "
            dsegment = gc_strdup(shell->gc[GC_COMMAND], "");
            while (input[i] && input[i] != '"')
            {
                if (do_expand && input[i] == '\\' && input[i + 1] == '$')
                {
#ifdef DEBUG
                    fprintf(stderr, "Double-quoted: Found backslash before $ at index %zu\n", i);
#endif
                    /* Count consecutive backslashes */
                    bs_count = 0;
                    while (input[i] && input[i] == '\\')
                    {
                        bs_count++;
#ifdef DEBUG
                        fprintf(stderr, "Double-quoted: bs_count now %d at index %zu\n", bs_count, i);
#endif
                        i++;
                    }
                    if (input[i] == '$')
                    {
#ifdef DEBUG
                        fprintf(stderr, "Double-quoted: After bs, found $ at index %zu, bs_count = %d\n", i, bs_count);
#endif
                        if (bs_count % 2 == 1)
                        {
                            for (int j = 0; j < bs_count / 2; j++)
                                dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, "\\");
                            dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, "\1");
#ifdef DEBUG
                            fprintf(stderr, "Double-quoted: Insert marker for escaped $\n");
#endif
                            i++; // skip '$'
                        }
                        else
                        {
                            for (int j = 0; j < bs_count / 2; j++)
                                dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, "\\");
                            i++; // skip '$'
#ifdef DEBUG
                            fprintf(stderr, "Double-quoted: Even bs, processing variable at index %zu\n", i);
#endif
                            if (input[i] == '?')
                            {
                                snprintf(buffer, sizeof(buffer), "%d", shell->exit_code);
                                dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, buffer);
                                i++;
                            }
                            else
                            {
                                var_start = i;
                                while (input[i] && (isalnum(input[i]) || input[i] == '_'))
                                    i++;
                                var_len = i - var_start;
                                var_name = gc_substr(shell->gc[GC_COMMAND], input, var_start, var_len);
                                var_value = expand_variable(var_name, shell);
                                dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, var_value);
                            }
                        }
                    }
                }
                else if (do_expand && input[i] == '$')
                {
#ifdef DEBUG
                    fprintf(stderr, "Double-quoted: Found $ at index %zu\n", i);
#endif
                    i++; // skip '$'
                    if (input[i] == '?')
                    {
                        snprintf(buffer, sizeof(buffer), "%d", shell->exit_code);
                        dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, buffer);
                        i++;
                    }
                    else
                    {
                        var_start = i;
                        while (input[i] && (isalnum(input[i]) || input[i] == '_'))
                            i++;
                        var_len = i - var_start;
                        var_name = gc_substr(shell->gc[GC_COMMAND], input, var_start, var_len);
                        var_value = expand_variable(var_name, shell);
                        dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, var_value);
                    }
                }
                else
                {
#ifdef DEBUG
                    fprintf(stderr, "Double-quoted: Copying literal char '%c' at index %zu\n", input[i], i);
#endif
                    char tmp[2] = { input[i], '\0' };
                    dsegment = gc_strjoin(shell->gc[GC_COMMAND], dsegment, tmp);
                    i++;
                }
            }
            if (input[i] == '"')
                i++; // skip closing "
            result = gc_strjoin(shell->gc[GC_COMMAND], result, dsegment);
        }
        else
        {
#ifdef DEBUG
            fprintf(stderr, "Unquoted segment starting at index %zu\n", i);
#endif
            /* Unquoted segment */
            useg = gc_strdup(shell->gc[GC_COMMAND], "");
            while (input[i] && input[i] != '\'' && input[i] != '"')
            {
                if (do_expand && input[i] == '\\' && input[i + 1] == '$')
                {
#ifdef DEBUG
                    fprintf(stderr, "Unquoted: Found backslash before $ at index %zu\n", i);
#endif
                    bs_count = 0;
                    while (input[i] && input[i] == '\\')
                    {
                        bs_count++;
#ifdef DEBUG
                        fprintf(stderr, "Unquoted: bs_count now %d at index %zu\n", bs_count, i);
#endif
                        i++;
                    }
                    if (input[i] == '$')
                    {
#ifdef DEBUG
                        fprintf(stderr, "Unquoted: After bs, found $ at index %zu, bs_count = %d\n", i, bs_count);
#endif
                        if (bs_count % 2 == 1)
                        {
                            for (int j = 0; j < bs_count / 2; j++)
                                useg = gc_strjoin(shell->gc[GC_COMMAND], useg, "\\");
                            /* Insert marker so that later expansion does not occur */
                            useg = gc_strjoin(shell->gc[GC_COMMAND], useg, "\1");
#ifdef DEBUG
                            fprintf(stderr, "Unquoted: Insert marker for escaped $\n");
#endif
                            i++; // skip '$'
                        }
                        else
                        {
                            for (int j = 0; j < bs_count / 2; j++)
                                useg = gc_strjoin(shell->gc[GC_COMMAND], useg, "\\");
                            i++; // skip '$'
#ifdef DEBUG
                            fprintf(stderr, "Unquoted: Even bs, processing variable at index %zu\n", i);
#endif
                            if (input[i] == '?')
                            {
                                snprintf(buffer, sizeof(buffer), "%d", shell->exit_code);
                                useg = gc_strjoin(shell->gc[GC_COMMAND], useg, buffer);
                                i++;
                            }
                            else
                            {
                                var_start = i;
                                while (input[i] && (isalnum(input[i]) || input[i] == '_'))
                                    i++;
                                var_len = i - var_start;
                                var_name = gc_substr(shell->gc[GC_COMMAND], input, var_start, var_len);
                                var_value = expand_variable(var_name, shell);
                                useg = gc_strjoin(shell->gc[GC_COMMAND], useg, var_value);
                            }
                        }
                    }
                }
                else if (do_expand && input[i] == '$')
                {
#ifdef DEBUG
                    fprintf(stderr, "Unquoted: Found $ at index %zu\n", i);
#endif
                    i++; // skip '$'
                    if (input[i] == '?')
                    {
                        snprintf(buffer, sizeof(buffer), "%d", shell->exit_code);
                        useg = gc_strjoin(shell->gc[GC_COMMAND], useg, buffer);
                        i++;
                    }
                    else
                    {
                        var_start = i;
                        while (input[i] && (isalnum(input[i]) || input[i] == '_'))
                            i++;
                        var_len = i - var_start;
                        var_name = gc_substr(shell->gc[GC_COMMAND], input, var_start, var_len);
                        var_value = expand_variable(var_name, shell);
                        useg = gc_strjoin(shell->gc[GC_COMMAND], useg, var_value);
                    }
                }
                else
                {
#ifdef DEBUG
                    fprintf(stderr, "Unquoted: Copying literal char '%c' at index %zu\n", input[i], i);
#endif
                    char tmp[2] = { input[i], '\0' };
                    useg = gc_strjoin(shell->gc[GC_COMMAND], useg, tmp);
                    i++;
                }
            }
            result = gc_strjoin(shell->gc[GC_COMMAND], result, useg);
        }
    }
#ifdef DEBUG
    fprintf(stderr, "process_token_full: result = \"%s\"\n", result);
#endif
    return (result);
}

void process_token(t_token *token, t_minishell *shell)
{
    char *new_value;

    new_value = process_token_full(token->value, shell);
    /* Restore markers back to literal '$' */
    new_value = restore_escaped_dollars(new_value, shell);
#ifdef DEBUG
    fprintf(stderr, "process_token: new token value = \"%s\"\n", new_value);
#endif
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