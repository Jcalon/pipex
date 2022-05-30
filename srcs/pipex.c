/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:49:23 by jcalon            #+#    #+#             */
/*   Updated: 2022/05/30 21:30:23 by jcalon           ###   ########.fr       */
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
	pipex.cmds = argc - 3;
	pipex.bouts = calloc(2 * (pipex.cmds - 1), sizeof(int));
	if (pipex.bouts == NULL)
		ft_perror(&pipex, "PIPE ERROR");
	pipex.pids = calloc(pipex.cmds, sizeof(pid_t));
	if (pipex.pids == NULL)
		ft_perror(&pipex, "PID ERROR");
	pipex.fdin = open(argv[1], O_RDONLY);
	if (pipex.fdin == -1)
		ft_perror(&pipex, "ERROR_INFILE");
	pipex.fdout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex.fdout == -1)
		ft_perror(&pipex, "ERROR_OUTFILE");
	return (pipex);
}

int	ft_pipex(t_pipe *pipex, char **argv, char **envp)
{
	int	i;
	int	exit_status;
	int	status;

	i = 0;
	exit_status = 1;
	while (i < pipex->cmds)
	{
		pipex->pids[i] = fork();
		if (pipex->pids[i] == 0)
			child(pipex, argv, envp, i);
		i++;
	}
	waitpid(-1, NULL, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	return (exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipe	pipex;
	int		i;
	int		exit_status;

	if (argc < 5)
	{
		ft_putstr_fd("Error, expected : ./pipex infile cmd1 cmd2 ... cmdn outfile\n", STDOUT_FILENO);
		return (1);
	}
	pipex = data_init(argc, argv, envp);
	i = 0;
	while (i < pipex.cmds - 1)
	{
		if (pipe(pipex.bouts + 2 * i) == -1)
			ft_perror(&pipex, "PIPE ERROR");
		i++;
	}
	if (dup2(pipex.fdin, STDIN_FILENO) == -1)
		ft_perror(&pipex, "ERROR_DUP_INFILE");
	if (dup2(pipex.fdout, STDOUT_FILENO) == -1)
		ft_perror(&pipex, "ERROR_DUP_OUTFILE");
	exit_status = ft_pipex(&pipex, argv, envp);
	close_files(&pipex);
	ft_clean(&pipex);
	return (exit_status);
}
