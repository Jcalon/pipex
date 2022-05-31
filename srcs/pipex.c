/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:49:23 by jcalon            #+#    #+#             */
/*   Updated: 2022/05/31 15:25:09 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

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

static t_pipe	data_init(int argc, char **argv, char **envp)
{
	t_pipe	pipex;

	pipex.paths = find_paths(envp);
	if (!pipex.paths)
		ft_perror(&pipex, "ERROR : No paths");
	pipex.cmdpath = NULL;
	pipex.cmd = NULL;
	if (!ft_strncmp("here_doc", argv[1], 9))
		pipex.here_doc = 1;
	else
		pipex.here_doc = 0;
	pipex.cmds = argc - 3 - pipex.here_doc;
	pipex.bouts = calloc(2 * (pipex.cmds - 1), sizeof(int));
	if (pipex.bouts == NULL)
		ft_perror(&pipex, "PIPE ERROR");
	pipex.pids = calloc(pipex.cmds, sizeof(pid_t));
	if (pipex.pids == NULL)
		ft_perror(&pipex, "PID ERROR");
	return (pipex);
}

void	ft_pipex(t_pipe *pipex, char **argv, char **envp)
{
	int	i;

	i = 0;
	while (i < pipex->cmds - 1)
	{
		if (pipe(pipex->bouts + 2 * i) == -1)
			ft_perror(pipex, "PIPE ERROR");
		i++;
	}
	if (dup2(pipex->fdin, STDIN_FILENO) == -1)
		ft_perror(pipex, "ERROR_DUP_INFILE");
	if (dup2(pipex->fdout, STDOUT_FILENO) == -1)
		ft_perror(pipex, "ERROR_DUP_OUTFILE");
	i = 0;
	while (i < pipex->cmds)
	{
		pipex->pids[i] = fork();
		if (pipex->pids[i] == 0)
			child(pipex, argv, envp, i);
		i++;
	}
	waitpid(-1, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipe	pipex;

	if (argc < 5)
	{
		if (argc >= 2 && !ft_strncmp("here_doc", argv[1], 9))
		{
			ft_putstr_fd("ERR : ./pipex here_doc LIMITER cmd1 ... cmdn file2.",
				1);
			return (1);
		}
		ft_putstr_fd("ERR : ./pipex file1 cmd1 ... cmdn file2.", 1);
		return (1);
	}
	pipex = data_init(argc, argv, envp);
	get_in_out_files(&pipex, argc, argv);
	ft_pipex(&pipex, argv, envp);
	close_files(&pipex);
	ft_clean(&pipex);
	return (0);
}
