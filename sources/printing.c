/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:23:37 by ssoraka           #+#    #+#             */
/*   Updated: 2019/06/10 17:23:38 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
**	основная функция для вывода на экран
*/

void	ft_print_lists(t_dlist *list, int dir)
{
	t_plist	*width;
	int		column;

	width = ft_find_max_strlens_in_lists(list);
	if (ft_gflags('l', 0))
	{
		if (width->countlist > 0 && ft_gflags('d', 0) == 0 && dir == DIRECT)
			ft_pseudoprintf("total *\n", width->totaltxt, 0, 0);
		print_list_flag_l(list);
	}
	else if (list)
	{
		column = ft_get_count_of_column(width);
		width->column = column;
		width->printl = (t_dlist **)ft_memalloc(sizeof(t_dlist) * column);
		if (width->printl == 0)
			return ;
		if (ft_gflags('s', 0) && ft_gflags('d', 0) == 0)
			ft_pseudoprintf("total *\n", width->totaltxt, 0, 0);
		ft_sort_lists_names_by_column(list);
		ft_print_list_names_by_column(width);
	}
	ft_sort_list_del(width);
}

/*
**	Вывод на печать одного элемента по столбикам
*/

void	print_list_to_column(t_dlist *list, int col, int flag_s)
{
	int count;

	if (flag_s)
		ft_pseudoprintf("* ", list->blocks, 0, list->global->maxblock);
	count = (list->global->last_row - 1) * list->global->column + col + 1;
	if (col + 1 == list->global->column)
		ft_putendl(list->name);
	else if (col + 2 == list->global->column &&
		list->number + list->global->last_row >= list->global->countlist)
		ft_putendl(list->name);
	else
		ft_pseudoprintf("", list->name, '-', list->global->maxnam);
}

/*
**	Вывод на печать одной строки одного элемента списка с флагом l
*/

void	print_list_flag_l(t_dlist *list)
{
	if (!list)
		return ;
	if (ft_gflags('s', 0))
		ft_pseudoprintf("* ", list->blocks, 0, list->global->maxblock);
	ft_putstr(list->mode);
	if ((list->link && listxattr(list->path, NULL, 0, XATTR_NOFOLLOW) > 0) ||
		(listxattr(list->path, NULL, 0, 0) > 0))
		ft_putstr("@");
	else
		ft_putstr(" ");
	ft_pseudoprintf("", list->n_link, 0, list->global->maxnlink + 1);
	if (ft_gflags('g', 0) == 0)
		ft_pseudoprintf(" *", list->user, '-', list->global->maxuser);
	if (ft_gflags('o', 0) == 0 && ft_gflags('g', 0) == 0)
		ft_putstr(" ");
	if (ft_gflags('o', 0) == 0)
		ft_pseudoprintf(" *", list->group, '-', list->global->maxgroup);
	ft_pseudoprintf("", list->size, 0, list->global->maxsize + 2);
	ft_pseudoprintf(" *", list->mtime, 0, 0);
	ft_pseudoprintf(" *", list->name, 0, 0);
	if (list->link)
		ft_putendl(list->link);
	else
		ft_putstr("\n");
	print_list_flag_l(list->next);
}

/*
**	облегченная версия принтф
*/

void	ft_pseudoprintf(char *mask, char *str, char minus, int spaces)
{
	while (mask && *mask && *mask != '*')
	{
		ft_putchar(*mask);
		mask++;
	}
	if (minus == '-')
		ft_putstr(str);
	spaces = spaces - ft_symb_utf8_count(str);
	while (spaces > 0)
	{
		ft_putchar(' ');
		spaces--;
	}
	if (minus != '-')
		ft_putstr(str);
	if (*mask == '*')
		mask++;
	while (mask && *mask)
	{
		ft_putchar(*mask);
		mask++;
	}
}

int		ft_symb_utf8_count(const char *str)
{
	int		i;
	int		n;
	int		utf8;

	i = 0;
	n = 0;
	while (str[i])
	{
		if ((unsigned char)str[i] >= 240)
			utf8 = 4;
		else if ((unsigned char)str[i] >= 224)
			utf8 = 3;
		else if ((unsigned char)str[i] >= 192)
			utf8 = 2;
		else
			utf8 = 1;
		i = i + utf8;
		n++;
	}
	return (n);
}
