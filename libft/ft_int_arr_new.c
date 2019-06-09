/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_int_arr_new.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 10:25:25 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/26 10:25:25 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		**ft_int_arr_new(int row, int column)
{
	int		i;
	int		**tab;

	i = 0;
	if ((tab = (int **)ft_memalloc(sizeof(tab) * row)) == NULL)
		return (0);
	while (i < row)
	{
		if ((tab[i] = (int *)ft_memalloc(sizeof(*tab) * column)) == NULL)
		{
			ft_int_arr_free(tab, i - 1);
			return (0);
		}
		i++;
	}
	return (tab);
}
