/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parametrs_of_stats.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:43:51 by ssoraka           #+#    #+#             */
/*   Updated: 2019/06/10 17:43:52 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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
	time_t	now_clock;

	str = ctime(abs_clock);
	now_clock = time(0);
	if (ft_gflags('T', 0))
		answer = ft_strsub(str, 4, ft_strlen(str + 4) - 1);
	else
	{
		answer = ft_strsub(str, 4, 12);
		if (now_clock < *abs_clock || now_clock - *abs_clock > 15724800)
			ft_memcpy(answer + 7, str + 19, 5);
	}
	return (answer);
}

/*
**	возвращает права доступа в виде строчки текста
*/

char	*ft_return_mode(short st_mode)
{
	char	*answer;
	int		i;
	short	mask;

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
	ft_type_of_file(st_mode, answer);
	return (answer);
}

void	ft_type_of_file(short st_mode, char *answer)
{
	if (S_ISBLK(st_mode))
		answer[0] = 'b';
	if (S_ISCHR(st_mode))
		answer[0] = 'c';
	if (S_ISDIR(st_mode))
		answer[0] = 'd';
	if (S_ISFIFO(st_mode))
		answer[0] = 'p';
	if (S_ISLNK(st_mode))
		answer[0] = 'l';
	if (S_ISSOCK(st_mode))
		answer[0] = 's';
}

/*
**	размер в виде строчки текста, для b и c возвращает номера портов
*/

char	*ft_size_to_str(struct stat *stats, char *mode)
{
	char	*major;
	char	*minor;
	char	*answer;

	if (mode[0] != 'c' && mode[0] != 'b')
		return (ft_ltoa(stats->st_size));
	answer = ft_strdup("   ,    ");
	major = ft_itoa(major(stats->st_rdev));
	minor = ft_itoa(minor(stats->st_rdev));
	ft_memcpy(answer + 3 - ft_strlen(major), major, ft_strlen(major));
	ft_memcpy(answer + 8 - ft_strlen(minor), minor, ft_strlen(minor));
	free(major);
	free(minor);
	return (answer);
}
