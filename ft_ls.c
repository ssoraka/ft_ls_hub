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

#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <uuid/uuid.h>
#include <grp.h>
#include "libft.h"
#include "time.h"
#include <sys/xattr.h>
#include <sys/ioctl.h>


#define SUCCESS 0
#define FAIL 1
#define ERROR -1
#define INSERT_LISTS 1
#define ADD_LIST 0
#define NOTHING 0
#define DEL_LIST -1
#define BUF_LINK_SIZE 1000

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
	char				*user;
	char				*group;
	char				*mtime;
	char				*mode;
	char				*size;
	char				*n_link;
	int					namlen;
	int					number;
	struct stat			*stats;
	struct s_dir_list	*parent;
	struct passwd		*user1;
	struct group		*group1;
	struct s_dir_list	*next;
	struct s_par_list	*global;
}						t_dlist;

/*
**	Функция для установки и снятия глобальных флагов
*/

int		ft_gflags(char flag, int write)
{
	static char flags[127];

	if (flag < 0 || flag > 127)
		return (0);
	if (write > 0)
		flags[flag] = 1;
	if (write < 0)
		flags[flag] = 0;
	return (flags[flag]);
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
	spaces = spaces - ft_strlen(str);
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

/*
**	понадобилось для вывода на печать по столбикам
*/
void	ft_listrevers(t_dlist **begin_list)
{
	t_dlist	*list;
	t_dlist	*temp;

	if (!begin_list || !(*begin_list))
		return ;
	while((*begin_list)->next)
	{
		list = *begin_list;
		*begin_list = (*begin_list)->next;
		temp = list->next;
		list->next = list->parent;
		list->parent = temp;
	}
	list = *begin_list;
	temp = list->next;
	list->next = list->parent;
	list->parent = temp;
}

/*
**	Вывод на печать одной строки одного элемента списка с флагом l
*/
void    print_list_flag_l(t_dlist *list)
{
	if (!list)
		return ;
	if (ft_gflags('s', 0))
		ft_pseudoprintf("* ", list->blocks, 0, list->global->maxblock);
	ft_putstr(list->mode);
	if (listxattr(list->path, NULL, 0, 0) > 0)
		ft_putstr("@");
	else
		ft_putstr(" ");
	ft_pseudoprintf("", list->n_link, 0, list->global->maxnlink + 1);
	ft_pseudoprintf(" *", list->user1->pw_name, '-', list->global->maxuser + 2);
	ft_pseudoprintf("", list->group1->gr_name, '-', list->global->maxgroup);
	ft_pseudoprintf("", list->size, '-', list->global->maxsize + 2);
	ft_pseudoprintf(" *", list->mtime, 0, 0);
	ft_pseudoprintf(" *", list->name, 0, 0);
	if (list->link)
		ft_putendl(list->link);
	else
		ft_putstr("\n");
	print_list_flag_l(list->next);
}

/*
**	Вывод на печать одного элемента по столбикам
*/
void    print_list_to_column(t_dlist *list, int col, int row, int flag_s)
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
			print_list_to_column((global->printl)[col], col, row, flag_s);
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
void    ft_sort_lists_names_by_column(t_dlist *list)
{
	int row;
	int col;

	col = 1;
	(list->global->printl)[0] = list;
	list->global->last_row = list->global->countlist / list->global->column;
	if (list->global->countlist % list->global->column)
		list->global->last_row++;
	while(list && col < list->global->column)
	{
		row = 0;
		while(list && list->next && row < list->global->last_row)
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
	int				plus;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	plus = 8 - width->maxnam % 8;
	width->maxnam = width->maxnam + plus;
	colcount = w.ws_col / (width->maxnam);
	if (width->countlist <= colcount)
		return (width->countlist);
	if (colcount < 1)
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

/*
**	возвращает ссылку, если она есть
*/
char	*ft_readlink(char *path)
{
	char	buff[BUF_LINK_SIZE + 1];
	size_t	read;
	char	*answer;

	buff[0] = '\0';
	if (readlink(path, NULL, 0) == -1)
		return (NULL);
	if ((read = readlink(path, buff, BUF_LINK_SIZE)) > 0)
		answer = ft_strdup(buff);
	answer = ft_strjoin_free(" -> ", answer, 0, 1);
	return (answer);
}

/*
**	возвращает время в виде строчки текста из системного
*/
char	*ft_ctime(time_t *abs_clock)
{
	char	*answer;
	char	*str;

	str = ctime(abs_clock);
	answer = ft_strsub(str, 4, 12);
	return (answer);
}

/*
**	возвращает права доступа в виде строчки текста
*/
char *ft_return_mode(short st_mode, char *link)
{
	char *answer;
	int i;
	short mask;

	i = 9;
	mask = 1;
	answer = ft_strdup("-rwxrwxrwx");
	while (i > 0)
	{
		if ((st_mode & mask) == 0)
			answer[i] = '-';
		mask = mask << 1;
		i--;
	}
	if ((st_mode & 0x0200) != 0)
		answer[9] = (answer[9] == '-') ? 'T' : 't';
	if ((st_mode & 0x0400) != 0)
		answer[6] = (answer[6] == '-') ? 'S' : 's';
	if ((st_mode & 0x0800) != 0)
		answer[3] = (answer[3] == '-') ? 'S' : 's';
	if ((st_mode & 0x4000) != 0)
		answer[0] = 'd';
	if (link && link[0] != '\0')
		answer[0] = 'l';
	return (answer);
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
	if (flag_l || ft_gflags('s', 0))
		list->blocks = ft_ltoa(list->stats->st_blocks);
	if (flag_l || ft_gflags('S', 0))
		list->size = ft_ltoa(list->stats->st_size);
	if (flag_l)
	{
		list->mode = ft_return_mode(list->stats->st_mode, list->link);
		list->n_link = ft_ultoa(list->stats->st_nlink);
		list->user1 = getpwuid(list->stats->st_uid);
		list->group1 = getgrgid(list->stats->st_gid);
	}
	if (flag_l || (ft_gflags('t', 0)))
		list->mtime = ft_ctime(&(list->stats->st_mtime));
}

/*
**	создаем новый элемент списка
*/
t_dlist	*ft_dlistnew(char *name, size_t namlen, char *path)
{
	t_dlist	*tmp;
	char	*s;
	struct passwd *user;
	struct group *group;

	if ((tmp = (t_dlist *)ft_memalloc(sizeof(t_dlist))) == 0)
		return (0);
	tmp->next = NULL;
	tmp->parent = NULL;
	tmp->name = ft_strdup(name);
	tmp->namlen = namlen;
	tmp->path = path;
	tmp->link = ft_readlink(path);
	return (tmp);
}

/*
** удаляем список
*/
void		ft_dlistdel(t_dlist **begin_list)
{
	t_dlist		*prev_list;

	if (begin_list == 0)
		return ;
	while (*begin_list != NULL)
	{
		prev_list = *begin_list;
		*begin_list = (*begin_list)->next;
		free(prev_list->stats);
		free(prev_list->path);
		//free(prev_list->user1);
		//free(prev_list->group1);
		free(prev_list->link);
		free(prev_list->mtime);
		free(prev_list->mode);
		free(prev_list->n_link);
		free(prev_list->blocks);
		free(prev_list->size);
		free(prev_list->name);
		free(prev_list);
	}
}

/*
**	определяем, существует ли файл или директория, и возвращаем статы
*/
int		ft_return_stat(struct stat **stats, char *path, int flags)
{
	int file;

	*stats = NULL;
	if (flags == 0)
		flags = ft_gflags('l', 0) + ft_gflags('s', 0) + ft_gflags('S', 0) +
		ft_gflags('t', 0);
	if (flags == 0)
		return (SUCCESS);
	if ((*stats = (struct stat *)malloc(sizeof(struct stat))) == 0)
		return (FAIL);
	file = lstat(path, *stats);
	if (file == FAIL)
		free(*stats);
	return (file);
}

























void	ft_find_flags(int *argc, char ***argv)
{
	int i;

	(*argc)--;
	(*argv)++;
	if (*argc == 0)
		return ;
	while (*argc > 0)
	{
		i = 0;
		if ((**argv)[i] == '-')
		{
			while ((**argv)[i])
			{
				ft_gflags((**argv)[i], 1);
				i++;
			}
		}
		else
			return ;
		(*argc)--;
		(*argv)++;
	}
}







DIR **ft_return_dirp(int argc, char **argv)
{
	DIR **dirp;
	struct stat *stats;
	int i;
	t_dlist *begin_list = NULL;
	t_dlist *new_list;

	i = 0;
	dirp = (DIR **)ft_memalloc(sizeof(*dirp) * (argc + 1));
	while (argc > 0)
	{
		dirp[i] = opendir(*argv);
		if (!dirp[i] || ft_gflags('d', 0) == 1)
		{
			if (ft_return_stat(&stats, *argv, 1) == SUCCESS)
			{
				new_list = ft_dlistnew(ft_strdup(*argv), ft_strlen(*argv), ft_strdup(*argv));
				ft_stats_to_list(new_list, stats);
				ft_place_dlist(&begin_list, new_list);
			}
			else
				ft_pseudoprintf("ls: *: No such file or directory\n", *argv, 0, 0);
		}
		i++;
		argv++;
		argc--;
	}
	if (ft_gflags('r', 0))
		ft_listrevers(&begin_list);
	print_list(begin_list);
	ft_dlistdel(&begin_list);
	return (dirp);
}









int		main(int argc, char **argv)
{
	DIR **dirp;
	int i;

	ft_find_flags(&argc, &argv);

	if (argc == 0)
	{
		argc++;
		argv--;
		(*argv)[0] = '.';
		(*argv)[1] = '\0';
	}
	//ft_change_flags();
	dirp = ft_return_dirp(argc, argv);

	i = 0;
	while (i < argc)
	{
		if (dirp[i])
		{
			if (ft_gflags('d', 0) == 0)
			{
				if (argc > 1)
					printf("\n%s:\n", argv[i]);
				ft_open_and_print_dir(argv[i], dirp[i]);
			}
			(void)closedir(dirp[i]);
		}
		i++;
	}
	free(dirp);
	return (0);
}
