/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 14:50:43 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/07 20:56:34 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mach-o/fat.h>
#include <mach-o/loader.h>

int	is_fat(uint32_t magic)
{
	return (magic == FAT_MAGIC || magic == FAT_CIGAM);
}

int	is_macho(uint32_t magic)
{
	return (magic == MH_MAGIC || magic == MH_CIGAM ||
			magic == MH_MAGIC_64 || magic == MH_CIGAM_64);
}

int	is_32_bit(uint32_t magic)
{
	return (magic == MH_MAGIC || magic == MH_CIGAM);
}

int	is_64_bit(uint32_t magic)
{
	return (magic == MH_MAGIC_64 || magic == MH_CIGAM_64);
}

int	is_big_endian(uint32_t magic)
{
	return (magic == FAT_CIGAM || magic == MH_CIGAM || magic == MH_CIGAM_64);
}
