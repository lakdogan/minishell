/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almatsch <almatsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:59:03 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/06 01:39:00 by almatsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

char	*reapply_quote(const char *str, char quote, t_gc *gc)
{
	char	*new_str;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	new_str = gc_malloc(gc, len + 3);
	if (!new_str)
		return ((char *)str);
	new_str[0] = quote;
	memcpy(new_str + 1, str, len);
	new_str[len + 1] = quote;
	new_str[len + 2] = '\0';
	return (new_str);
}

// char	*remove_quotes(char *value, t_gc *gc)
// {
// 	char	*cleaned;
// 	int		i;
// 	int		j;

// 	if (!value)
// 		return (NULL);
// 	cleaned = gc_malloc(gc, ft_strlen(value) + 1);
// 	if (!cleaned)
// 		return (value);
// 	i = 0;
// 	j = 0;
// 	while (value[i])
// 	{
// 		if (value[i] == '\'' || value[i] == '"')
// 			val_cpy(cleaned, value, &i, &j);
// 		else
// 		{
// 			cleaned[j] = value[i];
// 			j++;
// 			i++;
// 		}
// 	}
// 	cleaned[j] = '\0';
// 	return (cleaned);
// }

char	*remove_quotes (char *value, t_gc *gc)
{
	int		len;
	char	*clean;

	len = ft_strlen(value);
	if (len >= 2 && value[0] == '"' && value[len - 1] == '"')
	{
		clean = gc_malloc(gc, len - 1);
		if (!clean)
			return (value);
		memcpy(clean, value + 1, len - 2);
		clean[len - 2] = '\0';
		return (clean);
	}
	return (gc_strdup(gc, value));
}
