/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:30:29 by ssoraka           #+#    #+#             */
/*   Updated: 2019/06/10 17:30:29 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
**	вставка элемента в список
*/

void	ft_place_dlist(t_dlist **begin_list, t_dlist *new)
{
	t_dlist	*check;
	int		action;

	action = NOTHING;
	if (*begin_list == 0)
	{
		*begin_list = new;
		return ;
	}
	check = *begin_list;
	while (check)
	{
		if ((action = ft_check_suitable_place(check, new)) != NOTHING)
			break ;
		if (check->next == NULL)
			break ;
		check = check->next;
	}
	ft_list_swap(check, new, action, begin_list);
}

/*
**	проверяем, стоит ли вставлять рассматриваемый элемент в данном месте списка
*/

int		ft_check_suitable_place(t_dlist *list, t_dlist *new)
{
	int		abc;

	abc = 0;
	if (list->stats && ft_gflags('S', 0))
	{
		if (list && list->stats->st_size < new->stats->st_size)
			return (INSERT_LISTS);
		else if (list && list->stats->st_size == new->stats->st_size)
			abc = 1;
	}
	else if (list->stats && ft_gflags('t', 0))
	{
		if (list && list->time < new->time)
			return (INSERT_LISTS);
		else if (list && list->time == new->time)
			abc = 1;
	}
	else
		abc = 1;
	if (abc && list && ft_strcmp(list->name, new->name) >= 0)
		return (INSERT_LISTS);
	return (ADD_LIST);
}

/*
**	вставка элемента в список
*/

void	ft_list_swap(t_dlist *list, t_dlist *new, int action,
			t_dlist **begin_list)
{
	t_dlist	*tmp;

	if (action == ADD_LIST && *begin_list != NULL)
	{
		list->next = new;
		new->parent = list;
	}
	else if (action == DEL_LIST)
		ft_dlistdel(&new);
	else if (action == INSERT_LISTS && list != *begin_list)
	{
		tmp = list->parent;
		tmp->next = new;
		new->parent = tmp;
		new->next = list;
		list->parent = new;
	}
	else
	{
		new->next = list;
		list->parent = new;
		*begin_list = new;
	}
}

/*
**	собраем список ошибочных названий по алфавиту
*/

t_dlist	*ft_create_sort_list(t_dlist **begin_list, char *name, char *path,
			struct stat *stats)
{
	t_dlist		*new_list;

	if (name[0] == '\0')
	{
		ft_putstr_fd("ls: fts_open: No such file or directory\n", 2);
		exit(0);
	}
	new_list = ft_dlistnew(name, path);
	ft_stats_to_list(new_list, stats);
	ft_place_dlist(begin_list, new_list);
	return (new_list);
}

/*
**	понадобилось для вывода на печать по столбикам
*/

t_dlist	*ft_listrevers(t_dlist *begin_list)
{
	t_dlist	*list;
	t_dlist	*temp;

	if (!begin_list)
		return (NULL);
	while (begin_list->next)
	{
		list = begin_list;
		begin_list = begin_list->next;
		temp = list->next;
		list->next = list->parent;
		list->parent = temp;
	}
	list = begin_list;
	temp = list->next;
	list->next = list->parent;
	list->parent = temp;
	return (list);
}
