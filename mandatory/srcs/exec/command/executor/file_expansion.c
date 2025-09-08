/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:50:46 by lakdogan          #+#    #+#             */
/*   Updated: 2025/09/05 19:51:07 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/core/minishell.h"

/* Expand variables in input file names */
void	expand_infile_names(t_minishell *shell, t_exec *exec)
{
	t_infile	*infile;

	infile = exec->infiles;
	while (infile)
	{
		infile->name = expand_variables_with_quotes(infile->name, shell);
		infile = infile->next;
	}
}

/* Expand variables in output file names */
void	expand_outfile_names(t_minishell *shell, t_exec *exec)
{
	t_outfile	*outfile;

	outfile = exec->outfiles;
	while (outfile)
	{
		outfile->name = expand_variables_with_quotes(outfile->name, shell);
		outfile = outfile->next;
	}
}
