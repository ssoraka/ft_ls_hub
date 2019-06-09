/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skip_chrs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 11:15:41 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/26 11:15:47 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_char_check(char chr, char *skip)
{
	int i;

	i = 0;
	while (skip[i])
	{
		if (chr == skip[i])
			return (1);
		i++;
	}
	return (0);
}

char		*ft_skip_chrs(char *str, char *skip)
{
	int i;

	i = 0;
	if (str == 0)
		return (NULL);
	if (skip == 0)
		return (str);
	while (str[i])
	{
		if (ft_char_check(str[i], skip) == 0)
			break ;
		i++;
	}
	return (str + i);
}
