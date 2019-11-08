/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/07 17:49:54 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "machfile.h"
#include "misc.h"

#include "libft/libft.h"

static void	hexdump(void *data, uint32_t size, uint64_t addr)
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

/*
** There's also an align member in the section_64 struct, which does some
** curious things that I'm not sure if we need to handle, or if that's just for
** the creators of the mach-o file to deal with.
*/

void		display(t_machfile *machfile, int multiple_files)
{
	uint32_t			i;
	struct section_64	*s;

	UNUSED(multiple_files);
	ft_printf("%s:\n", machfile->file->filename);
	i = 0;
	while (i < machfile->nsects)
	{
		s = machfile->sects[i];
		if ((ft_strcmp(s->sectname, "__text") == 0) &&
				(ft_strcmp(s->segname, "__TEXT") == 0))
		{
			ft_printf("Contents of (__TEXT,__text) section\n");
			hexdump(machfile->file->contents + s->offset, s->size, s->addr);
		}
		i++;
	}
}
