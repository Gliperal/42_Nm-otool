/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/08 14:40:28 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO REMOVE
#include <inttypes.h>

#include "machfile.h"
#include "misc.h"

#include "libft/libft.h"

static void	hexdump_32(void *data, uint32_t size, uint32_t addr)
{
	unsigned char	*d;
	uint32_t		i;

	d = data;
	while (1)
	{
		PRINT_HEX_UINT32_T(addr);
		ft_printf("\t");
		i = 0;
		while (i < size && i < 16)
		{
			ft_printf("%.2x ", d[i]);
			i++;
		}
		ft_printf("\n");
		d += 16;
		if (size <= 16)
			break ;
		size -= 16;
		addr += 16;
	}
}

static void	hexdump_64(void *data, uint32_t size, uint64_t addr)
{
	unsigned char	*d;
	uint32_t		i;

	d = data;
	while (1)
	{
		PRINT_HEX_UINT64_T(addr);
		ft_printf("\t");
		i = 0;
		while (i < size && i < 16)
		{
			ft_printf("%.2x ", d[i]);
			i++;
		}
		ft_printf("\n");
		d += 16;
		if (size <= 16)
			break ;
		size -= 16;
		addr += 16;
	}
}

/*
** There's also an align member in the section_64 struct, which does some
** curious things that I'm not sure if we need to handle, or if that's just for
** the creators of the mach-o file to deal with.
*/

static void	display_32(t_machfile *machfile)
{
	struct section	*s;
	uint32_t		i;

	i = 0;
	while (i < machfile->nsects)
	{
		s = machfile->sects_32[i];
		if ((ft_strcmp(s->sectname, "__text") == 0) &&
				(ft_strcmp(s->segname, "__TEXT") == 0))
		{
			ft_printf("Contents of (__TEXT,__text) section\n");
			hexdump_32(machfile->file->contents + s->offset, s->size, s->addr);
		}
		i++;
	}
}

static void	display_64(t_machfile *machfile)
{
	struct section_64	*s;
	uint32_t			i;

	i = 0;
	while (i < machfile->nsects)
	{
		s = machfile->sects_64[i];
		if ((ft_strcmp(s->sectname, "__text") == 0) &&
				(ft_strcmp(s->segname, "__TEXT") == 0))
		{
			ft_printf("Contents of (__TEXT,__text) section\n");
			hexdump_64(machfile->file->contents + s->offset, s->size, s->addr);
		}
		i++;
	}
}

void		display(t_machfile *machfile, int multiple_files)
{
	UNUSED(multiple_files);
	ft_printf("%s:\n", machfile->file->filename);
	// TODO add 32/64 bit flag in machfile struct
	if (machfile->sects_32)
		display_32(machfile);
	if (machfile->sects_64)
		display_64(machfile);
}
