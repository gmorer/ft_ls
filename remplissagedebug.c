/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remplissage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/22 11:57:23 by gmorer            #+#    #+#             */
/*   Updated: 2016/04/18 11:59:28 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static char		ft_type(struct stat plop)
{
	if ((S_ISLNK(plop.st_mode)))
		return ('l');
	else if ((S_ISSOCK(plop.st_mode)))
		return ('s');
	else if ((S_ISCHR(plop.st_mode)))
		return ('c');
	else if ((S_ISBLK(plop.st_mode)))
		return ('b');
	else if ((S_ISFIFO(plop.st_mode)))
		return ('p');
	else if ((S_ISDIR(plop.st_mode)))
		return ('d');
	else if ((S_ISREG(plop.st_mode)))
		return ('-');
	return ('e');
}

static char		*permission(struct stat plop)
{
	char *rslt;

	rslt = ft_strnew(9);
	ft_memset(rslt, '-', 9);
	rslt[9] = '\0';
	if (plop.st_mode & S_IRUSR)
		rslt[0] = 'r';
	if (plop.st_mode & S_IWUSR)
		rslt[1] = 'w';
	if (plop.st_mode & S_IXUSR)
		rslt[2] = 'x';
	if (plop.st_mode & S_IRGRP)
		rslt[3] = 'r';
	if (plop.st_mode & S_IWGRP)
		rslt[4] = 'w';
	if (plop.st_mode & S_IXGRP)
		rslt[5] = 'x';
	if (plop.st_mode & S_IROTH)
		rslt[6] = 'r';
	if (plop.st_mode & S_IWOTH)
		rslt[7] = 'w';
	if (plop.st_mode & S_IXOTH)
		rslt[8] = 'x';
	return (rslt);
}

static char		*ft_timels(char *time)
{
	char	*rslt;
	int		i;

	i = 0;
	if((rslt = (char*)malloc(sizeof(char) * 13)) == 0)
		return (NULL);
	while (*time != ' ')
		time++;
	time++;
	while (i < 12)
	{
		rslt[i] = *time;
		i++;
		time++;
	}
	rslt[12] = '\0';
	return (rslt);
}

static t_file	*remplissage(t_file *rslt, struct stat plop, liste *list, char *name)
{
	rslt->content->name = name;
	if ((list->option_l) || (list->option_t))
	{
		name = ctime(&plop.st_mtime);
		rslt->content->date = ft_timels(ctime(&plop.st_mtime));
		rslt->content->time = plop.st_mtime;
	}
	if ((list->option_l) || (list->option_R))
		rslt->content->type = ft_type(plop);
	if ((list->option_l))
	{
		rslt->content->permission = permission(plop);
		rslt->content->linkno = (int)plop.st_nlink;
		rslt->content->groupuid = ft_gid(plop.st_gid);
		rslt->content->useruid = ft_uid(plop.st_uid);
		rslt->content->size = (int)plop.st_size;
	}
	rslt->next = NULL;
	rslt->previous = NULL;
	return (rslt);
}

t_file			*ft_newfile(char *argv, struct dirent *file, liste *list)
{
	t_file		*rslt;
	struct stat	plop;

	if (lstat(nxtfd(argv, file->d_name), &plop) == -1)
	{
		perror("error stat:");
		return (NULL);
	}
	if (!(rslt = (t_file*)malloc(sizeof(t_file))))
		return (NULL);
	if(!(rslt->content = (data*)malloc(sizeof(data))))
		return (NULL);
	if (!(file))
		return (NULL);
	rslt = remplissage(rslt, plop, list, file->d_name);
	if((list->option_l))
	{
		list->totalsize += rslt->content->size;
		if(ft_strlen(ft_itoa(rslt->content->linkno)) > list->maxlinklen)
			list->maxlinklen = ft_strlen(ft_itoa(rslt->content->linkno));
		if(ft_strlen(rslt->content->groupuid) > list->maxgidlen)
			list->maxgidlen = ft_strlen(rslt->content->groupuid);
		if(ft_strlen(rslt->content->useruid) > list->maxuidlen)
			list->maxuidlen = ft_strlen(rslt->content->useruid);
		if(ft_strlen(ft_itoa(rslt->content->size)) > list->maxsizelen)
			list->maxsizelen = ft_strlen(ft_itoa(rslt->content->size));
	}
		if((list->option_l && rslt->content->type == 'l'))
	{
		ft_memset(rslt->content->linkto, 0, sizeof(char));
		readlink(strcatturfu(argv, file->d_name), rslt->content->linkto, sizeof(rslt->content->linkto)-1);
	}
	return (rslt);
}
