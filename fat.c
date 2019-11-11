/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/11 15:39:50 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file.h"
#include "magic.h"

#include <mach-o/loader.h>
#include <mach-o/fat.h>

#include "libft/libft.h"

uint32_t	swap_endian_32(uint32_t n)
{
	return (n >> 24) | ((n >> 8) & 0xFF00) | ((n << 8) & 0xFF0000) | (n << 24);
}

static int	trim_fat_big_endian(t_file *file, struct fat_arch *arches,
															uint32_t nfat_arch)
{
	uint32_t i;
	uint32_t offset;
	uint32_t size;
	uint32_t magic;

	i = 0;
	while (i < nfat_arch)
	{
		offset = swap_endian_32(arches[i].offset);
		size = swap_endian_32(arches[i].size);
		if (file->size < offset + size)
		{
			ft_putstr("Unexpected end of file.\n");
			return (0);
		}
		magic = *(uint32_t *)(file->contents + offset);
		if (is_macho(magic) && !is_32_bit(magic))
		{
			ft_memmove(file->contents, file->contents + offset, size);
			file->size = size;
			return (1);
		}
		i++;
	}
	return (ft_error("Fat file didn't contain any useful sections.\n", 0));
}

static int	trim_fat_little_endian(t_file *file, struct fat_arch *arches,
															uint32_t nfat_arch)
{
	uint32_t i;
	uint32_t offset;
	uint32_t size;
	uint32_t magic;

	i = 0;
	while (i < nfat_arch)
	{
		offset = arches[i].offset;
		size = arches[i].size;
		if (file->size < offset + size)
		{
			ft_putstr("Unexpected end of file.\n");
			return (0);
		}
		magic = *(uint32_t *)(file->contents + offset);
		if (is_macho(magic) && !is_32_bit(magic))
		{
			ft_memmove(file->contents, file->contents + offset, size);
			file->size = size;
			return (1);
		}
		i++;
	}
	return (ft_error("Fat file didn't contain any useful sections.\n", 0));
}

int			trim_fat(t_file *file)
{
	struct fat_header	*header;
	uint32_t			nfat_arch;

	if (file->size < sizeof(struct fat_header))
	{
		ft_putstr("Unexpected end of file.\n");
		return (0);
	}
	header = (void *)file->contents;
	if (is_big_endian(header->magic))
		nfat_arch = swap_endian_32(header->nfat_arch);
	else
		nfat_arch = header->nfat_arch;
	if (file->size < sizeof(struct fat_header) +
											nfat_arch * sizeof(struct fat_arch))
	{
		ft_putstr("Unexpected end of file.\n");
		return (0);
	}
	if (is_big_endian(header->magic))
		return (trim_fat_big_endian(file, (void *)file->contents +
										sizeof(struct fat_header), nfat_arch));
	else
		return (trim_fat_little_endian(file, (void *)file->contents +
										sizeof(struct fat_header), nfat_arch));
}
