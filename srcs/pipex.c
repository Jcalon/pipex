/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:49:23 by jcalon            #+#    #+#             */
/*   Updated: 2022/06/01 17:04:28 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**find_paths(char **envp)
{
	char	**paths;

	while (*envp && !ft_strnstr(*envp, "PATH", 4))
		envp++;
	if (!*envp)
		return (NULL);
	paths = ft_split(*envp + 5, ':');
	if (!paths)
		ft_error(NULL, errmsg("Unexpected error", "", ""));
	return (paths);
}

static t_pipe	data_init(int argc, char **argv, char **envp)
{
	t_pipe	pipex;

	pipex.paths = find_paths(envp);
	pipex.cmdpath = NULL;
	pipex.cmd = NULL;
	if (!ft_strncmp("here_doc", argv[1], 9))
		pipex.here_doc = 1;
	else
		pipex.here_doc = 0;
	get_in_out_files(&pipex, argc, argv);
	pipex.cmds = argc - 3 - pipex.here_doc;
	pipex.bouts = ft_calloc(2 * (pipex.cmds - 1), sizeof(int));
	if (pipex.bouts == NULL)
		ft_error(&pipex, errmsg("PIPE ERROR", "", ""));
	pipex.pids = ft_calloc(pipex.cmds, sizeof(pid_t));
	if (pipex.pids == NULL)
		ft_error(&pipex, errmsg("PID ERROR", strerror(errno), ""));
	return (pipex);
}

static int	ft_pipex(t_pipe *pipex, char **argv, char **envp)
{
	int	i;
	int	exit_status;

	i = 0;
	while (i < pipex->cmds - 1)
	{
		if (pipe(pipex->bouts + 2 * i) == -1)
			ft_error(pipex, errmsg("PIPE CREATION ERROR", "", ""));
		i++;
	}
	i = 0;
	while (i < pipex->cmds)
	{
		pipex->pids[i] = fork();
		if (pipex->pids[i] == -1)
			ft_error(pipex, errmsg("Fork", ": ", strerror(errno)));
		if (pipex->pids[i] == 0)
			child(pipex, argv, envp, i);
		i++;
	}
	exit_status = parent(pipex, i);
	return (exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipe	pipex;
	int		exit_status;

	if (argc < 5)
	{
		if (argc >= 2 && !ft_strncmp("here_doc", argv[1], 9))
			return (errmsg("Write :",
					"! ./pipex here_doc LIMITER cmd1 ... cmdn file2",
					""));
		return (errmsg("Write :", "! ./pipex file1 cmd1 ... cmdn file2", ""));
	}
	else if (argc < 6 && !ft_strncmp("here_doc", argv[1], 9))
		return (errmsg("Write :",
				"! ./pipex here_doc LIMITER cmd1 ... cmdn file2", ""));
	pipex = data_init(argc, argv, envp);
	exit_status = ft_pipex(&pipex, argv, envp);
	return (exit_status);
}
