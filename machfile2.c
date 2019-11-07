/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   machfile2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/07 15:07:55 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "machfile.h"
#include "magic.h"

#include "libft/libft.h"

/*
** OTOOL -T
*/

static int	do_sections_64(t_machfile *machfile, size_t offset, uint32_t nsects)
{
	struct section_64	**new_sects;
	struct section_64	*sections;
	uint32_t			i;

	if (machfile->file->size < offset + nsects * sizeof(struct section_64))
	{
		ft_putstr("Unexpected end of file.\n");
		return (-1);
	}
	new_sects = malloc((machfile->nsects + nsects) *
												sizeof(struct section_64 *));
	if (new_sects == NULL)
		return (-1);
	i = -1;
	while (++i < machfile->nsects)
		new_sects[i] = machfile->sects[i];
	free(machfile->sects);
	machfile->sects = new_sects;
	sections = (void *)machfile->file->contents + offset;
	i = -1;
	while (++i < nsects)
		machfile->sects[machfile->nsects + i] = sections + i;
	machfile->nsects += nsects;
	return (0);
}

static int	do_segment_64(t_machfile *machfile, size_t offset)
{
	struct segment_command_64	*cmd;
	int							status;

	if (machfile->file->size < offset + sizeof(struct segment_command_64))
	{
		ft_putstr("Unexpected end of file.\n");
		return (-1);
	}
	cmd = (void *)machfile->file->contents + offset;
	status = do_sections_64(machfile,
					offset + sizeof(struct segment_command_64), cmd->nsects);
	return (status);
}

/*
** NM
*/

static int	do_symtab(t_machfile *machfile, size_t offset)
{
	struct symtab_command	*cmd;
	size_t					symtab_size;

	if (machfile->file->size < offset + sizeof(struct symtab_command))
		return (ft_error("Unexpected end of file.\n", -1));
	cmd = (void *)machfile->file->contents + offset;
	symtab_size = sizeof(struct nlist_64) * cmd->nsyms;
	if (machfile->file->size < cmd->symoff + symtab_size)
		return (ft_error("Unexpected end of file.\n", -1));
	machfile->symtab = malloc(symtab_size);
	if (machfile->symtab == NULL)
		return (-1);
	ft_memcpy(machfile->symtab, machfile->file->contents + cmd->symoff,
																symtab_size);
	machfile->nsyms = cmd->nsyms;
	if (machfile->file->size < cmd->stroff + cmd->strsize)
		return (ft_error("Unexpected end of file.\n", -1));
	machfile->strtab = machfile->file->contents + cmd->stroff;
	return (0);
}

/*
** COMMON
*/

static int	do_cmds(t_machfile *machfile, size_t offset, uint32_t ncmds)
{
	struct load_command *header;

	while (ncmds--)
	{
		if (machfile->file->size < offset + sizeof(struct load_command))
		{
			ft_putstr("Unexpected end of file.\n");
			return (-1);
		}
		header = (void *)machfile->file->contents + offset;
		if (header->cmd == LC_SEGMENT_64)
		{
			if (do_segment_64(machfile, offset) == -1)
				return (-1);
		}
		else if (header->cmd == LC_SYMTAB)
			if (do_symtab(machfile, offset) == -1)
				return (-1);
		offset += header->cmdsize;
	}
	return (0);
}

int			do_things(t_machfile *machfile)
{
	struct mach_header_64 *header;

	if (machfile->file->size < sizeof(struct mach_header_64))
	{
		ft_putstr("Unexpected end of file.\n");
		return (-1);
	}
	header = (void *)machfile->file->contents;
	if (!is_macho(header->magic))
	{
		ft_putstr("Not an object file.\n");
		return (-1);
	}
	machfile->reverse_byte_order = is_big_endian(header->magic);
	if (machfile->reverse_byte_order)
	{
		ft_putstr("Reverse byte order not currently supported.\n");
		return (-1);
	}
	if (is_32_bit(header->magic))
	{
		ft_putstr("32-bit not currently supported.\n");
		return (-1);
	}
	return (do_cmds(machfile, sizeof(struct mach_header_64), header->ncmds));
}
