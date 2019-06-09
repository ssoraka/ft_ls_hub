/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_byte_to_bits.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 17:24:20 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/28 17:24:50 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	функция возвращает содержимое переменной var
**	в виде строки битов для вывода
**	ft_putstr(ft_bytes_to_bits((char *)(&var), sizeof(var), "__"));
**	первый аргумент - указатель на 1-ый байт
**	второй - количество байт
**	третий - строка для отображения разделителя между байтами
*/

static char	*ft_byte_to_bits(char c)
{
	unsigned char	chr;
	char			*str;
	int				i;

	i = 0;
	str = ft_strnew(8);
	chr = (unsigned char)c;
	while (i < 8)
	{
		str[7 - i] = chr % 2 + '0';
		chr = chr >> 1;
		i++;
	}
	return (str);
}

char		*ft_bytes_to_bits(char *c, int n, char *separator)
{
	char	*str;
	char	*temp1;
	char	*temp2;
	int		i;

	i = 0;
	str = ft_strnew(0);
	while (i < n)
	{
		n--;
		temp2 = str;
		temp1 = ft_byte_to_bits(*(c + n));
		str = ft_strjoin_free(temp2, temp1, 1, 1);
		temp1 = str;
		if (i < n)
			str = ft_strjoin(temp1, separator);
		else
			temp1 = NULL;
		free(temp1);
	}
	return (str);
}
