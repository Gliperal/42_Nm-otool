/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 16:04:52 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/11/08 16:15:20 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SORT_H
# define SORT_H

void	quicksort_32(t_machfile *machfile, uint32_t low, uint32_t high);
void	quicksort_64(t_machfile *machfile, uint32_t low, uint32_t high);
void	sort(t_machfile *machfile);

#endif
