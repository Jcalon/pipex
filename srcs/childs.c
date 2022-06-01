/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 12:56:18 by jcalon            #+#    #+#             */
/*   Updated: 2022/06/01 14:35:50 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static char	*get_cmd(char **paths, char *cmd)
{
	size_t	i;
	char	*tmp;
	char	*cmdpath;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (errinfo("Unexpected error", ""));
		cmdpath = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!cmdpath)
			return (errinfo("Unexpected error", ""));
		if (access(cmdpath, X_OK) == 0)
			return (cmdpath);
		free(cmdpath);
	}
	return (NULL);
}

int	parent(t_pipe *pipex, int i)
{
	pid_t	wpid;
	int		status;
	int		exit_status;	

	exit_status = 1;
	close_files(pipex);
	i--;
	while (i >= 0)
	{
		wpid = waitpid(pipex->pids[i], &status, 0);
		if (wpid == pipex->pids[pipex->cmds - 1])
		{
			if ((i == (pipex->cmds - 1)) && WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
		}
		i--;
	}
	ft_clean(pipex);
	return (exit_status);
}

static void	exec_child(t_pipe *pipex, char **argv, char **envp, int i)
{
	if (argv[i][0] == '\0')
		argv[i] = "cat";
	pipex->cmd = ft_split(argv[i], ' ');
	if (!pipex->cmd)
		ft_error(pipex, errmsg("Unexpected error", "", ""));
	if (access(pipex->cmd[0], F_OK | X_OK) == 0)
		pipex->cmdpath = pipex->cmd[0];
	else
		pipex->cmdpath = get_cmd(pipex->paths, pipex->cmd[0]);
	if (pipex->cmdpath == NULL)
	{
		if (access(pipex->cmd[0], F_OK) == 0
			&& (access(pipex->cmd[0], X_OK) != 0))
			ft_error(pipex, errmsg(strerror(errno), ": ", pipex->cmd[0]));
		ft_error(pipex, cmderr("command not found", ": ", pipex->cmd[0]));
	}
	if (execve(pipex->cmdpath, pipex->cmd, envp) == -1)
		ft_error(pipex, errmsg(pipex->cmd[0], ": ", strerror(errno)));
	niel(pipex->cmd);
	free(pipex->cmdpath);
}

void	child(t_pipe *pipex, char **argv, char **envp, int i)
{
	if (i == 0)
	{
		if (dup2(pipex->fdin, STDIN_FILENO) == -1)
			ft_error(pipex, 1);
		if (dup2(pipex->bouts[1], STDOUT_FILENO) == -1)
			ft_error(pipex, 1);
	}
	else if (i == pipex->cmds - 1)
	{
		if (dup2(pipex->bouts[i * 2 - 2], STDIN_FILENO) == -1)
			ft_error(pipex, 1);
		if (dup2(pipex->fdout, STDOUT_FILENO) == -1)
			ft_error(pipex, 1);
	}
	else
	{
		if (dup2(pipex->bouts[2 * i - 2], STDIN_FILENO) == -1)
			ft_error(pipex, 1);
		if (dup2(pipex->bouts[2 * i + 1], STDOUT_FILENO) == -1)
			ft_error(pipex, 1);
	}
	close_files(pipex);
	exec_child(pipex, argv, envp, i + 2 + pipex->here_doc);
}
