/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_stats.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:44:25 by ssoraka           #+#    #+#             */
/*   Updated: 2019/06/10 17:44:26 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
**	определяем, существует ли файл или директория, и возвращаем статы
*/

int		ft_return_stat(struct stat **stats, char *path, struct dirent *dp)
{
	int file;
	int flags;

	flags = 0;
	if (dp == NOT_DIR)
		flags = FILE;
	else if (dp && ft_gflags('a', 0) == 0 && (dp->d_name)[0] == '.')
		return (FAIL);
	*stats = NULL;
	if (flags != FILE)
		flags = ft_gflags('l', 0) + ft_gflags('s', 0) + ft_gflags('S', 0) +
		ft_gflags('t', 0);
	if (dp && flags == 0)
		return (SUCCESS);
	if ((*stats = (struct stat *)malloc(sizeof(struct stat))) == 0)
		return (FAIL);
	file = lstat(path, *stats);
	if (file == ERROR)
		free(*stats);
	return (file);
}

/*
**	возвращает параметры из stats элементу списка в зависимости от флагов
*/

void	ft_stats_to_list(t_dlist *list, struct stat *stats)
{
	int flag_l;

	list->stats = stats;
	if (stats == NULL)
		return ;
	flag_l = ft_gflags('l', 0);
	if (ft_gflags('s', 0))
		list->blocks = ft_ltoa(list->stats->st_blocks);
	if (flag_l)
	{
		list->mode = ft_return_mode(list->stats->st_mode);
		list->size = ft_size_to_str(list->stats, list->mode);
		list->n_link = ft_ultoa(list->stats->st_nlink);
		list->user = ft_strdup(getpwuid(list->stats->st_uid)->pw_name);
		list->group = ft_strdup(getgrgid(list->stats->st_gid)->gr_name);
	}
	if (ft_gflags('u', 0))
		list->time = list->stats->st_atime;
	else if (flag_l || ft_gflags('t', 0))
		list->time = list->stats->st_mtime;
	if (flag_l || ft_gflags('t', 0))
		list->mtime = ft_ctime(&(list->time));
}
