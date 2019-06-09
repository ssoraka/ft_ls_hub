/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 20:15:52 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/19 17:57:42 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		ft_del_content(void *mem, size_t size)
{
	ft_bzero(mem, size);
	ft_memdel(&mem);
}

t_list			*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list *begin_list;
	t_list *list;

	if (lst == 0)
		return (0);
	if (f == 0)
		return (lst);
	list = f(lst);
	begin_list = list;
	while (begin_list && lst->next != NULL)
	{
		lst = lst->next;
		if ((list->next = f(lst)) == 0)
		{
			ft_lstdel(&begin_list, &ft_del_content);
			return (begin_list);
		}
		list = list->next;
	}
	return (begin_list);
}
