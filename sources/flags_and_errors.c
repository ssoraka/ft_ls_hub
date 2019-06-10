/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags_and_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:53:02 by ssoraka           #+#    #+#             */
/*   Updated: 2019/06/10 17:53:02 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
**	Функция для установки и снятия глобальных флагов
*/

int		ft_gflags(char flag, int write)
{
	static char	flags[127];
	int			i;

	i = (int)flag;
	if (i < 0 || i > 127)
		return (0);
	if (write > 0)
		flags[i] = 1;
	if (write < 0)
		flags[i] = 0;
	if (flag == 'o' || flag == 'g')
		flags['l'] = 1;
	if (flag == '.')
	{
		ft_putstr_fd("ls: illegal option -- .\nusage: ls [", 2);
		ft_putstr_fd("-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]\n", 2);
		exit(0);
	}
	return (flags[i]);
}

void ft_error(void)
{
	ft_putstr_fd("ls: illegal option -- -\nusage: ls [", 2);
	ft_putstr_fd("-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]\n", 2);
	exit(0);
}

/*
**	считывание всех флагов и перемещение по аргументам
*/

void	ft_find_flags(int *argc, char ***argv)
{
	int i;

	(*argv)++;
	while (*argc > 1)
	{
		i = 0;
		(*argc)--;
		if ((**argv)[i] == '-' && (**argv)[i + 1] != '\0')
		{
			while ((**argv)[i])
			{
			//	if (((**argv)[i] == '-' && (**argv)[i + 1] == '-'))
			//		ft_error() ;
				ft_gflags((**argv)[i], 1);
				i++;
			}
		}
		else
			break ;
		(*argv)++;
	}
	if (*argc == 1 && (**argv) == NULL)
	{
		(*argv)--;
		ft_strcpy(**argv, DEFAULT_PATH);
	}
}

/*
**	выводим на печать и удаляем список ошибочных названий
*/

void	print_and_del_error_list(t_dlist **error_list)
{
	t_dlist *list;

	list = *error_list;
	while (list)
	{
		ft_pseudoprintf(MSG_ERROR, list->name, 0, 0);
		list = list->next;
	}
	ft_dlistdel(error_list);
}
