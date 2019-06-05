/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 11:13:07 by ssoraka           #+#    #+#             */
/*   Updated: 2019/05/27 11:13:07 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <uuid/uuid.h>
#include <grp.h>
#include "libft.h"
#include "time.h"
#include <errno.h>
#include <sys/xattr.h>
#include <sys/ioctl.h>


#define SUCCESS 0
#define FAIL 1
#define ERROR -1
#define MAX_NAME_SIZE 255
#define INSERT_LISTS 1
#define ADD_LIST 0
#define NOTHING 0
#define DEL_LIST -1
#define BUF_LINK_SIZE 1000
#define COLUMN 6

#define TEST1

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

int		ft_max(int first, int second)
{
	if (second > first)
		return (second);
	return (first);
}


t_plist		*ft_return_width(t_dlist *list)
{
	t_plist *tmp;

	if ((tmp = (t_plist *)ft_memalloc(sizeof(t_plist))) == 0)
		return (0);
	while(list)
	{
		if (ft_gflags('l', 0) || ft_gflags('s', 0))
			tmp->total = tmp->total + list->stats->st_blocks;
		if (ft_gflags('l', 0))
		{
			tmp->maxnlink = ft_max(tmp->maxnlink, ft_strlen(list->n_link));
			tmp->maxuser = ft_max(tmp->maxuser, ft_strlen(list->user1->pw_name));
			tmp->maxsize = ft_max(tmp->maxsize, ft_strlen(list->group1->gr_name));
			tmp->maxsize = ft_max(tmp->maxsize, ft_strlen(list->size));
		}
		if (ft_gflags('s', 0))
			tmp->maxblock = ft_max(tmp->maxblock, ft_strlen(list->blocks));
		tmp->maxnam = ft_max(tmp->maxnam, list->namlen);
		list->number = tmp->countlist;
		tmp->countlist++;
		list->global = tmp;
		list = list->next;
	}
	return (tmp);
}

/*
**	не нужно, хотя хз
*/
void	ft_listrevers(t_dlist **begin_list)
{
	t_dlist *list;
	struct s_dir_list	*temp;

	if (!begin_list || !(*begin_list))
		return ;
	while((*begin_list)->next)
	{
		list = *begin_list;
	//	list->number = list->global->countlist - list->number - 1;
		*begin_list = (*begin_list)->next;
		temp = list->next;
		list->next = list->parent;
		list->parent = temp;
	}
	list = *begin_list;
	//list->number = list->global->countlist - list->number - 1;
	temp = list->next;
	list->next = list->parent;
	list->parent = temp;
}



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

void    print_list_name(t_dlist *list, int col, int row, int flag_s)
{
	int count;

	if (flag_s)
		//printf("%*s ", list->global->maxblock, list->blocks);
		ft_pseudoprintf("* ", list->blocks, 0, list->global->maxblock);
	count = (list->global->last_row - 1) * list->global->column + col + 1;
	//printf("%d_%d_", list->number + list->global->last_row, list->global->countlist);
	if (col + 1 == list->global->column)
		//printf("%s", list->name);
		ft_putstr(list->name);
	else if (col + 2 == list->global->column &&
		list->number + list->global->last_row >= list->global->countlist)
		//printf("%s", list->name);
		ft_putstr(list->name);
	else
		//printf("%-*s", list->global->maxnam + 2, list->name);
		//ft_pseudoprintf(0, list->name, list->namlen - (list->global->maxnam + 2) , 0);
		ft_pseudoprintf("", list->name, '-', list->global->maxnam);
}


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
			print_list_name((global->printl)[col], col, row, flag_s);
			(global->printl)[col] = ((global->printl)[col])->next;
			col++;
		}
		//ft_putstr("\n");
		printf("\n");
		if (col + 1 < global->column)
			break ;
		row++;
	}
}

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
		(list->global->printl)[col] = list;
		col++;
	}
}



