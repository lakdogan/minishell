#include "../../../includes/core/minishell.h"

static int	handle_exit_status(char **expanded, t_minishell *shell)
{
	*expanded = gc_itoa(shell->gc[GC_EXPAND], shell->exit_code);
	if (*expanded)
		return (SPECIAL_VAR_LENGTH);
	return (NO_EXPANSION);
}

static int	handle_process_id(char **expanded, t_minishell *shell)
{
	pid_t	pid;

	pid = getpid();
	*expanded = gc_itoa(shell->gc[GC_EXPAND], pid);
	if (*expanded)
		return (SPECIAL_VAR_LENGTH);
	return (NO_EXPANSION);
}

static void	set_expansion_result(char *var_value, char **expanded,
		t_minishell *shell)
{
	if (var_value)
		*expanded = gc_strdup(shell->gc[GC_EXPAND], var_value);
	else
		*expanded = gc_strdup(shell->gc[GC_EXPAND], EMPTY_STRING);
}

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

int	expand_single_var(const char *start, char **expanded, t_minishell *shell)
{
	if (start[NEXT_CHAR_INDEX] == QUESTION_MARK)
		return (handle_exit_status(expanded, shell));
	if (start[NEXT_CHAR_INDEX] == DOLLAR_SIGN)
		return (handle_process_id(expanded, shell));
	return (expand_env_variable(start, expanded, shell));
}
