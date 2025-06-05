#include "../../../includes/core/minishell.h"

static  bool is_var_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

static int get_var_name_len(const char *str)
{
    int i;

    i = 0;
    while (str[i] && is_var_char(str[i]))
        i++;
    return (i);
}

static char *extract_var_name(const char *str, t_minishell *shell)
{
    int name_len;

    name_len = get_var_name_len(str);
    return gc_substr(shell->gc[GC_TEMP], str, 0, name_len);
}

static char *handle_special_var(const char *var_name, t_minishell *shell)
{
    if (ft_strcmp(var_name, "?") == 0)
        return gc_itoa(shell->gc[GC_TEMP], shell->exit_code);
    return NULL;
}

static int expand_single_var(const char *start, char **expanded, t_minishell *shell)
{
    int consumed;
    char *var_name;
    char *var_value;

    var_value = NULL;
    if (start[1] == '$')
    {
        *expanded = gc_itoa(shell->gc[GC_TEMP], getpid());
        return 2;
    }
    if (start[1] == '?')
    {
        *expanded = gc_itoa(shell->gc[GC_TEMP], shell->exit_code);
        return 2;
    }
    var_name = extract_var_name(start + 1, shell);
    consumed += ft_strlen(var_name);
    if (get_env_value(var_name, shell, &var_value) == 0 && var_value)
        *expanded = gc_strdup(shell->gc[GC_TEMP], var_value);
    else
        *expanded = gc_strdup(shell->gc[GC_TEMP], "");
    return (consumed);
}

char    *expand_variables(const char *str, t_minishell *shell)
{
    char *result;
    char *expanded;
    int consumed;
    char tmp[2];
    int i;
    
    result = gc_strdup(shell->gc[GC_TEMP], "");
    i = 0;
    while(str[i])
    {
        if (str[i] == '$' && str[i+1] && (is_var_char(str[i+1]) || str[i+1] == '?' || str[i+1] == '$'))
        {
            expanded = NULL;
            consumed = expand_single_var(&str[i], &expanded, shell);
            result = gc_strjoin(shell->gc[GC_TEMP], result, expanded);
            i += consumed;
        }
        else
            tmp[0] = str[i];
            tmp[1] = '\0';
            result = gc_strjoin(shell->gc[GC_TEMP], result, tmp);
            i++;
    }
    return (result);
}

char    *expand_variables_with_quotes(const char *str, t_minishell *shell)
{
    
}