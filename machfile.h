/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   machfile.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 14:47:14 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/06 15:43:48 by nwhitlow         ###   ########.fr       */
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
	struct section_64	**sects;
	unsigned int		nsects;
	struct nlist_64		*symtab;
	uint32_t			nsyms;
	char				*strtab;
}				t_machfile;

t_machfile		*load_machfile(t_file *file);
void			unload_machfile(t_machfile *machfile);

/*
** This is MY function! Do not use this! The only reason it's non-static and
** floating around in a header file is because machfile.c was 7 functions long.
*/

int				do_things(t_machfile *machfile);

#endif
