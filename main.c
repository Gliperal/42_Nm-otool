/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/03 15:44:13 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO REMOVE
#include <inttypes.h>

#include <mach-o/loader.h>

#include "file.h"

#include "libft/libft.h"

void	print_hex_uint32_t(uint32_t n)
{
	// TODO
	ft_printf("00000000%.8x\t", n);
}

void	hexdump(void *data, uint32_t size, uint32_t addr)
{
	unsigned char *d;
	int i;

	d = data;
	while (1)
	{
		print_hex_uint32_t(addr);
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

void	do_sections_64(t_file *file, size_t offset, uint32_t nsects)
{
	struct section_64 *sects = (void *)file->contents + offset;
	ft_printf("%" PRId32 " sects\n", nsects);
	for (uint32_t i = 0; i < nsects; i++)
	{
		struct section_64 *s = sects + i;
//		ft_printf("align = %" PRId32 "\n", s->align);
		if ((ft_strcmp(s->sectname, "__text") == 0) && (ft_strcmp(s->segname, "__TEXT") == 0))
		{
			ft_printf("Contents of (__TEXT,__text) section\n");
			hexdump(file->contents + s->offset, s->size, s->addr);
		}
	}
}

void	do_segment_64(t_file *file, size_t offset)
{
	struct segment_command_64 *cmd;

	if (file->size < offset + sizeof(struct segment_command_64))
	{
		ft_putstr("Unexpected end of file.\n");
		return ; // return something to stop the parsing
	}
	cmd = (void *)file->contents + offset;
	do_sections_64(file, offset + sizeof(struct segment_command_64), cmd->nsects);
}

void	do_cmds(t_file *file, size_t offset, uint32_t ncmds)
{
	struct load_command *header;

	for (int i = 0; i < ncmds; i++)
	{
		if (file->size < offset + sizeof(struct load_command))
		{
			ft_putstr("Unexpected end of file.\n");
			return ; // return something to stop the parsing
		}
		header = (void *)file->contents + offset;
		ft_printf("cmd %d\n", header->cmd);
		if (header->cmd == LC_SEGMENT_64)
			do_segment_64(file, offset);
		else if (header->cmd == LC_SYMTAB)
			ft_printf("LC_SYMTAB\n");
		else if (header->cmd == LC_DYSYMTAB)
			ft_printf("LC_DYSTMTAB\n");
		else if (header->cmd == LC_VERSION_MIN_MACOSX)
			ft_printf("LC_VERSION_MIN_MACOSX\n");
		offset += header->cmdsize;
	}
}

void	do_things(t_file *file)
{
	struct mach_header_64 *header;

	if (file->size < sizeof(struct mach_header_64))
	{
		ft_putstr("Unexpected end of file.\n");
		return ;
	}
	header = (void *)file->contents;
	if (header->magic == MH_CIGAM_64)
	{
		ft_putstr("Reverse byte order not currently supported.\n");
		return ;
	}
	if (header->magic != MH_MAGIC_64)
	{
		ft_putstr("Not an object file.\n");
		return ;
	}
	do_cmds(file, sizeof(struct mach_header_64), header->ncmds);
}

int main(int argc, const char **argv)
{
	t_file *file;
	if (argc < 2)
		file = ft_open("test_files/malloc.so");
	else
		file = ft_open(argv[1]);
	if (!file)
		return (-1);
	do_things(file);
	ft_close(file);
}
