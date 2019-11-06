/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:24:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/06 15:01:49 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "machfile.h"
#include "misc.h"

#include "libft/libft.h"

int	main(int argc, const char **argv)
{
	t_file		*file;
	t_machfile	*machfile;

	if (argc < 2)
		file = ft_open("a.out");
	else
		file = ft_open(argv[1]);
	if (!file)
		return (-1);
	if (file->size >= 8 && ft_strncmp(file->contents, "!<arch>\n", 8) == 0)
	{
		ft_putstr("Archive files not currently supported.\n");
		ft_close(file);
		return (-1);
	}
	machfile = load_machfile(file);
	if (!machfile)
		return (-1);
	display(machfile);
	unload_machfile(machfile);
}
