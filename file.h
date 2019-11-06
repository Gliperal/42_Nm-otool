/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 12:27:27 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/06 12:41:52 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_H
# define FILE_H

# include <string.h>

typedef struct	s_file
{
	const char	*filename;
	char		*contents;
	size_t		alloc_size;
	size_t		size;
}				t_file;

t_file			*ft_open(const char *filename);
void			ft_close(t_file *file);

#endif
