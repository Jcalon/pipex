/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 12:56:18 by jcalon            #+#    #+#             */
/*   Updated: 2022/05/30 14:16:45 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static char	*get_cmd(char **paths, char *cmd)
{
	size_t	i;
	char	*tmp;
	char	*cmdpath;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		cmdpath = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(cmdpath, F_OK | X_OK) == 0)
			return (cmdpath);
		free(cmdpath);
		i++;
	}
	return (NULL);
}

void	child1(t_pipe pipex, char **argv, char **envp)
{
	pipex.fdin = open(argv[1], O_RDONLY);
	if (pipex.fdin == -1)
		ft_perror(pipex, "ERROR_INFILE", 0, 0);
	if (dup2(pipex.fdin, STDIN_FILENO) == -1)
		ft_perror(pipex, "ERROR_DUP_INFILE", 0, 0);
	if (dup2(pipex.bouts[1], STDOUT_FILENO) == -1)
		ft_perror(pipex, "ERROR_DUP_OUTPIPE", 0, 0);
	close(pipex.bouts[0]);
	close(pipex.bouts[1]);
	close(pipex.fdin);
	if (argv[2][0] == '\0')
		argv[2] = "cat";
	pipex.cmd1 = ft_split(argv[2], ' ');
	pipex.cmdpath = get_cmd(pipex.paths, pipex.cmd1[0]);
	if (pipex.cmdpath == NULL)
		ft_perror(pipex, "command not found", 0, 1);
	if (execve(pipex.cmdpath, pipex.cmd1, envp) == -1)
		ft_perror(pipex, "ERROR", 1, 1);
	niel(pipex.cmd1);
	free(pipex.cmdpath);
}

void	child2(t_pipe pipex, char **argv, char **envp)
{
	pipex.fdout = open(argv[4], O_WRONLY | O_TRUNC, 0644);
	if (pipex.fdout == -1)
		ft_perror(pipex, "ERROR_OUTFILE", 0, 0);
	if (dup2(pipex.fdout, STDOUT_FILENO) == -1)
		ft_perror(pipex, "ERROR_DUP_OUTFILE", 0, 0);
	if (dup2(pipex.bouts[0], STDIN_FILENO) == -1)
		ft_perror(pipex, "ERROR_DUP_INTPIPE", 0, 0);
	close(pipex.bouts[0]);
	close(pipex.bouts[1]);
	close(pipex.fdout);
	if (argv[3][0] == '\0')
		argv[3] = "cat";
	pipex.cmd2 = ft_split(argv[3], ' ');
	pipex.cmdpath = get_cmd(pipex.paths, pipex.cmd2[0]);
	if (pipex.cmdpath == NULL)
		ft_perror(pipex, "command not found", 0, 2);
	if (execve(pipex.cmdpath, pipex.cmd2, envp) == -1)
		ft_perror(pipex, "ERROR", 1, 2);
	niel(pipex.cmd2);
	free(pipex.cmdpath);
}
