/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/08 14:36:05 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO add -WWW to makefile

#include "machfile.h"
#include "misc.h"
#include "magic.h"

#include <mach-o/loader.h>
#include <mach-o/fat.h>

#include "libft/libft.h"

uint32_t	swap_endian_32(uint32_t n)
{
	return (n >> 24) | ((n >> 8) & 0xFF00) | ((n << 8) & 0xFF0000) | (n << 24);
}

static int	trim_fat_helper(t_file *file, struct fat_arch *arches,
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
	ft_putstr("Fat file didn't contain any useful sections.\n");
	return (0);
}

static int	trim_fat(t_file *file)
{
	struct fat_header	*header;
	uint32_t			nfat_arch;
	int					status;

	if (file->size < sizeof(struct fat_header))
	{
		ft_putstr("Unexpected end of file.\n");
		return (0);
	}
	header = (void *)file->contents;
	// TODO fat files won't always be reverse
	nfat_arch = swap_endian_32(header->nfat_arch);
	if (file->size < sizeof(struct fat_header) +
											nfat_arch * sizeof(struct fat_arch))
	{
		ft_putstr("Unexpected end of file.\n");
		return (0);
	}
	return (trim_fat_helper(file, (void *)file->contents +
										sizeof(struct fat_header), nfat_arch));
}

static int	per_file(const char *filename, int multiple_files)
{
	t_file		*file;
	t_machfile	*machfile;

	file = ft_open(filename);
	if (!file)
		return (1);
	if (file->size >= 8 && ft_strncmp(file->contents, "!<arch>\n", 8) == 0)
	{
		ft_putstr("Archive files not currently supported.\n");
		ft_close(file);
		return (1);
	}
	if (file->size < 4)
	{
		ft_putstr("Unexpected end of file.\n");
		ft_close(file);
		return (1);
	}
	if (is_fat(*(uint32_t *)file->contents))
		if (!trim_fat(file))
			return (1);
	machfile = load_machfile(file);
	if (!machfile)
		return (1);
	display(machfile, multiple_files);
	unload_machfile(machfile);
	return (0);
}

int	main(int argc, const char **argv)
{
	int status;
	int i;

	status = 0;
	if (argc < 2)
		status |= per_file("a.out", 0);
	else
	{
		i = 1;
		while (i < argc)
		{
			status |= per_file(argv[i], argc > 2);
			i++;
		}
	}
	return (status);
}
