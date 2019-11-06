/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/06 12:41:34 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO REMOVE
#include <inttypes.h>

#include "machfile.h"

#include "libft/libft.h"

// OTOOL -T

void	print_hex_uint32_t(uint64_t n);

static void	hexdump(void *data, uint32_t size, uint64_t addr)
{
	unsigned char *d;
	int i;

	d = data;
	while (1)
	{
		print_hex_uint32_t(addr);
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

void	display(t_machfile *machfile)
{
	ft_printf("%s:\n", machfile->file->filename);
	for (uint32_t i = 0; i < machfile->nsects; i++)
	{
		struct section_64 *s = machfile->sects[i];
//		ft_printf("align = %" PRId32 "\n", s->align);
		if ((ft_strcmp(s->sectname, "__text") == 0) && (ft_strcmp(s->segname, "__TEXT") == 0))
		{
			ft_printf("Contents of (__TEXT,__text) section\n");
			hexdump(machfile->file->contents + s->offset, s->size, s->addr);
		}
	}
}
