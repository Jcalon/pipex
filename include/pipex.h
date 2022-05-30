/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:49:15 by jcalon            #+#    #+#             */
/*   Updated: 2022/05/30 16:06:52 by jcalon           ###   ########.fr       */
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
	int		bouts[2];
	pid_t	pid1;
	pid_t	pid2;
	int		fdin;
	int		fdout;
	char	**cmd1;
	char	**cmd2;
}				t_pipe;

void	child2(t_pipe pipex, char **argv, char **envp);
void	child1(t_pipe pipex, char **argv, char **envp);
void	niel(char	**split);
void	ft_perror(t_pipe pipex, char *str, int freepath, int freecmd);
void	ft_error(char *str);

#endif