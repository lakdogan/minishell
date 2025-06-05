#include "../../../includes/core/minishell.h"

static char handle_quote(char c, char quote_char)
{
    if ((c == '\'' || c == '"') && (quote_char == 0 || quote_char == c))
    {
        if (quote_char == 0)
            return c;
        else
            return (0);
    }
    return (quote_char);
}

static char *append