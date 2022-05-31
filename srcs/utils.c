/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 12:55:37 by jcalon            #+#    #+#             */
/*   Updated: 2022/05/31 21:18:57 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	close_files(t_pipe *pipex)
{
	int	j;

	j = 0;
	if (pipex->fdin != -1)
		close(pipex->fdin);
	if (pipex->fdout != -1)
		close(pipex->fdout);
	while (j < (pipex->cmds - 1) * 2)
	{
		close(pipex->bouts[j]);
		j++;
	}
	if (pipex->here_doc == 1)
		unlink(".heredoc.tmp");
}

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

void	ft_clean(t_pipe *pipex)
{
	if (pipex->paths)
		niel(pipex->paths);
	if (pipex->cmd)
		niel(pipex->cmd);
	if (pipex->bouts)
		free(pipex->bouts);
	if (pipex->pids)
		free(pipex->pids);
}

char	*errinfo(char *str1, char *str2)
{
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(str2, 2);
	return (NULL);
}

void	ft_error(t_pipe *pipex, char *str1, char *str2, int erno)
{
	ft_clean(pipex);
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	exit(erno);
}
