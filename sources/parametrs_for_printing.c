/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parametrs_for_printing.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:16:50 by ssoraka           #+#    #+#             */
/*   Updated: 2019/06/10 17:16:51 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
**	определение самых длинных имен и общего количества блоков для колонок вывода
*/

t_plist	*ft_find_max_strlens_in_lists(t_dlist *list)
{
	t_plist	*g;

	if ((g = (t_plist *)ft_memalloc(sizeof(t_plist))) == 0)
		return (0);
	while (list)
	{
		if (ft_gflags('l', 0) || ft_gflags('s', 0))
			g->total = g->total + list->stats->st_blocks;
		if (ft_gflags('l', 0))
		{
			g->maxnlink = ft_max(g->maxnlink, ft_strlen(list->n_link));
			g->maxuser = ft_max(g->maxuser, ft_strlen(list->user));
			g->maxgroup = ft_max(g->maxgroup, ft_strlen(list->group));
			g->maxsize = ft_max(g->maxsize, ft_strlen(list->size));
		}
		if (ft_gflags('s', 0))
			g->maxblock = ft_max(g->maxblock, ft_strlen(list->blocks));
		g->maxnam = ft_max(g->maxnam, list->namlen);
		list->number = g->countlist;
		g->countlist++;
		list->global = g;
		list = list->next;
	}
	g->totaltxt = ft_ltoa(g->total);
	return (g);
}

/*
**	удаление структуры c длинами имен
*/

void	ft_sort_list_del(t_plist *g)
{
	if (g)
	{
		free(g->printl);
		free(g->totaltxt);
	}
	free(g);
}

/*
**	Вывод на печать списка по столбикам
*/

void	ft_print_list_names_by_column(t_plist *global)
{
	int col;
	int row;
	int flag_s;

	col = 0;
	row = 0;
	flag_s = ft_gflags('s', 0);
	while (row < global->last_row)
	{
		col = 0;
		while ((global->printl)[col])
		{
			print_list_to_column((global->printl)[col], col, flag_s);
			(global->printl)[col] = ((global->printl)[col])->next;
			col++;
		}
		if (col + 1 < global->column)
			break ;
		row++;
	}
}

/*
**	получение указателей на начала столбцов списка для печати
*/

void	ft_sort_lists_names_by_column(t_dlist *list)
{
	int row;
	int col;

	col = 1;
	(list->global->printl)[0] = list;
	list->global->last_row = list->global->countlist / list->global->column;
	if (list->global->countlist % list->global->column)
		list->global->last_row++;
	while (list && col < list->global->column)
	{
		row = 0;
		while (list && list->next && row < list->global->last_row)
		{
			list = list->next;
			row++;
		}
		if (row == list->global->last_row)
			(list->global->printl)[col] = list;
		col++;
	}
}

/*
**	определение количества столбцов списка для печати из ширины экрана,
**	ширины самого широкого наименования и количества элементов
*/

int		ft_get_count_of_column(t_plist *width)
{
	struct winsize	w;
	int				colcount;
	int				lastcolcount;
	int				rows;
	int				colwidth;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	width->maxblock = (width->maxblock == 0) ? -1 : width->maxblock;
	colwidth = width->maxnam + width->maxblock + 1;
	colwidth = colwidth + (8 - colwidth % 8);
	width->maxnam = colwidth - width->maxblock - 1;
	colcount = w.ws_col / colwidth;
	if (isatty(1) && width->countlist <= colcount)
		return (width->countlist);
	if (!(isatty(1)) || colcount < 1)
		return (1);
	while (colcount > 1)
	{
		lastcolcount = width->countlist % (colcount - 1);
		rows = width->countlist / (colcount - 1);
		if (rows > lastcolcount)
			break ;
		colcount--;
	}
	return (colcount);
}
