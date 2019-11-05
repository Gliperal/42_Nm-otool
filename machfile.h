/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   machfile.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 14:47:14 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/04 16:23:07 by nwhitlow         ###   ########.fr       */
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

t_machfile		*load_machfile(const char *filename);
void			unload_machfile(t_machfile *machfile);

#endif
