/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_fd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 14:58:17 by jcalon            #+#    #+#             */
/*   Updated: 2022/06/01 17:04:22 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	get_here_doc(t_pipe *pipex, char **argv)
{
	int		fdtmp;
	char	*line;

	fdtmp = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fdtmp == -1)
		ft_error(pipex, errmsg("ERROR HERE_DOC :", ": ", strerror(errno)));
	line = "";
	while (1)
	{
		ft_putstr_fd("here_doc > ", 1);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break ;
		if (ft_strlen(argv[2]) + 1 == ft_strlen(line)
			&& !ft_strncmp(line, argv[2], ft_strlen(argv[2] + 1)))
		{
			free(line);
			break ;
		}
		else
			ft_putstr_fd(line, fdtmp);
		free(line);
	}
	close(fdtmp);
}

void	get_in_out_files(t_pipe *pipex, int ac, char **argv)
{
	if (pipex->here_doc == 1)
	{
		get_here_doc(pipex, argv);
		pipex->fdin = open(".heredoc.tmp", O_RDONLY);
		if (pipex->fdin == -1)
			ft_error(pipex, errmsg("ERROR HERE_DOC :", "", strerror(errno)));
		pipex->fdout = open(argv[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (pipex->fdout == -1)
			ft_error(pipex, errmsg(strerror(errno), ": ", argv[ac - 1]));
	}
	else
	{
		pipex->fdin = open(argv[1], O_RDONLY);
		if (pipex->fdin == -1)
			errmsg(strerror(errno), ": ", argv[1]);
		pipex->fdout = open(argv[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipex->fdout == -1)
			errmsg(strerror(errno), ": ", argv[ac - 1]);
	}
}
