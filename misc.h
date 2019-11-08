/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 15:01:06 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/07 17:49:18 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MISC_H
# define MISC_H

# define PRINT_HEX_UINT32_T(N) ft_printf("%.8x%.8x", N >> 32, N)

void	display(t_machfile *machfile, int multiple_files);

#endif
