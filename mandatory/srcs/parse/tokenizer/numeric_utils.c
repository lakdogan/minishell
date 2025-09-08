/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 23:49:27 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/08 23:38:08 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"

// Counts number of digits in an integer.
int	count_digits(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

// Converts integer to string.
char	*int_to_str(int n, t_minishell *shell)
{
	char	*str;
	int		len;
	int		temp;
	int		i;

	len = count_digits(n);
	str = gc_malloc(shell->gc[GC_COMMAND], sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	i = len - 1;
	if (n == 0)
		str[0] = '0';
	if (n < 0)
		str[0] = '-';
	while (n != 0)
	{
		temp = n % 10;
		if (temp < 0)
			temp = -temp;
		str[i--] = temp + '0';
		n = n / 10;
	}
	return (str);
}
