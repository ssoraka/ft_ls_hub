/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_struct.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 20:59:39 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/19 13:13:30 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_STRUCT_H
# define FT_LS_STRUCT_H

/*
**	общая структура, доступная из каждой структуры файла или папки,
**	хранящая параметры для вывода на печать
*/

typedef struct			s_par_list
{
	long long			total;
	int					maxnam;
	int					maxblock;
	int					maxuser;
	int					maxgroup;
	int					maxsize;
	int					maxnlink;
	int					countlist;
	int					column;
	int					last_row;
	char				*totaltxt;
	struct s_dir_list	**printl;
}						t_plist;

/*
**	структура для одного файла или папки
*/

typedef struct			s_dir_list
{
	char				*blocks;
	char				*name;
	char				*path;
	char				*link;
	char				*mtime;
	char				*mode;
	char				*size;
	char				*n_link;
	char				*user;
	char				*group;
	time_t				time;
	int					namlen;
	int					number;
	struct stat			*stats;
	struct s_dir_list	*parent;
	struct s_dir_list	*next;
	struct s_par_list	*global;
}						t_dlist;

#endif
