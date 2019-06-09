/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iterative_power.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 22:58:27 by ssoraka           #+#    #+#             */
/*   Updated: 2019/03/02 21:49:39 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_power(long nb, int power)
{
	if (power < 0 || nb != (nb * 10) / 10)
		return (0);
	if (power == 0 || nb == 1)
		return (1);
	if (power == 1)
		return (nb);
	if (power % 2)
		return (nb * ft_power(nb, power - 1));
	else
		return (ft_power(nb * nb, power >> 1));
}
