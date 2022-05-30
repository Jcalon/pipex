/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:49:23 by jcalon            #+#    #+#             */
/*   Updated: 2022/05/30 15:04:06 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static t_pipe	data_init(void)
{
	t_pipe	pipex;

	pipex.paths = NULL;
	pipex.cmdpath = NULL;
	pipex.bouts[0] = -1;
	pipex.bouts[1] = -1;
	pipex.pid1 = -1;
	pipex.pid2 = -1;
	pipex.fdin = -1;
	pipex.fdout = -1;
	pipex.cmd1 = NULL;
	pipex.cmd2 = NULL;
	return (pipex);
}

static char	**find_paths(char **envp)
{
	char	**paths;

	while (*envp && !ft_strnstr(*envp, "PATH", 4))
		envp++;
	paths = ft_split(*envp + 5, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipe	pipex;

	pipex = data_init();
	if (argc != 5)
		ft_error("Error: wrong count of arguments\n");
	if (pipe(pipex.bouts) == -1)
		ft_error("ERROR\n");
	pipex.paths = find_paths(envp);
	if (!pipex.paths)
		ft_error("ERROR : No paths\n");
	pipex.pid1 = fork();
	if (pipex.pid1 == 0)
		child1(pipex, argv, envp);
	pipex.pid2 = fork();
	if (pipex.pid2 == 0)
		child2(pipex, argv, envp);
	close(pipex.bouts[0]);
	close(pipex.bouts[1]);
	waitpid(pipex.pid1, NULL, 0);
	waitpid(pipex.pid2, NULL, 0);
	niel(pipex.paths);
	return (0);
}
