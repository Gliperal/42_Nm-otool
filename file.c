/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 11:48:40 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/11 17:28:37 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>

#include "libft/libft.h"

#include "file.h"

static t_file	*file_new(void)
{
	t_file *file;

	file = malloc(sizeof(t_file));
	if (!file)
		return (NULL);
	file->alloc_size = 64;
	file->size = 0;
	file->contents = malloc(file->alloc_size * sizeof(char));
	if (file->contents == NULL)
	{
		free(file);
		return (NULL);
	}
	return (file);
}

static int		file_read(t_file *file, int fd)
{
	char	buffer[1024];
	int		bytes_read;
	char	*new_contents;

	while ((bytes_read = read(fd, buffer, 1024)) > 0)
	{
		while (file->size + bytes_read > file->alloc_size)
		{
			new_contents = malloc(2 * file->alloc_size * sizeof(char));
			if (!new_contents)
				return (-1);
			ft_memcpy(new_contents, file->contents, file->size);
			free(file->contents);
			file->contents = new_contents;
			file->alloc_size *= 2;
		}
		ft_memcpy(file->contents + file->size, buffer, bytes_read);
		file->size += bytes_read;
	}
	if (bytes_read == 0)
		return (0);
	ft_putstr("Read failure.\n");
	return (-1);
}

void			ft_close(t_file *file)
{
	free(file->contents);
	free(file);
}

t_file			*ft_open(const char *filename)
{
	int		fd;
	t_file	*file;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("Failed to open %s\n", filename);
		return (NULL);
	}
	file = file_new();
	file->filename = filename;
	if (!file)
		return (NULL);
	if (file_read(file, fd) == -1)
	{
		ft_close(file);
		file = NULL;
	}
	close(fd);
	return (file);
}
