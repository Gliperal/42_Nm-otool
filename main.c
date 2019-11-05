/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/04 16:36:37 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO REMOVE
#include <inttypes.h>

#include "machfile.h"

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

static void	otool(t_machfile *machfile)
{
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

// NM

static char	sym_type(struct nlist_64 *sym, t_machfile *machfile)
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
		uint8_t i = sym->n_sect;
		if (i > machfile->nsects)
		{
			// TODO
			return 0;
		}
		char *sectname = machfile->sects[i - 1]->sectname;
		if (ft_strcmp(sectname, "__text") == 0)
			type = 't';
		else if (ft_strcmp(sectname, "__data") == 0)
			type = 'd';
		else if (ft_strcmp(sectname, "__bss") == 0)
			type = 'b';
		else if (ft_strcmp(sectname, "__common") == 0) // TODO double check sectname
			type = 'c';
		else
			type = 's';
	}
	else if ((ntype & N_TYPE) == N_PBUD)
		type = '?';
	else if ((ntype & N_TYPE) == N_INDR)
		type = 'i';
	if (ntype & N_EXT)
		type = ft_toupper(type);
	return (type);
}

static void	nm(t_machfile *machfile)
{
	for (uint32_t i = 0; i < machfile->nsyms; i++)
	{
		struct nlist_64 *sym;
		sym = machfile->symtab + i;
		if ((sym->n_type & N_TYPE) == N_SECT)
			print_hex_uint32_t(sym->n_value);
		else
			ft_printf("                ");
		ft_printf(" %c ", sym_type(sym, machfile));
		// TODO check end of file
		ft_printf("%s\n", machfile->strtab + sym->n_un.n_strx);
	}
}

int main(int argc, const char **argv)
{
	t_machfile *machfile;
	if (argc < 2)
		machfile = load_machfile("test_files/malloc.so");
	else
		machfile = load_machfile(argv[1]);
	if (!machfile)
		return (-1);
	nm(machfile);
	otool(machfile);
	unload_machfile(machfile);
}
