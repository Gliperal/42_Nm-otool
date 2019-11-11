/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 14:50:43 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/11 15:31:25 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAGIC_H
# define MAGIC_H

# include <stdint.h>

int	is_fat(uint32_t magic);
int	is_macho(uint32_t magic);
int	is_32_bit(uint32_t magic);
int	is_64_bit(uint32_t magic);
int	is_big_endian(uint32_t magic);

#endif
