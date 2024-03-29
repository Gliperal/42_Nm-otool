/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   machfile.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 14:47:14 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/08 14:43:56 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACHFILE_H
# define MACHFILE_H

# include <mach-o/loader.h>
# include <mach-o/nlist.h>

# include "file.h"

typedef struct	s_machfile
{
	t_file				*file;
	int					reverse_byte_order;
	int					is_32_bit;
	struct section		**sects_32;
	struct section_64	**sects_64;
	unsigned int		nsects;
	struct nlist		*symtab_32;
	struct nlist_64		*symtab_64;
	uint32_t			nsyms;
	char				*strtab;
}				t_machfile;

t_machfile		*load_machfile(t_file *file);
void			unload_machfile(t_machfile *machfile);

/*
** This is MY function! Do not use this! The only reason it's non-static and
** floating around in a header file is because machfile.c was 7 functions long.
*/

int				do_things_32(t_machfile *machfile);
int				do_things_64(t_machfile *machfile);

#endif
