/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_arr_new.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 10:24:21 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/26 10:24:21 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_str_arr_new(int row, int column)
{
	int		i;
	char	**str;

	i = 0;
	if ((str = (char**)ft_memalloc(sizeof(str) * (row + 1))) == NULL)
		return (0);
	while (i < row)
	{
		if ((str[i] = ft_strnew(column)) == NULL)
		{
			ft_str_arr_free(str);
			return (0);
		}
		i++;
	}
	return (str);
}