void    print_list_l2(t_dlist *list)
{
	if (!list)
		return ;
	char namebuf[1000];
	ft_bzero(namebuf, 1000);
	//listxattr(list->path, NULL, 1000, 0);
	//printf("%s", namebuf);

	if (ft_gflags('s', 0))
		printf("%*s ", list->global->maxblock, list->blocks);
	printf("%s", list->mode);

//	printf("%u", list->stats->st_flags);
//	getxattr(ft_strsub(list->path,0, ft_strlen(list->path) - ft_strlen(list->name)) - 1, list->name, (void *)namebuf, 1000, 0, XATTR_SHOWCOMPRESSION);
//		printf("%s", namebuf);
	//if (getxattr(ft_strsub(list->path,0, ft_strlen(list->path) - ft_strlen(list->name)) - 1, list->name, (void *)namebuf, 100, 100, 0) > 0)
	//	printf("%s", namebuf);


	//acl = acl_from_text(namebuf);
	//acl_create_entry(&acl, entry_p);


	if (listxattr(list->path, NULL, 0, 0) > 0)
		printf("@");
	else
		printf(" ");
	printf(" %.18s",ft_bytes_to_bits((char *)(&(list->stats->st_mode)), 2, "__"));
	printf("%*s", list->global->maxnlink + 1, list->n_link);
	printf(" %-*s", list->global->maxuser + 2, list->user1->pw_name);
	printf("%-*s", list->global->maxgroup + 0, list->group1->gr_name);
	printf("%*s", list->global->maxsize + 2, list->size);
	printf(" %s", list->mtime);
	printf(" %s", list->name);
	if (list->link)
		printf("%s\n", list->link);
	else
		printf("\n");
		//write(1, "\n", 2);
	print_list_l2(list->next);
}

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
	if (width->countlist < colcount)
		return (width->countlist);
	if (colcount < 1)
		return (1);
	while (colcount > 1)
	{
		lastcolcount = width->countlist % (colcount - 1);
		rows = width->countlist / (colcount - 1);
		//printf("%d_%d_%d_%d\n", colcount, width->countlist / (colcount - 1), width->countlist, lastcolcount);
		if (rows > lastcolcount)
			break ;
		colcount--;
	}
	//width->maxnam = width->maxnam + 4;//w.ws_col / (colcount + 1);
	//printf ("lines %d\n", w.ws_row);
	//printf ("columns %d\n", w.ws_col);
	return (colcount);
//	         untitled folder 7

}






void    print_list(t_dlist *list)
{
	t_plist *width;
	int column;

	width = ft_return_width(list);
	if (ft_gflags('l', 0))
	{
		if (width->countlist > 0)
			printf("total %lld\n", width->total);
		//print_list_l(list);
		print_list_l2(list);
	}
	else if (list)
	{
		column = ft_get_count_of_column(width);
		width->column = column;
		width->printl = (t_dlist **)ft_memalloc(sizeof(t_dlist) * column);
		if (width->printl == 0)
			return ;
//		if (ft_gflags('r', 0))
//			ft_listrevers(&list);
		if (ft_gflags('s', 0))
			printf("total %lld\n", width->total);
		ft_sort_lists_names_by_column(list);
		ft_print_list_names_by_column(width);
	}
	if (width && list)
		free(width->printl);
	free(width);
}

