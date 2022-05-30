/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:49:15 by jcalon            #+#    #+#             */
/*   Updated: 2022/05/30 21:08:16 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>
# include "../libft/libft.h"

typedef struct s_pipe
{
	char	**paths;
	char	*cmdpath;
	int		*bouts;
	pid_t	*pids;
	int		fdin;
	int		fdout;
	char	**cmd;
	int		cmds;
}				t_pipe;

void	child(t_pipe *pipex, char **argv, char **envp, int i);
void	niel(char	**split);
void	ft_perror(t_pipe *pipex, char *str);
void	close_files(t_pipe *pipex);
void	ft_clean(t_pipe *pipex);

#endif