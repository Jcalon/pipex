/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 12:55:37 by jcalon            #+#    #+#             */
/*   Updated: 2022/05/30 14:17:55 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	niel(char	**split)
{
	size_t	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	ft_perror(t_pipe pipex, char *str, int freepath, int freecmd)
{
	if (freepath == 1)
		free(pipex.cmdpath);
	if (freecmd == 1)
		niel(pipex.cmd1);
	else if (freecmd == 2)
		niel(pipex.cmd2);
	niel(pipex.paths);
	perror(str);
	exit(EXIT_FAILURE);
}

void	ft_error(char *str)
{
	ft_putstr_fd(str, 1);
	exit(EXIT_FAILURE);
}
