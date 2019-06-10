/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 20:59:39 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/19 13:13:30 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H
# include <dirent.h>
# include <sys/stat.h>
# include <pwd.h>
# include <uuid/uuid.h>
# include <grp.h>
# include "time.h"
# include <sys/xattr.h>
# include <sys/ioctl.h>
# include "libft.h"
# include "ft_ls_struct.h"

/*
**	for functions
*/

# define DEFAULT_PATH "."
# define BUF_LINK_SIZE 1000
# define MSG_ERROR "ls: *: No such file or directory\n"

/*
**	for functions
*/

# define SUCCESS 0
# define FAIL 1
# define ERROR -1

/*
**	actions with lists
*/

# define INSERT_LISTS 1
# define ADD_LIST 0
# define NOTHING 0
# define DEL_LIST -1

/*
**	for functions with stats of dir entry
*/

# define NOT_DIR NULL
# define FILE 1
# define DIRECT 0

/*
**	считывающие директории и управляющие функции
**	ft_ls.c
*/

DIR		**ft_return_dirp_and_print_files(int argc, char **argv);
int		ft_read_entry_from_path(t_dlist **begin_list, struct dirent *dp,
			char *name, char *path);
int		ft_read_and_print_dir(char *path_of_dir, DIR *dirp);
void	ft_recursive_call(t_dlist *list);

/*
**	функции получающие и возвращающие флаги
**	flags_and_errors.c
*/

int		ft_gflags(char flag, int write);
void	ft_find_flags(int *argc, char ***argv);
void	print_and_del_error_list(t_dlist **error_list);

/*
**	функции для подготовки к выводу на печать
**	parametrs_for_printing.c
*/

t_plist	*ft_find_max_strlens_in_lists(t_dlist *list);
void	ft_sort_list_del(t_plist *g);
int		ft_get_count_of_column(t_plist *width);
void	ft_sort_lists_names_by_column(t_dlist *list);
void	ft_print_list_names_by_column(t_plist *global);

/*
**	функции вывода на печать
**	printing.c
*/
void	ft_print_lists(t_dlist *list, int dir);
void	print_list_to_column(t_dlist *list, int col, int flag_s);
void	print_list_flag_l(t_dlist *list);
void	ft_pseudoprintf(char *mask, char *str, char minus, int spaces);
int		ft_symb_utf8_count(const char *str);

/*
**	функции для создания списков
**	lists.c
*/

t_dlist	*ft_dlistnew(char *name, char *path);
void	ft_dlistdel(t_dlist **begin_list);

/*
**	функции для сортировки списков
**	lists2.c
*/

void	ft_place_dlist(t_dlist **begin_list, t_dlist *new);
int		ft_check_suitable_place(t_dlist *list, t_dlist *new);
void	ft_list_swap(t_dlist *list, t_dlist *new, int action,
			t_dlist **begin_list);
t_dlist	*ft_create_sort_list(t_dlist **begin_list, char *name, char *path,
			struct stat *stats);
t_dlist	*ft_listrevers(t_dlist *begin_list);

/*
**	функции, возвращающие параметрами дирректорий и файлов
**	return_stats.c
*/

int		ft_return_stat(struct stat **stats, char *path, struct dirent *dp);
void	ft_stats_to_list(t_dlist *list, struct stat *stats);

/*
**	функции, возвращающие параметры stat в приемлемом виде
**	parametrs_of_stats.c
*/

char	*ft_readlink(char *path);
char	*ft_ctime(time_t *abs_clock);
char	*ft_return_mode(short st_mode);
void	ft_type_of_file(short st_mode, char *answer);
char	*ft_size_to_str(struct stat *stats, char *mode);

#endif
