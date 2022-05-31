/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:49:15 by jcalon            #+#    #+#             */
/*   Updated: 2022/05/31 21:41:56 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>
# include "../libft/libft.h"
# include <string.h>
# include <errno.h>

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
	int		here_doc;
}				t_pipe;

void	child(t_pipe *pipex, char **argv, char **envp, int i);
void	niel(char	**split);
void	ft_error(t_pipe *pipex, char *str1, char *str2, int erno);
void	close_files(t_pipe *pipex);
void	ft_clean(t_pipe *pipex);
void	get_in_out_files(t_pipe *pipex, int argc, char **argv);
int		parent(t_pipe *pipex, int i);
char	*errinfo(char *str1, char *str2);

#endif