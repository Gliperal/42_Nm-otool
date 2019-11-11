/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/11 15:33:29 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "machfile.h"
#include "misc.h"
#include "magic.h"

#include <mach-o/loader.h>

#include "libft/libft.h"

static int	per_file(const char *filename, int multiple_files)
{
	t_file		*file;
	t_machfile	*machfile;

	if ((file = ft_open(filename)) == NULL)
		return (1);
	if (file->size >= 8 && ft_strncmp(file->contents, "!<arch>\n", 8) == 0)
	{
		ft_putstr("Archive files not currently supported.\n");
		ft_close(file);
		return (1);
	}
	if (file->size < 4)
	{
		ft_putstr("Unexpected end of file.\n");
		ft_close(file);
		return (1);
	}
	if (is_fat(*(uint32_t *)file->contents))
		if (!trim_fat(file))
			return (1);
	if ((machfile = load_machfile(file)) == NULL)
		return (1);
	display(machfile, multiple_files);
	unload_machfile(machfile);
	return (0);
}

int			main(int argc, const char **argv)
{
	int status;
	int i;

	status = 0;
	if (argc < 2)
		status |= per_file("a.out", 0);
	else
	{
		i = 1;
		while (i < argc)
		{
			status |= per_file(argv[i], argc > 2);
			i++;
		}
	}
	return (status);
}
