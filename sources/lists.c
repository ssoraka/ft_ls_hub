/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:30:10 by ssoraka           #+#    #+#             */
/*   Updated: 2019/06/10 17:30:11 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
**	создаем новый элемент списка
*/

t_dlist	*ft_dlistnew(char *name, char *path)
{
	t_dlist	*tmp;

	if ((tmp = (t_dlist *)ft_memalloc(sizeof(t_dlist))) == 0)
		return (0);
	tmp->next = NULL;
	tmp->parent = NULL;
	tmp->name = ft_strdup(name);
	tmp->path = path;
	tmp->link = ft_readlink(path);
	return (tmp);
}

/*
** удаляем весь список
*/

void	ft_dlistdel(t_dlist **begin_list)
{
	t_dlist		*prev_list;

	if (begin_list == NULL || *begin_list == NULL)
		return ;
	while (*begin_list != NULL)
	{
		prev_list = *begin_list;
		*begin_list = (*begin_list)->next;
		free(prev_list->stats);
		free(prev_list->path);
		free(prev_list->link);
		free(prev_list->mtime);
		free(prev_list->mode);
		free(prev_list->n_link);
		free(prev_list->blocks);
		free(prev_list->size);
		free(prev_list->name);
		free(prev_list->user);
		free(prev_list->group);
		free(prev_list);
	}
}
