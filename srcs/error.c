/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 12:10:54 by jcalon            #+#    #+#             */
/*   Updated: 2022/06/01 13:56:54 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*errinfo(char *str1, char *str2)
{
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(str2, 2);
	return (NULL);
}

int	errmsg(char *str1, char *str2, char *str3)
{
	ft_putstr_fd("pipex :", 2);
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	ft_putendl_fd(str3, 2);
	return (1);
}

int	cmderr(char *str1, char *str2, char *str3)
{
	ft_putstr_fd("pipex : ", 2);
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	ft_putendl_fd(str3, 2);
	return (127);
}

void	ft_error(t_pipe *pipex, int erno)
{
	ft_clean(pipex);
	exit(erno);
}
