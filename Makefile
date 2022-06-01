# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/02 01:28:06 by crazyd            #+#    #+#              #
#    Updated: 2022/06/01 17:08:23 by jcalon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= pipex

SRCS 	= srcs/pipex.c srcs/childs.c srcs/utils.c srcs/get_fd.c srcs/error.c
OBJS 	= ${SRCS:.c=.o}

CC 		= gcc
CFLAGS 	= -Wall -Wextra -Werror

INCLUDES = -I libft -I includes

.c.o:
		@${CC} ${CFLAGS} ${INCLUDES} -c -o $@ $<

all: ${NAME}

${NAME}: ${OBJS}
		@echo "\033[33m----Compiling lib----"
		@make re -C libft --no-print-directory
		@$(CC) $(CFLAGS) $(OBJS) -Llibft -lft ${INCLUDES} -o ${NAME}
		@echo "\033[32mpipex Compiled! ᕦ(\033[31m♥\033[32m_\033[31m♥\033[32m)ᕤ\n"

clean:
		@make clean -C libft --no-print-directory
		@rm -f ${OBJS}

fclean: clean
		@make fclean -C libft
		@rm -f $(NAME)
		@echo "\n\033[31mDeleting EVERYTHING! ⌐(ಠ۾ಠ)¬\n"

re:			fclean all

.PHONY: all clean fclean re re_bonus bonus party