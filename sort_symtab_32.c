/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_symtab_32.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 15:06:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/08 16:14:32 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "machfile.h"
#include "sort.h"

#include "libft/libft.h"

static void		swap(t_machfile *machfile, uint32_t a, uint32_t b)
{
	struct nlist tmp;

	if (a == b)
		return ;
	tmp = machfile->symtab_32[a];
	machfile->symtab_32[a] = machfile->symtab_32[b];
	machfile->symtab_32[b] = tmp;
}

static int		cmp(t_machfile *machfil, struct nlist *a, struct nlist *b)
{
	const char	*a_str;
	const char	*b_str;
	int			strdiff;

	a_str = machfil->strtab + a->n_un.n_strx;
	b_str = machfil->strtab + b->n_un.n_strx;
	strdiff = ft_strcmp(a_str, b_str);
	if (strdiff != 0)
		return (strdiff);
	if (a->n_value < b->n_value)
		return (-1);
	else
		return (1);
}

static uint32_t	partition(t_machfile *machfile, uint32_t low, uint32_t high)
{
	uint32_t		p;
	struct nlist	*pivot;
	struct nlist	*sym;

	p = low;
	pivot = machfile->symtab_32 + p;
	low++;
	while (low <= high)
	{
		sym = machfile->symtab_32 + low;
		if (cmp(machfile, sym, pivot) < 0)
			low++;
		else
		{
			swap(machfile, low, high);
			high--;
		}
	}
	swap(machfile, high, p);
	return (high);
}

void			quicksort_32(t_machfile *machfile, uint32_t low, uint32_t high)
{
	uint32_t p;

	if (low + 1 >= high + 1)
		return ;
	p = partition(machfile, low, high);
	quicksort_32(machfile, low, p - 1);
	quicksort_32(machfile, p + 1, high);
}
