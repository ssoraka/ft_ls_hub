/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ltoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 12:53:16 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/17 17:33:51 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_ltoa(signed long n)
{
	char	temp[21];
	long	num;
	int		znak;

	znak = 1;
	if (n < 0)
		znak = -1;
	ft_bzero(temp, 21);
	num = 0;
	temp[num] = '0';
	while (n != 0 || num == 0)
	{
		temp[num] = '0' + znak * (n % 10);
		n = n / 10;
		num++;
	}
	if (znak < 0)
		temp[num] = '-';
	ft_strrevers(temp);
	return (ft_strdup(temp));
}

char		*ft_ultoa(unsigned long un)
{
	char	temp[21];
	long	num;

	ft_bzero(temp, 21);
	num = 0;
	temp[num] = '0';
	while (un != 0 || num == 0)
	{
		temp[num] = '0' + (un % 10);
		un = un / 10;
		num++;
	}
	ft_strrevers(temp);
	return (ft_strdup(temp));
}
