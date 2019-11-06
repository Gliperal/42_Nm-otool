/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/05 15:51:49 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO REMOVE
#include <inttypes.h>

#include "machfile.h"

#include "libft/libft.h"

void	print_hex_uint32_t(uint64_t n);

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

void	display(t_machfile *machfile)
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
