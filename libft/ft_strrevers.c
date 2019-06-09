/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrevers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 17:03:39 by ssoraka           #+#    #+#             */
/*   Updated: 2019/05/04 17:03:39 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	эта функция
**	реверсирует строку
*/

char	*ft_strrevers(char *str)
{
	int		i;
	int		len;
	char	chr;

	i = 0;
	if (str == NULL || str[0] == 0)
		return (NULL);
	len = ft_strlen(str);
	while (i < len / 2)
	{
		chr = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = chr;
		i++;
	}
	return (str);
}
