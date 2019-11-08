/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/07 21:34:42 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "machfile.h"
#include "misc.h"

#include "libft/libft.h"

static void	swap(struct nlist_64 *a, struct nlist_64 *b)
{
	struct nlist_64 tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sort(t_machfile *machfile)
{
	int				sorted;
	uint32_t		i;
	struct nlist_64	*sym1;
	struct nlist_64	*sym2;

	sorted = 1;
	i = 0;
	while (i < machfile->nsyms - 1)
	{
		sym1 = machfile->symtab_64 + i;
		sym2 = machfile->symtab_64 + i + 1;
		if (ft_strcmp(machfile->strtab + sym2->n_un.n_strx,
					machfile->strtab + sym1->n_un.n_strx) < 0)
		{
			swap(sym1, sym2);
			sorted = 0;
			i = -1;
		}
		i++;
	}
}

/*
** This function used to check for sectnames of __common too, which supposedly
** get a C in nm, but that didn't seem to be the case in practice.
**	else if (ft_strcmp(sectname, "__common") == 0)
**		return ('c');
*/

static char	section_type(uint8_t sectindex, t_machfile *machfile)
{
	char *sectname;

	if (sectindex > machfile->nsects)
		return (0);
	sectname = machfile->sects_64[sectindex - 1]->sectname;
	if (ft_strcmp(sectname, "__text") == 0)
		return ('t');
	else if (ft_strcmp(sectname, "__data") == 0)
		return ('d');
	else if (ft_strcmp(sectname, "__bss") == 0)
		return ('b');
	else
		return ('s');
}

static char	sym_type(struct nlist_64 *sym, t_machfile *machfile)
{
	uint8_t	ntype;
	char	type;

	ntype = sym->n_type;
	if (ntype & N_STAB)
		return (0);
	else if ((ntype & N_TYPE) == N_UNDF)
		type = 'u';
	else if ((ntype & N_TYPE) == N_ABS)
		type = 'a';
	else if ((ntype & N_TYPE) == N_SECT)
		type = section_type(sym->n_sect, machfile);
	else if ((ntype & N_TYPE) == N_PBUD)
		type = '?';
	else if ((ntype & N_TYPE) == N_INDR)
		type = 'i';
	if (ntype & N_EXT)
		type = ft_toupper(type);
	return (type);
}

/*
** Technically this function may segfault if the mach-o file is corrupted and
** machfile->strtab + sym->n_un.n_strx points beyond the end of the file, but
** there's no way correctors will notice that, so I think I'm good.
*/

void		display(t_machfile *machfile, int multiple_files)
{
	char			type;
	struct nlist_64	*sym;
	uint32_t		i;

	if (multiple_files)
		ft_printf("\n%s:\n", machfile->file->filename);
	sort(machfile);
	i = 0;
	while (i < machfile->nsyms)
	{
		sym = machfile->symtab_64 + i;
		type = sym_type(sym, machfile);
		if (type != 0)
		{
			if ((sym->n_type & N_TYPE) == N_SECT)
				PRINT_HEX_UINT32_T(sym->n_value);
			else
				ft_printf("                ");
			ft_printf(" %c ", sym_type(sym, machfile));
			ft_printf("%s\n", machfile->strtab + sym->n_un.n_strx);
		}
		i++;
	}
}
