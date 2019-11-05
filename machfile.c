/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   machfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/04 16:27:16 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO REMOVE
#include <inttypes.h>

#include <stdlib.h>

#include "machfile.h"

#include "libft/libft.h"

// OTOOL -T

static void	do_sections_64(t_machfile *machfile, size_t offset, uint32_t nsects)
{
	struct section_64 **new_sects;
	// TODO check end of file

	new_sects = malloc((machfile->nsects + nsects) * sizeof(struct section_64 *));
	if (new_sects == NULL)
		return ; // return something to stop the parsing
	for (int i = 0; i < machfile->nsects; i++)
		new_sects[i] = machfile->sects[i];
	free(machfile->sects);
	machfile->sects = new_sects;
	for (uint32_t i = 0; i < nsects; i++)
	{
		// TODO check end of file
		machfile->sects[machfile->nsects + i] = (void *)machfile->file->contents + offset + (i * sizeof(struct section_64));
	}
	machfile->nsects += nsects;
}

static void	do_segment_64(t_machfile *machfile, size_t offset)
{
	struct segment_command_64 *cmd;

	if (machfile->file->size < offset + sizeof(struct segment_command_64))
	{
		ft_putstr("Unexpected end of file.\n");
		return ; // return something to stop the parsing
	}
	cmd = (void *)machfile->file->contents + offset;
	do_sections_64(machfile, offset + sizeof(struct segment_command_64), cmd->nsects);
}

static void	do_symtab(t_machfile *machfile, size_t offset)
{
	struct symtab_command *cmd;

	if (machfile->file->size < offset + sizeof(struct symtab_command))
	{
		ft_putstr("Unexpected end of file.\n");
		return ; // return something to stop the parsing
	}
	cmd = (void *)machfile->file->contents + offset;

	size_t symtab_size = sizeof(struct nlist_64) * cmd->nsyms;
	if (machfile->file->size < cmd->symoff + symtab_size)
	{
		ft_putstr("Unexpected end of file.\n");
		return ; // return something to stop the parsing
	}
	machfile->symtab = malloc(symtab_size);
	if (machfile->symtab == NULL)
		return ; // return something to stop the parsing
	ft_memcpy(machfile->symtab, machfile->file->contents + cmd->symoff, symtab_size);
	machfile->nsyms = cmd->nsyms;
	// TODO check end of file
	machfile->strtab = machfile->file->contents + cmd->stroff;
}

// COMMON

static void	do_cmds(t_machfile *machfile, size_t offset, uint32_t ncmds)
{
	struct load_command *header;

	for (int i = 0; i < ncmds; i++)
	{
		if (machfile->file->size < offset + sizeof(struct load_command))
		{
			ft_putstr("Unexpected end of file.\n");
			return ; // return something to stop the parsing
		}
		header = (void *)machfile->file->contents + offset;
		if (header->cmd == LC_SEGMENT_64)
			do_segment_64(machfile, offset);
		else if (header->cmd == LC_SYMTAB)
			do_symtab(machfile, offset);
		offset += header->cmdsize;
	}
}

static int	do_things(t_machfile *machfile)
{
	struct mach_header_64 *header;

	if (machfile->file->size < sizeof(struct mach_header_64))
	{
		ft_putstr("Unexpected end of file.\n");
		return (-1);
	}
	header = (void *)machfile->file->contents;
	if (header->magic == MH_CIGAM_64)
	{
		ft_putstr("Reverse byte order not currently supported.\n");
		return (-1);
	}
	if (header->magic != MH_MAGIC_64)
	{
		ft_putstr("Not an object file.\n");
		return (-1);
	}
	do_cmds(machfile, sizeof(struct mach_header_64), header->ncmds);
	return (0);
}

t_machfile	*load_machfile(const char *filename)
{
	t_machfile *machfile;
	t_file *file;
	int status;

	machfile = malloc(sizeof(t_machfile));
	if (!machfile)
		return (NULL);
	machfile->file = ft_open(filename);
	if (!machfile->file)
	{
		free(machfile);
		return (NULL);
	}
	machfile->sects = NULL;
	machfile->nsects = 0;
	machfile->symtab = NULL;
	machfile->strtab = NULL;
	status = do_things(machfile);
	if (status == -1 || machfile->sects == NULL /* etc */)
	{
		ft_close(machfile->file);
		free(machfile);
		return (NULL);
	}
	return (machfile);
}

void	unload_machfile(t_machfile *machfile)
{
	ft_close(machfile->file);
	free(machfile->sects);
	free(machfile);
}
