/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/06 14:23:50 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO REMOVE
#include <inttypes.h>

#include "machfile.h"

#include "libft/libft.h"

void	print_hex_uint32_t(uint64_t n);

static void	sort(t_machfile *machfile)
{
	int sorted;
	sorted = 1;
	for (uint32_t i = 0; i < machfile->nsyms - 1; i++)
	{
		struct nlist_64 *sym1;
		sym1 = machfile->symtab + i;
		struct nlist_64 *sym2;
		sym2 = machfile->symtab + i + 1;
		const char *str1 = machfile->strtab + sym1->n_un.n_strx;
		const char *str2 = machfile->strtab + sym2->n_un.n_strx;
		if (ft_strcmp(str2, str1) < 0)
		{
			struct nlist_64 tmp;
			ft_memcpy(&tmp, sym1, sizeof(struct nlist_64));
			ft_memcpy(sym1, sym2, sizeof(struct nlist_64));
			ft_memcpy(sym2, &tmp, sizeof(struct nlist_64));
			sorted = 0;
			i = -1;
		}
	}
}

static char	sym_type(struct nlist_64 *sym, t_machfile *machfile)
{
	uint8_t ntype;
	char type;

	ntype = sym->n_type;
	if (ntype & N_STAB)
		return (0);
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
//		else if (ft_strcmp(sectname, "__common") == 0) // TODO double check sectname
//			type = 'c';
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

void	display(t_machfile *machfile)
{
	char type;
	struct nlist_64 *sym;
	uint32_t i;

	sort(machfile);
	i = 0;
	while (i < machfile->nsyms)
	{
		sym = machfile->symtab + i;
		type = sym_type(sym, machfile);
		if (type != 0)
		{
			if ((sym->n_type & N_TYPE) == N_SECT)
				print_hex_uint32_t(sym->n_value);
			else
				ft_printf("                ");
			ft_printf(" %c ", sym_type(sym, machfile));
			// TODO check end of file
			ft_printf("%s\n", machfile->strtab + sym->n_un.n_strx);
		}
		i++;
	}
}
