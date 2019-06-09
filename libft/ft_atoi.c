/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 16:42:49 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/15 20:25:23 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_atoi(const char *str)
{
	size_t	i;
	size_t	nb;
	int		znak;

	nb = 00;
	i = 0;
	znak = 1;
	while (str[i] != '\0' && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			znak = -1;
		i++;
	}
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		if (nb > 922337203685477580)
			return ((1 + znak) / (-2));
		if (nb == 922337203685477580 && str[i] - 48 > 8 + (1 + znak) / (-2))
			return ((1 + znak) / (-2));
		nb = 10 * nb + (str[i] - 48);
		i++;
	}
	return (znak * nb);
}
