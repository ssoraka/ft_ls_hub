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
		//ft_putchar_utf8((char *)str + i, utf8);
		//ft_putchar_utf8("\n", 2);
		i = i + utf8;
		n++;
	}
	return (n);
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

/*
**	понадобилось для вывода на печать по столбикам
*/

t_dlist	*ft_listrevers(t_dlist *begin_list)
{
	t_dlist	*list;
	t_dlist	*temp;

	if (!begin_list)
		return (NULL);
	while(begin_list->next)
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
	char buf[1000];
	buf[0] = '\0';
	listxattr(list->path, buf, 1000, 0);
	ft_putstr(buf);
	if (listxattr(list->path, NULL, 0, 0) > 0)
		ft_putstr("@");
	else
		ft_putstr(" ");
	ft_pseudoprintf("", list->n_link, 0, list->global->maxnlink + 1);
	ft_pseudoprintf(" *", list->user1->pw_name, '-', list->global->maxuser + 2);
	ft_pseudoprintf("", list->group1->gr_name, '-', list->global->maxgroup);
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
**	Вывод на печать одного элемента по столбикам
*/

void	print_list_to_column(t_dlist *list, int col, int row, int flag_s)
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

void	ft_sort_lists_names_by_column(t_dlist *list)
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
**	получение указателей на начала столбцов списка для печати
*/

t_plist		*ft_find_max_strlen_in_lists(t_dlist *list)
{
	t_plist *g;

	if ((g = (t_plist *)ft_memalloc(sizeof(t_plist))) == 0)
		return (0);
	while(list)
	{
		if (ft_gflags('l', 0) || ft_gflags('s', 0))
			g->total = g->total + list->stats->st_blocks;
		if (ft_gflags('l', 0))
		{
			g->maxnlink = ft_max(g->maxnlink, ft_strlen(list->n_link));
			g->maxuser = ft_max(g->maxuser, ft_strlen(list->user1->pw_name));
			g->maxgroup = ft_max(g->maxgroup, ft_strlen(list->group1->gr_name));
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


void	ft_sort_list_del(t_plist *width)
{
	if (width)
	{
		free(width->printl);
		free(width->totaltxt);
	}
	free(width);
}


/*
**	основная функция для вывода на экран
*/

void	ft_print_lists(t_dlist *list)
{
	t_plist *width;
	int column;

	if (ft_gflags('r', 0))
		list = ft_listrevers(list);
	width = ft_find_max_strlen_in_lists(list);
	if (ft_gflags('l', 0))
	{
		if (width->countlist > 0 && ft_gflags('d', 0) == 0)
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

	answer = NULL;
	buff[0] = '\0';
	if (path == NULL || readlink(path, NULL, 0) == -1)
		return (NULL);
	if ((read = readlink(path, buff, BUF_LINK_SIZE)) > 0)
	{
		buff[read] = '\0';
		answer = ft_strdup(buff);
		answer = ft_strjoin_free(" -> ", answer, 0, 1);
	}
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

char	*ft_return_mode(short st_mode, char *link)
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

t_dlist	*ft_dlistnew(char *name, char *path)
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
		free(prev_list);
	}
}

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
**	проверяем, стоит ли вставлять рассматриваемый элемент в данном месте списка
*/

int		ft_check_suitable_place(t_dlist *list, t_dlist *new)
{
	int		abc;

	abc = 0;
	if (list->stats && ft_gflags('S', 0))//по весу
	{
		if (list && list->stats->st_size < new->stats->st_size)
			return (INSERT_LISTS);
		else if (list && list->stats->st_size == new->stats->st_size)
			abc = 1;
	}
	else if (list->stats && ft_gflags('t', 0))//по дате
	{
		if (list && list->stats->st_mtime < new->stats->st_mtime)
			return (INSERT_LISTS);
		else if (list && list->stats->st_mtime == new->stats->st_mtime)
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

void	ft_list_swap(t_dlist *list, t_dlist *new, int action, t_dlist **begin_list)
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
	else//вставляем в начало списка
	{
		new->next = list;
		list->parent = new;
		*begin_list = new;
	}
}

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
**	собраем список ошибочных названий по алфавиту
*/

t_dlist	*ft_create_sort_list(t_dlist **begin_list, char *name, char *path, struct stat *stats)
{
	t_dlist		*new_list;

	new_list = ft_dlistnew(name, path);
	ft_stats_to_list(new_list, stats);
	ft_place_dlist(begin_list, new_list);
	return (new_list);
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

/*
**	считывание файла или папки по текущему пути
**	и выстраивание списка из прочитанных
*/

int		ft_read_entry_from_path(t_dlist **begin_list, struct dirent *dp, char *name, char *path)
{
	t_dlist *new_list;
	struct stat *stats;
	int namlen;

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
		//new_list = ft_dlistnew(name, path);
		//ft_stats_to_list(new_list, stats);
		//ft_place_dlist(begin_list, new_list);
		new_list->namlen = namlen;
		return (SUCCESS);
	}
	free(path);
	return (FAIL);
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

/*
**	считывание открытой дирректории с выводом списка и его одалением
*/

int		ft_read_and_print_dir(char *path_of_dir, DIR *dirp)
{
	struct dirent *dp;
	t_dlist *begin_list;
	char *str;

	begin_list = NULL;
	path_of_dir = ft_strjoin(path_of_dir, "/");
	while ((dp = readdir(dirp)) != NULL)
		ft_read_entry_from_path(&begin_list, dp, dp->d_name, path_of_dir);
	ft_memdel((void **)(&path_of_dir));
//	if (ft_gflags('r', 0))
//		ft_listrevers(&begin_list);
	ft_print_lists(begin_list);
	if (ft_gflags('R', 0))
		ft_recursive_call(begin_list);
	ft_dlistdel(&begin_list);
	return (SUCCESS);
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
		if ((**argv)[i] == '-')
		{
			while ((**argv)[i])
			{
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
**	формирование списка открытых дирректорий;
**	создание, вывод и удаление списка файлов;
**	вывод ошибок
*/

DIR		**ft_return_dirp_and_print_files(int argc, char **argv)
{
	DIR **dirp;
	int i;
	t_dlist *begin;
	t_dlist *errors;
	t_dlist *new_list;

	i = 0;
	begin = NULL;
	errors = NULL;
	dirp = (DIR **)ft_memalloc(sizeof(*dirp) * (argc + 1));
	while (argc > 0)
	{
		dirp[i] = opendir(argv[i]);
		if (!dirp[i] || ft_gflags('d', 0))
			if (ft_read_entry_from_path(&begin, NOT_DIR, argv[i], argv[i]) == FAIL)
				ft_create_sort_list(&errors, argv[i], NULL, NULL);
		i++;
		argc--;
	}
	print_and_del_error_list(&errors);
	ft_print_lists(begin);
	ft_dlistdel(&begin);
	return (dirp);
}

/*
**	сам ls...
*/
int		main(int argc, char **argv)
{
	DIR **dirp;
	int i;

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
		//argc--;
	}
	free(dirp);
	return (0);
}
