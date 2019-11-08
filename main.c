/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/07 17:51:12 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO add -WWW to makefile

// TODO REMOVE
#include <inttypes.h>

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

int	trim_fat(t_file *file)
{
	struct fat_header *header = (void *)file->contents;
	uint32_t nfat_arch = swap_endian_32(header->nfat_arch);
	for (uint32_t i = 0; i < nfat_arch; i++)
	{
		struct fat_arch *arch;
		arch = (void *)file->contents + sizeof(struct fat_header) + i * sizeof(struct fat_arch);
		uint32_t offset = swap_endian_32(arch->offset);
		uint32_t size = swap_endian_32(arch->size);
		uint32_t magic = *(uint32_t *)(file->contents + swap_endian_32(arch->offset));
		if (is_macho(magic))
		{
			ft_memmove(file->contents, file->contents + offset, size);
			file->size = size;
			return (1);
		}
	}
	ft_putstr("Fat file didn't contain any useful sections.\n");
	return (0);
}

int	per_file(const char *filename, int multiple_files)
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
