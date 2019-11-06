/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   machfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/06 15:45:07 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "machfile.h"

void		unload_machfile(t_machfile *machfile)
{
	ft_close(machfile->file);
	free(machfile->sects);
	free(machfile->symtab);
	free(machfile);
}

t_machfile	*load_machfile(t_file *file)
{
	t_machfile	*machfile;
	int			status;

	machfile = malloc(sizeof(t_machfile));
	if (!machfile)
	{
		ft_close(file);
		return (NULL);
	}
	machfile->file = file;
	machfile->sects = NULL;
	machfile->nsects = 0;
	machfile->symtab = NULL;
	machfile->strtab = NULL;
	status = do_things(machfile);
	if (status == -1 || machfile->sects == NULL || machfile->symtab == NULL)
	{
		unload_machfile(machfile);
		return (NULL);
	}
	return (machfile);
}
