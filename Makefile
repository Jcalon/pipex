# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/02 01:28:06 by crazyd            #+#    #+#              #
#    Updated: 2022/05/30 13:11:41 by jcalon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= pipex
NAME_B  = pipex_bonus

SRCS 	= srcs/pipex.c srcs/childs.c srcs/utils.c
OBJS 	= ${SRCS:.c=.o}

SRCS_B	= srcs/pipex_bonus.c srcs/utils.c srcs/utils_bonus.c
OBJS_B	= ${SRCS_B:.c=.o}

HEADER	= -Iincludes

CC 		= gcc
CFLAGS 	= -Wall -Wextra -Werror

.c.o: %.o : %.c
		@gcc ${CFLAGS} ${HEADER} -c $< -o $(<:.c=.o)

all: ${NAME}

${NAME}: ${OBJS}
		@echo "\033[33m----Compiling lib----"
		@make re -C ./libft
		@$(CC) ${OBJS} -Llibft -lft -o ${NAME}
		@echo "\033[32mPipex Compiled! ᕦ(\033[31m♥\033[32m_\033[31m♥\033[32m)ᕤ\n"


bonus:		${PROG_B}

${PROG_B}:	${OBJS_B}
					@echo "\033[33m----Compiling lib----"
					@make re -C ./libft
					@$(CC) ${OBJS_B} -Llibft -lft -o ${PROG_B}
					@echo "\033[32mPipex Bonus Compiled! ᕦ(\033[31m♥\033[32m_\033[31m♥\033[32m)ᕤ\n"

clean:
		@make clean -C ./libft
		@rm -f ${OBJS} ${OBJS_B}

fclean: clean
		@make fclean -C ./libft
		@rm -f $(NAME)
		@echo "\n\033[31mDeleting EVERYTHING! ⌐(ಠ۾ಠ)¬\n"

re:			fclean all

re_bonus:	fclean bonus

.PHONY: all clean fclean re re_bonus bonus party