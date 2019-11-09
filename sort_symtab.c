/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_symtab.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 15:06:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/08 16:17:40 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "machfile.h"
#include "sort.h"

void	sort(t_machfile *machfile)
{
	if (machfile->is_32_bit)
		quicksort_32(machfile, 0, machfile->nsyms - 1);
	else
		quicksort_64(machfile, 0, machfile->nsyms - 1);
}
