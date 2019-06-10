#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/11 12:49:25 by ssoraka           #+#    #+#              #
#    Updated: 2019/03/11 20:57:23 by ssoraka          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

CC = gcc
CFLAGS = -c -Wall -Wextra -Werror
NAME = ft_ls
HEADER = ./includes/
LIB = ./libft/
SRC1 = ./sources/
SRC = ft_ls.c \
flags_and_errors.c \
parametrs_for_printing.c \
printing.c \
lists.c \
lists2.c \
return_stats.c \
parametrs_of_stats.c

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	make lib_refresh
	$(CC) -o $(NAME) -I $(HEADER) -I $(LIB) $(OBJS) $(LIB)libft.a


%.o: $(SRC1)%.c
	$(CC) $(CFLAGS) $< -I $(HEADER) -I $(LIB)

lib_refresh:
	make -C $(LIB)

norm:
	norminette -R CheckForbiddenSourceHeader

clean:
	rm -rf *.o
	make -C $(LIB) fclean

fclean: clean
	@rm -rf $(NAME)

re: fclean all