/*
void	ft_prepare_to_print(t_dlist **list)
{
	t_plist *width;

	width = ft_return_width(*list);
	if (ft_gflags('r', 0))
		ft_listrevers(list);
}
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

char	*ft_ctime(time_t *abs_clock)
{
	char	*answer;
	char	*str;

	str = ctime(abs_clock);
	answer = ft_strsub(str, 4, 12);

	return (answer);
}

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


void	ft_return_stats(t_dlist *list, int flag_l)
{
	struct stat *stats;

	if ((stats = (struct stat *)malloc(sizeof(struct stat))) == 0)
		return ;

	//ft_strcpy(str, dp->d_name);
	lstat(list->path, stats);
	list->stats = stats;
	//////получение количества блоков для TOTAL
	if (flag_l || ft_gflags('s', 0))
		list->blocks = ft_ltoa(list->stats->st_blocks);
	//////получение разрешений в виде строки
	if (flag_l)
		list->mode = ft_return_mode(list->stats->st_mode, list->link);
	//////получение размера файла в виде строки
	if (flag_l || (ft_gflags('S', 0)))
		list->size = ft_ltoa(list->stats->st_size);
	//////получение количство ссылок в виде строки
	if (flag_l)
		list->n_link = ft_ultoa(list->stats->st_nlink);
}

t_dlist	*ft_dlistnew(char *name, size_t namlen, char *path, int flag_l)
{
	t_dlist	*tmp;
	char	*s;
	struct passwd *user;
	struct group *group;

	if ((tmp = (t_dlist *)ft_memalloc(sizeof(t_dlist))) == 0)
		return (0);
	tmp->next = NULL;
	tmp->parent = NULL;
	//ft_strcpy(tmp->name, name);
	tmp->name = ft_strdup(name);
	tmp->namlen = namlen;
	//tmp->path = ft_strdup(path);
	tmp->path = path;
	//if (ft_gflags('l', 0))

	//////получение ссылки, если она есть
	tmp->link = ft_readlink(path);

	ft_return_stats(tmp, flag_l);

	if (flag_l)
	{
		//////получение юзера
		tmp->user1 = getpwuid(tmp->stats->st_uid);
		//tmp->user = ft_strdup(user->pw_name);
		//////получение группы
		tmp->group1 = getgrgid(tmp->stats->st_gid);
	}
	if (flag_l || (ft_gflags('t', 0)))
		//tmp->group = ft_strdup(group->gr_name);
		//////получение времени
		tmp->mtime = ft_ctime(&(tmp->stats->st_mtime));

	return (tmp);
}

void		ft_dlistdel(t_dlist **begin_list)
{
	t_dlist		*prev_list;

	if (begin_list == 0)
		return ;
	while (*begin_list != NULL)
	{
		prev_list = *begin_list;
		*begin_list = (*begin_list)->next;
		prev_list->next = 0;
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

int		ft_find_suitable_place(t_dlist *list, t_dlist *new)
{
	int		abc;

	abc = 0;
	if (ft_gflags('S', 0))//по весу
	{
		if (list && list->stats->st_size < new->stats->st_size)
			return (INSERT_LISTS);
		else if (list && list->stats->st_size == new->stats->st_size)
			abc = 1;
	}
	else if (ft_gflags('t', 0))//по дате
	{
		if (list && list->stats->st_mtime < new->stats->st_mtime)
			return (INSERT_LISTS);
		else if (list && list->stats->st_mtime == new->stats->st_mtime)
			abc = 1;
	}
	else
		abc = 1;
	if (abc && list && ft_strcmp(list->name, new->name) >= 0)//по алфавиту, вроде норм...
		return (INSERT_LISTS);
	return (ADD_LIST);
}

int		ft_check_and_del_notvalid_list(t_dlist **check_list)
{
	if (ft_gflags('a', 0) == 0)
		if (((*check_list)->name)[0] == '.')
		{
			ft_dlistdel(check_list);
			return (DEL_LIST);
		}
	return (ADD_LIST);
}


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

void	ft_place_dlist(t_dlist **begin_list, t_dlist *new)
{
	t_dlist	*check;
	int		action;

	action = NOTHING;
	if (ft_check_and_del_notvalid_list(&new) == DEL_LIST)
		return ;
	if (*begin_list == 0)
	{
		*begin_list = new;
		return ;
	}
	check = *begin_list;
	while (check)
	{
		if ((action = ft_find_suitable_place(check, new)) != NOTHING)
			break ;
		if (check->next == NULL)
			break ;
		check = check->next;
	}
	ft_list_swap(check, new, action, begin_list);
}



t_dlist 	*ft_read_dir(DIR *dirp, char *path)
{
	struct dirent *dp;
	t_dlist *begin_list;
	t_dlist *new_list;
	int flag_l;
	char *str;

	begin_list = NULL;
	flag_l = ft_gflags('l', 0);
	while ((dp = readdir(dirp)) != NULL)
	{
		str = ft_strjoin(path, dp->d_name);
		new_list = ft_dlistnew(dp->d_name, dp->d_namlen, str, flag_l);
		ft_place_dlist(&begin_list, new_list);
	}
	return (begin_list);
}

int 	ft_open_and_print_dir(char *path, DIR *dirp);

void ft_recursive_call(t_dlist *list)
{
	DIR *dirp;

	while (list)
	{
		if (ft_strcmp(".", list->name) != 0 && ft_strcmp("..", list->name) != 0 && list->link == NULL)
	//	if ((list->name)[0] != '.' && list->link == NULL)
			if ((dirp = opendir(list->path)))
			{
				ft_putstr("\n");
				ft_putendl(list->path);
				ft_open_and_print_dir(list->path, dirp);
				(void)closedir(dirp);
			}
		list = list->next;
	}
}



int 	ft_open_and_print_dir(char *path, DIR *dirp)
{
	t_dlist *begin_list;
	char *str;

	path = ft_strjoin(path, "/");
	begin_list = ft_read_dir(dirp, path);

	ft_memdel((void **)(&path));

	if (ft_gflags('r', 0))
		ft_listrevers(&begin_list);
	print_list(begin_list);
	if (ft_gflags('R', 0))
		ft_recursive_call(begin_list);
	ft_dlistdel(&begin_list);
	return (SUCCESS);
}


void ft_find_flags(int *argc, char ***argv)
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
	int i;

	i = 0;
	dirp = (DIR **)ft_memalloc(sizeof(*dirp) * (argc + 1));
	while (argc > 0)
	{
		dirp[i] = opendir(*argv);
		if (!dirp[i])
		{
			//////////////////
			//попробовать открыть как файл и все вывести, если не проканало, то все оставляем, как есть
			/////////////////
			ft_putstr("ls: ");
			ft_putstr(*argv);
			ft_putstr(": No such file or directory\n");
		}
		i++;
		argv++;
		argc--;
	}
	return (dirp);
}

int		main(int argc, char **argv)
{

	//char *path = "/Users/ssoraka/Desktop";
	//char *path = ".";
	//char *path = "/Users/ssoraka/Library";
	//char *path = "/Users/ssoraka/Library/Containers/com.apple.AirPlayUIAgent/Data/Desktop/days/cheacker/test/tests";
	//char *path = "/Users/ssoraka/Library/Calendars/DCBFBA8D-214F-434C-9CF5-4E6A352A3077.calendar/Events";
//	char *path = "/Users/ssoraka/Desktop/days/Ls";


	ft_find_flags(&argc, &argv);

	if (argc == 0)
	{
		argc++;
		argv--;
		(*argv)[0] = '.';
		(*argv)[1] = '\0';
	}

	DIR **dirp1;
	int i;
	dirp1 = ft_return_dirp(argc, argv);

	//int n = argc;
	i = 0;
	while (i < argc)
	{
		if (dirp1[i])
		{
			if (argc > 1)
				printf("\n%s:\n", argv[i]);
			ft_open_and_print_dir(argv[i], dirp1[i]);
			(void)closedir(dirp1[i]);
		}
		i++;
		free(dirp1[i]);
	}
	free(dirp1);


/*
**	|drwxr-xr-x	|@+			| 53 		|ssoraka	|  2019 	|1802	| May 28 15:50 	|Library	|-> /Volumes/Storage/goinfre/ssoraka/
**	|st_mode	|getxattr	|st_nlink	|getpwuid	|getgrgid	|st_size|	st_mtime	|d_name		|readlink
*/






	return (0);
}
