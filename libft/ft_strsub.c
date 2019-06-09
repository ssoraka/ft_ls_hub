/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 20:13:23 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/16 21:02:55 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	size_t	n;
	char	*src;
	char	*dest;

	if (s == 0)
		return (NULL);
	n = ft_strlen(s);
	if ((size_t)start > n)
		return (NULL);
	if (!(dest = ft_strnew(len)))
		return (NULL);
	src = (char *)s + start;
	return (ft_strncpy(dest, src, len));
}
