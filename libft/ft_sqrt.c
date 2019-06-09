/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 12:33:58 by ssoraka           #+#    #+#             */
/*   Updated: 2019/03/02 20:15:39 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_sqrt(size_t nb)
{
	size_t	root;
	size_t	root2;

	if (nb == 0 || nb == 2 || nb == 3)
		return (0);
	if (nb == 1)
		return (1);
	root = nb >> 1;
	while (root != (root2 = nb / root))
		root = ((root + root2) >> 1);
	if (nb % root)
		return (0);
	return (root);
}
