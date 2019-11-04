/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/04 14:28:47 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO REMOVE
#include <inttypes.h>

#include <mach-o/loader.h>

#include "file.h"

#include "libft/libft.h"

// OTOOL -T

void	print_hex_uint32_t(uint64_t n)
{
	ft_printf("%.8x%.8x", n >> 32, n);
}

void	hexdump(void *data, uint32_t size, uint64_t addr)
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

void	do_sections_64(t_file *file, size_t offset, uint32_t nsects)
{
	struct section_64 *sects = (void *)file->contents + offset;
	ft_printf("%" PRId32 " sects\n", nsects);
	for (uint32_t i = 0; i < nsects; i++)
	{
		// TODO check end of file
		struct section_64 *s = sects + i;
//		ft_printf("align = %" PRId32 "\n", s->align);
		ft_printf("section : sectname = %.8s\n", s->sectname);
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

// NM

#include <mach-o/nlist.h>

char	sym_type(struct nlist_64 *sym)
{
	uint8_t ntype;
	char type;

	ntype = sym->n_type;
	if (ntype & N_STAB)
		return ('-');
	else if ((ntype & N_TYPE) == N_UNDF)
		type = 'u';
	else if ((ntype & N_TYPE) == N_ABS)
		type = 'a';
	else if ((ntype & N_TYPE) == N_SECT)
	{
		uint8_t sect = sym->n_sect;
		// depending on section type
		// T = __text
		// D = __data
		// B = __bss
		// C = ?common
		// S = other
		type = 't';
	}
	else if ((ntype & N_TYPE) == N_PBUD)
		type = '?';
	else if ((ntype & N_TYPE) == N_INDR)
		type = 'i';
	if (ntype & N_EXT)
		type = ft_toupper(type);
	return (type);
}

void	do_symtab(t_file *file, size_t offset)
{
	struct symtab_command *cmd;

	if (file->size < offset + sizeof(struct symtab_command))
	{
		ft_putstr("Unexpected end of file.\n");
		return ; // return something to stop the parsing
	}
	cmd = (void *)file->contents + offset;

	// TODO check end of file
	struct nlist_64 *syms = (void *)file->contents + cmd->symoff;
	char *strtab = file->contents + cmd->stroff;
	for (uint32_t i = 0; i < cmd->nsyms; i++)
	{
		// TODO check end of file
		if ((syms[i].n_type & N_TYPE) == N_SECT)
		{
			print_hex_uint32_t(syms[i].n_value);
			// TODO check end of file
		}
		else
		{
			ft_printf("                ");
		}
		ft_printf(" %c ", sym_type(syms + i));
		ft_printf("%s\n", strtab + syms[i].n_un.n_strx);
//		ft_printf("n_desc = %" PRId16 "\n", syms[i].n_desc);
	}
}

// COMMON

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
		if (header->cmd == LC_SEGMENT_64)
			do_segment_64(file, offset);
		else if (header->cmd == LC_SYMTAB)
			do_symtab(file, offset);
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
