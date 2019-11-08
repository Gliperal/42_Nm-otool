/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   machfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/08 14:21:24 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "machfile.h"
#include "magic.h"

#include "libft/libft.h"

void		unload_machfile(t_machfile *machfile)
{
	ft_close(machfile->file);
	free(machfile->sects_64);
	free(machfile->symtab_64);
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
	machfile->sects_32 = NULL;
	machfile->sects_64 = NULL;
	machfile->nsects = 0;
	machfile->symtab_32 = NULL;
	machfile->symtab_64 = NULL;
	machfile->strtab = NULL;
	uint32_t magic = *(uint32_t *)file->contents;
	if (!is_macho(magic))
	{
		ft_putstr("Not an object file.\n");
		free(machfile);
		ft_close(file);
		return (NULL);
	}
	machfile->reverse_byte_order = is_big_endian(magic);
	if (machfile->reverse_byte_order)
	{
		ft_putstr("Reverse byte order not currently supported.\n");
		free(machfile);
		ft_close(file);
		return (NULL);
	}
	if (is_32_bit(magic))
		status = do_things_32(machfile);
	else
		status = do_things_64(machfile);
	if (status == -1)
//	if (status == -1 || machfile->sects_64 == NULL || machfile->symtab_64 == NULL)
	{
		unload_machfile(machfile);
		return (NULL);
	}
	return (machfile);
}
