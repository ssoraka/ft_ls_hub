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


#define SUCCESS 0
#define FAIL 1
#define ERROR -1
#define MAX_NAME_SIZE 205
#define INSERT_LISTS 1
#define ADD_LIST 0
#define NOTHING 0
#define DEL_LIST -1
#define BUF_LINK_SIZE 1000
#define COLUMN 1

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
	char				name[MAX_NAME_SIZE + 1];
	char				*path;
	char				*link;
	char				*user;
	char				*group;
	char				*mtime;
	char				*mode;
	char				*size;
	char				*n_link;
	int					namlen;
	int					blocklen;
	int					userlen;
	int					grouplen;
	int					sizelen;
	int					linkslen;
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

t_plist		*ft_return_width(t_dlist *list)
{
	t_plist *tmp;

	if ((tmp = (t_plist *)ft_memalloc(sizeof(t_plist))) == 0)
		return (0);
	while(list)
	{
		tmp->total = tmp->total + list->stats->st_blocks;
		if (tmp->maxnlink < (list->linkslen = ft_strlen(list->n_link)))
			tmp->maxnlink = list->linkslen;
		#ifndef TEST
		if (tmp->maxuser < (list->userlen = ft_strlen(list->user1->pw_name)))
			tmp->maxuser = list->userlen;
		if (tmp->maxgroup < (list->grouplen = ft_strlen(list->group1->gr_name)))
			tmp->maxgroup = list->grouplen;
		#endif
		if (tmp->maxsize < (list->sizelen = ft_strlen(list->size)))//вес
			tmp->maxsize = list->sizelen;
		if (tmp->maxblock < (list->blocklen = ft_strlen(list->blocks)))//количество блоков
			tmp->maxblock = list->blocklen;
		if (tmp->maxnam < list->namlen)
			tmp->maxnam = list->namlen;
		list->number = tmp->countlist;
		tmp->countlist++;//количество
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

/*
void    print_list_test(t_dlist *list)
{
	t_plist *width;

	width = ft_return_width(list);

	printf("total %lld\n", width->total);
	while(list)
	{

		//printf("%s___%s",list->user, list->group);
		//printf("%s___%s___%s___",list->mtime, list->user, list->group);

		//printf("___%lld___%s___%s\n",list->blocks, list->name, list->mode);
		if (ft_gflags('s', 0))
			printf("%*s ", width[5], list->blocks);
		printf("%s", list->mode);
		printf("%*s", width[1] + 1, list->n_link);
		printf(" %-*s", width[2] + 2, list->user1->pw_name);
		printf("%-*s", width[3] + 0, list->group1->gr_name);
		printf("%*s", width[4] + 2, list->size);
		printf(" %s", list->mtime);
		printf(" %s", list->name);
		printf("%s\n", list->link);


		//printf("%s\n",list->mtime);
		//struct passwd *getpwuid(uid_t uid);
		//pas->pw_name
		//printf("%s\n",ctime(&(list->stats->st_mtime)));
		//printf("%s\n",ctime(time(&(list->stats->st_mtime))));
		//char *ctime(const time_t *clock);
		//time_t ;
		//printf("%s___%s___\n",list->link, list->name);
		//readlink(list->path, buf, 10000);
		//printf("%s\n", buf);
		//ft_bzero(buf, 10000);
		//printf("%s",ft_bytes_to_bits((char *)(&(list->stats->st_mode)), 2, "__"));
		list = list->next;
	}
}*/

void    print_list_names(t_dlist *list, int *width)
{
	if (!list)
		return ;
	if (ft_gflags('r', 0))
		print_list_names(list->next, width);
//	width[9]++;
	if (!(list->parent) && ft_gflags('r', 0))
		printf("%s\n", list->name);
//	else if (width[9] % width[8] == 0)
//		printf("%s\n", list->name);
	else if (list->next)
		printf("%-*s", width[6] + 2, list->name);
	else if (!ft_gflags('r', 0))
		printf("%s\n", list->name);
	else
		printf("%-*s", width[6] + 2, list->name);
	if (!ft_gflags('r', 0))
		print_list_names(list->next, width);
}


void    print_list_name(t_dlist *list, int col, int row)
{
	int count;

	/////if (ft_gflags('s', 0))
	/////	printf("%*s ", list->global->maxblock, list->blocks);
	count = (list->global->last_row - 1) * list->global->column + col + 1;
	//printf("%d_%d_", list->number + list->global->last_row, list->global->countlist);
	if (col + 1 == list->global->column)
		write(1, list->name, list->namlen);
		//printf("%s", list->name);
	else if (col + 2 == list->global->column &&
		list->number + list->global->last_row >= list->global->countlist)
		printf("%s", list->name);
	else
		printf("%-*s", list->global->maxnam + 2, list->name);
}


void	ft_print_list_names_by_column(t_plist *global)
{
	int col;
	int row;

	col = 0;
	row = 0;
//	global->last_row = global->countlist / global->column;
//	if (global->countlist % global->column)
//		global->last_row++;
	while (row < global->last_row)
	{
		col = 0;
		while ((global->printl)[col])
		{
			print_list_name((global->printl)[col], col, row);
			(global->printl)[col] = ((global->printl)[col])->next;
			col++;
		}
		write(1, "\n", 1);
		//printf("\n");
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


void    print_list_l(t_dlist *list)
{
	if (!list)
		return ;
	if (ft_gflags('r', 0))
		print_list_l(list->next);
	if (ft_gflags('s', 0))
		printf("%*s ", list->global->maxblock, list->blocks);
	printf("%s", list->mode);
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
	if (!ft_gflags('r', 0))
		print_list_l(list->next);
}


void    print_list_l2(t_dlist *list)
{
	if (!list)
		return ;
	if (ft_gflags('s', 0))
		printf("%*s ", list->global->maxblock, list->blocks);
	printf("%s", list->mode);
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


void    print_list(t_dlist *list)
{
	t_plist *width;
	int column;

	width = ft_return_width(list);
	//if (list)
	//	width = list->global;
	if (ft_gflags('l', 0))
	{
		if (width->countlist > 0)
			printf("total %lld\n", width->total);
		//print_list_l(list);
		print_list_l2(list);
	}
	else if (list)
	{
		column = COLUMN;//ft_get_count_of_column(width);
		width->column = column;
		width->printl = (t_dlist **)ft_memalloc(sizeof(t_dlist) * column);
		if (width->printl == 0)
			return ;
//		if (ft_gflags('r', 0))
//			ft_listrevers(&list);
		ft_sort_lists_names_by_column(list);
		ft_print_list_names_by_column(width);
	}
	if (width && list)
		free(width->printl);
	free(width);
		//print_list_names(list, width);
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
		//return (ft_strdup(""));
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
	//str = "123123123123123123123";
	answer = ft_strsub(str, 4, 12);

	//str = ctime(abs_clock);
	//str = ctime(abs_clock);
	//str = ctime(abs_clock);
	//str = ctime(abs_clock);
	//ft_memmove(str, str + 4, 12);
	//str[12] = '\0';
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
	if ((st_mode & 0x8000) == 0)
		answer[0] = 'd';
	if (link && link[0] != '\0')
		answer[0] = 'l';
	return (answer);
}



t_dlist	*ft_dlistnew(char *name, size_t namlen, char *path, struct stat *stats)
{
	t_dlist	*tmp;
	char	*s;
	struct passwd *user;
	struct group *group;

	if ((tmp = (t_dlist *)ft_memalloc(sizeof(t_dlist))) == 0)
		return (0);
	//tmp->next = NULL;
	//tmp->parent = NULL;
	tmp->namlen = namlen;
	//tmp->path = ft_strdup(path);
	tmp->path = path;
	ft_strcpy(tmp->name, name);
	tmp->stats = stats;
	if (ft_gflags('l', 0))
	{
		#ifndef TEST
		//////получение ссылки, если она есть
		tmp->link = ft_readlink(path);

		//////получение юзера
		tmp->user1 = getpwuid(tmp->stats->st_uid);
		//tmp->user = ft_strdup(user->pw_name);
		//////получение группы
		tmp->group1 = getgrgid(tmp->stats->st_gid);

		//tmp->group = ft_strdup(group->gr_name);
		//////получение времени
		tmp->mtime = ft_ctime(&(tmp->stats->st_mtime));
		#endif
	}

	if (stats)
	{
		//////получение количества блоков для TOTAL
		tmp->blocks = ft_ltoa(tmp->stats->st_blocks);
		//////получение разрешений в виде строки
		tmp->mode = ft_return_mode(tmp->stats->st_mode, tmp->link);
		//////получение размера файла в виде строки
		tmp->size = ft_ltoa(tmp->stats->st_size);
		//////получение количство ссылок в виде строки
		tmp->n_link = ft_ultoa(tmp->stats->st_nlink);
	}
	return (tmp);
}

void		ft_dlistdel(t_dlist **begin_list)
{
	t_dlist		*prev_list;

	if (begin_list == 0)
		return ;
	//free((*begin_list)->global->printl);
	//free((*begin_list)->global);
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
		free(prev_list);
	}
}

int		ft_find_suitable_place(t_dlist *list, t_dlist *new)
{
	int		abc;

	//if ((new->name)[0] == '.')
	//	return (DEL_LIST);
	//по размеру
	abc = 0;
	return (INSERT_LISTS);
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
//	if (ft_check_and_del_notvalid_list(&new) == DEL_LIST)
//		return ;
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
	struct stat *stats;
	//struct stat stats[sizeof(struct stat)];
	char *str;

	begin_list = NULL;
	//str = path + ft_strlen(path);
	while ((dp = readdir(dirp)) != NULL)
	{
		stats = 0;
		//ft_strcpy(str, dp->d_name);
		str = ft_strjoin(path, dp->d_name);
		if (ft_gflags('l', 0))
		{
			if ((stats = (struct stat *)malloc(sizeof(struct stat))) == 0)
				return (0);
			lstat(str, stats);
		}
		//str[-1] = '\0';
		new_list = ft_dlistnew(dp->d_name, dp->d_namlen, str, stats);
		ft_place_dlist(&begin_list, new_list);
	}
	//print_list(begin_list);
	return (begin_list);
}

int 	ft_open_and_print_dir(char *path, DIR *dirp);

void ft_recursive_call(t_dlist *list)
{
	DIR *dirp;

	while (list)
	{
		if (ft_strcmp(".", list->name) != 0 && ft_strcmp("..", list->name) != 0)
			if ((dirp = opendir(list->path)))
			{
				//printf("\n%s:\n", list->path);
//				write(1, "\n", 1);
//				write(1, list->path, ft_strlen(list->path));
//				write(1, "\n", 1);
				ft_open_and_print_dir(list->path, dirp);
				(void)closedir(dirp);
	//	if (list->next && dir == SUCCESS)
			}
		list = list->next;
	}
}



int 	ft_open_and_print_dir(char *path, DIR *dirp)
{
//	DIR *dirp;
	t_dlist *begin_list;
	char *str;

//	if ((dirp = opendir(path)) == NULL)
//		return (ERROR);
//	str = ft_strnew(ft_strlen(path) + MAX_NAME_SIZE + 1);
//	path = ft_strcpy(str, path);
//	ft_strcat(path, "/");
	path = ft_strjoin(path, "/");
	begin_list = ft_read_dir(dirp, path);

//	(void)closedir(dirp);
	ft_memdel((void **)(&path));
//	if (ft_gflags('r', 0))
//		ft_listrevers(&begin_list);
//	ft_prepare_to_print(&begin_list);
	if (ft_gflags('r', 0))
		ft_listrevers(&begin_list);
	//print_list(begin_list);
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



	//ft_ls_parser(argv);

/*	int argc1 = 6;
	char *argv1[] = {
		"name",
		"alrR",
		"l",
		"-R",
		".",
		"-S"
	};
*/

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


		//path = argv[1];

	/*
		ft_gflags('a', 1);
		ft_gflags('S', 0);
		ft_gflags('s', 0);
		ft_gflags('t', 0);
		ft_gflags('r', 0);
		ft_gflags('l', 1);
		ft_gflags('R', 1);
	*/

/*
	printf("%s  -   %d\n", argv1[1], ft_gflags('a', 0));
	printf("%s  -   %d\n", argv1[2], ft_gflags('l', 0));
	printf("%s  -   %d\n", argv1[3], ft_gflags('R', 0));
	printf("%s  -   %d\n", argv1[4], ft_gflags('s', 0));
	printf("%s  -   %d\n", argv1[5], ft_gflags('S', 0));
	printf("%s  -   %d\n", argv1[5], ft_gflags('r', 0));
*/

//
//


/*
**	|drwxr-xr-x	|@+			| 53 		|ssoraka	|  2019 	|1802	| May 28 15:50 	|Library	|-> /Volumes/Storage/goinfre/ssoraka/
**	|st_mode	|getxattr	|st_nlink	|getpwuid	|getgrgid	|st_size|	st_mtime	|d_name		|readlink
*/

/*
readlink(list->path, buf, 10000);
printf("%s\n", buf);
ft_bzero(buf, 10000);
*/
	//char *errorbuf = strerror(errno);
	//fprintf(stderr, "Error message : %s\n", errorbuf);
	//printf("Error message : %s\n", errorbuf);
	//perror("const char *s");

/*
	DIR *dirp;

	if ((dirp = opendir(path)))
		ft_open_and_print_dir(path, dirp);
	else
	{
		ft_putstr("ls: ");
		ft_putstr(path);
		ft_putstr(": No such file or directory\n");
	}

	if (dirp)
		(void)closedir(dirp);
*/



		//return (0);
	//perror("const char *s");


/*
	int i;
	i = 0;
	float ld=0.98775;

	while (i<10)
	{
		printf("%.*f\n", 4, ld);
		printf("%.*f\n\n", 50, ld);
		ld = ld + 1;
		i++;
	}
*/



//	printf("%.100e\n", 234564165.0);

//	dirp = opendir(path);
//	if (dirp == NULL)
//		return (1);
//	len = strlen(name);
/*
	while ((dp = readdir(dirp)) != NULL)
	{
		if ((stats = (struct stat *)ft_memalloc(sizeof(struct stat))) == 0)
			return (0);
		str = ft_strjoin(path, dp->d_name);
		stat(str, stats);

		new_list = ft_dlistnew(dp->d_name, dp->d_namlen, path, stats);
		ft_place_dlist(&begin_list, new_list);
*/
		//stat(dp->d_name, &buff);
		//printf("%s __ %ld\n", dp->d_name, buff.st_birthtime);
		//free(buff);


		//printf("%d __ %s __ %lld\n", i, dp->d_name, buff.st_size);
		/*
		if (dp->d_namlen == len && strcmp(dp->d_name, name) == 0)
		{
				(void)closedir(dirp);
				printf("%s\n", "123");
				return (0);
		}*/
//	}
//	(void)closedir(dirp);



	return (0);
}
