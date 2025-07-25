
#include "../../../includes/core/minishell.h"

int check_for_paren(const char *cmd, int *i, int len)
{
	if (cmd[(*i) + len] == '(' || cmd[(*i) + len] == ')')
		return (1);
	else
		return (0);
}

int	check_for_qoutes(const char *cmd, int *i, int *len)
{
	char	quote;

	quote = cmd[(*i) + (*len)];
	(*len)++;
	while (cmd[(*i) + (*len)] && cmd[(*i) + (*len)] != quote)
		(*len)++;
	if (cmd[(*i) + (*len)] == quote)
	{
		(*len)++;
		return (1);
	}
	return (0);
}

// int	tok_len(const char *cmd, int *i)
// {
// 	int		len;

// 	len = 0;

// 	if (cmd[*i] == '>' || cmd[*i] == '<')
// 	{
// 		if (cmd[*i] == cmd[*i + 1])
// 			return (2);
// 		else
// 			return (1);
// 	}
// 	if (cmd[*i] == '|' || cmd[*i] == '&' || cmd[*i] == '(' || cmd[*i] == ')')
// 		return (1);
// 	while (cmd[(*i) + len])
// 	{
// 		if (cmd[(*i) + len] == '"' || cmd[(*i) + len] == '\'')
//         {
//             if (!check_for_qoutes(cmd, i, &len))
//                 break;
//             // Continue processing after the quote - important to not break on special chars inside quotes
//             continue;
//         }
// 		if (cmd[(*i) + len] == '|' || cmd[(*i) + len] == '<' || 
//             cmd[(*i) + len] == '>' || cmd[(*i) + len] == '&' ||
//             cmd[(*i) + len] == '(' || cmd[(*i) + len] == ')')
//             break;
// 		else if (ft_isspace(cmd[*i] + len))
// 			break;
// 		if (cmd[(*i) + len] == '"' || cmd[(*i) + len] == '\'')
// 		{
// 			if (!check_for_qoutes(cmd, i, &len))
// 				break ;
// 		}
// 		else if (ft_isspace(cmd[(*i) + len]))
// 			break ;
// 		else if (check_for_paren(cmd, i, len) && len == 0)
// 		{
// 			len++;
// 			break ;
// 		}
// 		else if (check_for_paren(cmd, i, len) && len > 0)
// 			break ;
// 		else
// 			len++;
// 	}
// 	return (len);
// }

int tok_len(const char *cmd, int *i)
{
    int len;

    len = 0;

    // First handle special operators
    if (cmd[*i] == '>' || cmd[*i] == '<')
    {
        if (cmd[*i] == cmd[*i + 1])
            return (2);
        else
            return (1);
    }
    if (cmd[*i] == '|' || cmd[*i] == '&' || cmd[*i] == '(' || cmd[*i] == ')')
        return (1);
    
    // Process normal word tokens
    while (cmd[(*i) + len])
    {
        // Handle quoted content
        if (cmd[(*i) + len] == '"' || cmd[(*i) + len] == '\'')
        {
            if (!check_for_qoutes(cmd, i, &len))
                break;
            continue;
        }
        
        // Break on special characters (crucial for adjacent tokens)
        if (cmd[(*i) + len] == '|' || cmd[(*i) + len] == '<' || 
            cmd[(*i) + len] == '>' || cmd[(*i) + len] == '&' ||
            cmd[(*i) + len] == '(' || cmd[(*i) + len] == ')')
            break;
            
        // Break on whitespace
        else if (ft_isspace(cmd[(*i) + len]))
            break;
            
        // Otherwise keep building the token
        else
            len++;
    }
    
    // Ensure we return at least one character for non-empty tokens
    return (len > 0 ? len : 0);
}