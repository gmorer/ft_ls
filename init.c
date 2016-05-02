/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/30 10:24:34 by gmorer            #+#    #+#             */
/*   Updated: 2016/05/02 17:08:20 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char *nxtfd(char *str1, char *str2)
{
	char *rslt;
	int i;
	int x;

	x = 0;
	i = 0;
	rslt = ft_strnew(ft_strlen(str1) + ft_strlen(str2));
	while(str1[i])
	{
		rslt[i] = str1[i];
		i++;
	}
	if(str1[i - 1] != '/')
		rslt[i] = '/';
	if(str1[i - 1] != '/')
		i++;
	while(str2[x])
	{
		rslt[i] = str2[x];
		x++;
		i++;
	}
	rslt[i] = '\0';
	return (rslt);
}

static char *errorargv(char *str)
{
	if(str[ft_strlen(str) - 1] == '/')
		str[ft_strlen(str) - 1] = '\0';
	return(str);
}

static t_file *initfile(t_file *filetmp, char *argv, t_liste *list)
{
	struct dirent *file;

	if((list->actualdir = opendir(argv)) == NULL)
	{
		list->ok = 0;
		free(filetmp->content);
		free(filetmp);
		filetmp = NULL;
		//ft_putendl(ft_strjoin("\n", argv));
		perror(ft_strjoin("ft_ls: ", errorargv(argv)));
		return(filetmp);
	}
	list->ok = 1;
	list->initialpath = argv;
	while(filetmp && (file = readdir(list->actualdir)))
		if((list->option_a) || (!list->option_a && file->d_name[0] != '.'))
		{
			if((filetmp->content->name))
				filetmp->next = ft_newfile(argv, file, list);
			else
				filetmp = ft_newfile(argv, file, list);
			if((filetmp->next))
				filetmp->next->previous = filetmp;
			if((filetmp->next))
				filetmp = filetmp->next;
		}
	return (filetmp);
}



t_liste		*init(char *argv, t_liste *list)
{
	t_file	*filetmp;

	if((filetmp = (t_file*)malloc(sizeof(t_file))) == NULL)
		exit(-1);
	if((filetmp->content = (t_data*) malloc(sizeof(t_data))) == NULL)
		exit(-1);
	filetmp->previous = NULL;
	filetmp->next = NULL;
	filetmp->content->name = NULL;
	filetmp->content->type = 0;
	filetmp->content->permission = NULL;
	list->initialpath = NULL;
	filetmp = initfile(filetmp, argv, list);
	if(list->ok == 0)
	{
		return (list);
	}
	while((filetmp->previous))
		filetmp = filetmp->previous;
	list->first = filetmp;
	return (list);
}
