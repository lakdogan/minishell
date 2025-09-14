/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_single_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:21:17 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/14 05:54:28 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Expands the special variable $? to the last exit status.
static int	expand_exit_status(char **expanded, t_minishell *shell)
{
	*expanded = gc_itoa(shell->gc[GC_EXPAND], shell->exit_code);
	if (*expanded)
		return (SPECIAL_VAR_LENGTH);
	return (NO_EXPANSION);
}

// Expands the special variable $$ to the current process ID.
static int	handle_process_id(char **expanded, t_minishell *shell)
{
	*expanded = gc_itoa(shell->gc[GC_EXPAND], shell->shell_pid);
	if (*expanded)
		return (SPECIAL_VAR_LENGTH);
	return (NO_EXPANSION);
}

// Sets the expansion result for a variable.
static void	set_expansion_result(char *var_value, char **expanded,
		t_minishell *shell)
{
	if (var_value)
		*expanded = gc_strdup(shell->gc[GC_EXPAND], var_value);
	else
		*expanded = gc_strdup(shell->gc[GC_EXPAND], EMPTY_STRING);
}

// Expands an environment variable from the string.
static int	expand_env_variable(const char *start, char **expanded,
		t_minishell *shell)
{
	int		consumed;
	char	*var_name;
	char	*var_value;

	consumed = INITIAL_CONSUMED_COUNT;
	var_name = extract_var_name(start + SKIP_DOLLAR_SIGN, shell);
	consumed = ft_strlen(var_name);
	var_value = find_env_value(shell, var_name);
	set_expansion_result(var_value, expanded, shell);
	return (consumed + SKIP_DOLLAR_SIGN);
}

// Expands a single variable (env, ~, $? or $$).
int	expand_single_var(const char *start, char **expanded, t_minishell *shell)
{
	if (start[0] == '$' && (start[1] == '\0' || !(ft_isalnum(start[1])
				|| start[1] == '_' || start[1] == '?' || start[1] == '$')))
	{
		*expanded = gc_strdup(shell->gc[GC_EXPAND], "");
		return (1);
	}
	if (start[NEXT_CHAR_INDEX] == QUESTION_MARK)
		return (expand_exit_status(expanded, shell));
	if (start[0] == '$' && start[1] == '$' && start[2] == '\0')
		return (handle_process_id(expanded, shell));
	return (expand_env_variable(start, expanded, shell));
}
