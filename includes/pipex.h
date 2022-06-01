/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:49:15 by jcalon            #+#    #+#             */
/*   Updated: 2022/06/01 17:04:07 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>
# include "libft.h"
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
int		parent(t_pipe *pipex, int i);

void	niel(char	**split);
void	close_files(t_pipe *pipex);
void	ft_clean(t_pipe *pipex);

void	get_in_out_files(t_pipe *pipex, int argc, char **argv);

char	*errinfo(char *str1, char *str2);
int		errmsg(char *str1, char *str2, char *str3);
int		cmderr(char *str1, char *str2, char *str3);
void	ft_error(t_pipe *pipex, int err);

#endif