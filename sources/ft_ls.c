/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 11:03:47 by ssoraka           #+#    #+#             */
/*   Updated: 2019/06/07 11:03:48 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
**	сам ls...
*/

int		main(int argc, char **argv)
{
	DIR	**dirp;
	int	i;

	ft_find_flags(&argc, &argv);
	dirp = ft_return_dirp_and_print_files(argc, argv);
	i = 0;
	while (i <= argc)
	{
		if (dirp[i])
		{
			if (ft_gflags('d', 0) == 0)
			{
				if (argc > 1 && i > 0)
					ft_putstr("\n");
				if (argc > 1)
					ft_pseudoprintf("*:\n", argv[i], 0, 0);
				ft_read_and_print_dir(argv[i], dirp[i]);
			}
			(void)closedir(dirp[i]);
		}
		i++;
	}
	free(dirp);
	return (0);
}

/*
**	формирование списка открытых дирректорий;
**	создание, вывод и удаление списка файлов;
**	вывод ошибок
*/

DIR		**ft_return_dirp_and_print_files(int argc, char **argv)
{
	DIR		**dirp;
	int		i;
	t_dlist	*begin;
	t_dlist	*errors;

	i = 0;
	begin = NULL;
	errors = NULL;
	dirp = (DIR **)ft_memalloc(sizeof(*dirp) * (argc + 1));
	while (argc > 0)
	{
		dirp[i] = opendir(argv[i]);
		if (!dirp[i] || ft_gflags('d', 0))
			if (ft_read_entry_from_path(&begin, NOT_DIR, argv[i], argv[i]) ==
				FAIL)
				ft_create_sort_list(&errors, argv[i], NULL, NULL);
		i++;
		argc--;
	}
	print_and_del_error_list(&errors);
	if (ft_gflags('r', 0))
		begin = ft_listrevers(begin);
	ft_print_lists(begin, FILE);
	ft_dlistdel(&begin);
	return (dirp);
}

/*
**	считывание файла или папки по текущему пути
**	и выстраивание списка из прочитанных
*/

int		ft_read_entry_from_path(t_dlist **begin_list, struct dirent *dp,
			char *name, char *path)
{
	t_dlist		*new_list;
	struct stat	*stats;
	int			namlen;

	if (dp == NOT_DIR)
	{
		path = ft_strdup(path);
		namlen = ft_strlen(name);
	}
	else
	{
		path = ft_strjoin(path, name);
		namlen = dp->d_namlen;
	}
	if (ft_return_stat(&stats, path, dp) == SUCCESS)
	{
		new_list = ft_create_sort_list(begin_list, name, path, stats);
		new_list->namlen = namlen;
		return (SUCCESS);
	}
	free(path);
	return (FAIL);
}

/*
**	считывание открытой дирректории с выводом списка и его одалением
*/

int		ft_read_and_print_dir(char *path_of_dir, DIR *dirp)
{
	struct dirent	*dp;
	t_dlist			*begin_list;

	begin_list = NULL;
	path_of_dir = ft_strjoin(path_of_dir, "/");
	while ((dp = readdir(dirp)) != NULL)
		ft_read_entry_from_path(&begin_list, dp, dp->d_name, path_of_dir);
	ft_memdel((void **)(&path_of_dir));
	if (ft_gflags('r', 0))
		begin_list = ft_listrevers(begin_list);
	ft_print_lists(begin_list, DIRECT);
	if (ft_gflags('R', 0))
		ft_recursive_call(begin_list);
	ft_dlistdel(&begin_list);
	return (SUCCESS);
}

/*
**	рекурсивное открытие дирректорий, формирование списков, вывод на печать,
**	удаление
*/

void	ft_recursive_call(t_dlist *list)
{
	DIR *dirp;

	while (list)
	{
		if (ft_strcmp(".", list->name) != 0 &&
			ft_strcmp("..", list->name) != 0 && list->link == NULL)
			if ((dirp = opendir(list->path)))
			{
				ft_pseudoprintf("\n*:\n", list->path, 0, 0);
				ft_read_and_print_dir(list->path, dirp);
				(void)closedir(dirp);
			}
		list = list->next;
	}
}
