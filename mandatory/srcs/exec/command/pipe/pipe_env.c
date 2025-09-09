/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 20:45:42 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/09 20:48:53 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

char	**list_to_envp_array(t_gc *gc, t_list *envp)
{
	int		size;
	char	**arr;
	t_list	*node;
	int		i;

	size = ft_lstsize(envp);
	arr = gc_malloc(gc, sizeof(char *) * (size + 1));
	node = envp;
	i = 0;
	while (node)
	{
		arr[i++] = ((t_env *)node->content)->content;
		node = node->next;
	}
	arr[i] = NULL;
	return (arr);
}
