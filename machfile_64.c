/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   machfile_64.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/08 14:59:41 by nwhitlow         ###   ########.fr       */
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
		new_sects[i] = machfile->sects_64[i];
	free(machfile->sects_64);
	machfile->sects_64 = new_sects;
	sections = (void *)machfile->file->contents + offset;
	i = -1;
	while (++i < nsects)
		machfile->sects_64[machfile->nsects + i] = sections + i;
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
	machfile->symtab_64 = malloc(symtab_size);
	if (machfile->symtab_64 == NULL)
		return (-1);
	ft_memcpy(machfile->symtab_64, machfile->file->contents + cmd->symoff,
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

int			do_things_64(t_machfile *machfile)
{
	struct mach_header_64	*header;
	int						status;

	if (machfile->file->size < sizeof(struct mach_header_64))
	{
		ft_putstr("Unexpected end of file.\n");
		return (-1);
	}
	header = (void *)machfile->file->contents;
	status = do_cmds(machfile, sizeof(struct mach_header_64), header->ncmds);
	if (status == -1)
		return (-1);
	if (machfile->sects_64 == NULL || machfile->symtab_64 == NULL)
	{
		ft_putstr("Missing elements in file.\n");
		return (-1);
	}
	return (0);
}
