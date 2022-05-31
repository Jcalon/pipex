/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 12:56:18 by jcalon            #+#    #+#             */
/*   Updated: 2022/05/31 14:58:31 by jcalon           ###   ########.fr       */
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

static void	exec_child(t_pipe *pipex, char **argv, char **envp, int i)
{
	if (argv[i][0] == '\0')
		argv[i] = "cat";
	pipex->cmd = ft_split(argv[i], ' ');
	pipex->cmdpath = get_cmd(pipex->paths, pipex->cmd[0]);
	if (pipex->cmdpath == NULL)
		ft_perror(pipex, "command not found");
	if (execve(pipex->cmdpath, pipex->cmd, envp) == -1)
		ft_perror(pipex, "ERROR");
	niel(pipex->cmd);
	free(pipex->cmdpath);
}

void	child(t_pipe *pipex, char **argv, char **envp, int i)
{
	if (i == 0)
	{
		if (dup2(pipex->fdin, STDIN_FILENO) == -1)
			ft_perror(pipex, "ERROR_DUP");
		if (dup2(pipex->bouts[1], STDOUT_FILENO) == -1)
			ft_perror(pipex, "ERROR_DUP");
	}
	else if (i == pipex->cmds - 1)
	{
		if (dup2(pipex->bouts[i * 2 - 2], STDIN_FILENO) == -1)
			ft_perror(pipex, "ERROR_DUP");
		if (dup2(pipex->fdout, STDOUT_FILENO) == -1)
			ft_perror(pipex, "ERROR_DUP");
	}
	else
	{
		if (dup2(pipex->bouts[2 * i - 2], STDIN_FILENO) == -1)
			ft_perror(pipex, "ERROR_DUP");
		if (dup2(pipex->bouts[2 * i + 1], STDOUT_FILENO) == -1)
			ft_perror(pipex, "ERROR_DUP");
	}
	close_files(pipex);
	exec_child(pipex, argv, envp, i + 2 + pipex->here_doc);
}
