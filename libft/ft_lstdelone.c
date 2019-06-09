/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 19:01:37 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/17 19:23:12 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list **alst, void (*del)(void *, size_t))
{
	if (alst == 0 || *alst == 0 || del == 0)
		return ;
	del((*alst)->content, (*alst)->content_size);
	(*alst)->content = 0;
	(*alst)->content_size = 0;
	free(*alst);
	*alst = 0;
}
