/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:49:23 by jcalon            #+#    #+#             */
/*   Updated: 2022/05/27 21:58:53 by jcalon           ###   ########.fr       */
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

char	*get_cmd(char **paths, char *cmd)
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

char	**find_paths(char **envp)
{
	char	**paths;
	while (*envp && !ft_strnstr(*envp, "PATH", 4))
		envp++;
	paths = ft_split(*envp + 5, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

void	child1(t_pipe pipex, char **argv, char **envp)
{
	pipex.fdin = open(argv[1], O_RDONLY);
	if (pipex.fdin == -1)
		ft_perror(pipex,"ERROR_INFILE", 0, 0);
	if (dup2(pipex.fdin, 0) == -1)
		ft_perror(pipex, "ERROR_DUP_INFILE", 0, 0);
	if (dup2(pipex.bouts[1], 1) == -1)
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
		ft_perror(pipex,"ERROR_OUTFILE", 0, 0);
	if (dup2(pipex.fdout, 1) == -1)
		ft_perror(pipex, "ERROR_DUP_OUTFILE", 0, 0);
	if (dup2(pipex.bouts[0], 0) == -1)
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

int main(int argc, char **argv, char **envp)
{
	t_pipe	pipex;

	if (argc != 5)
		ft_error("Error: wrong count of arguments\n");
	if (pipe(pipex.bouts) == -1)
		ft_error("ERROR\n");
	pipex.paths = find_paths(envp);
	if (!pipex.paths)
		ft_error("ERROR\n");
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
