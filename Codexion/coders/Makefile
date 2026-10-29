# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vabad-ro <vabad-ro@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/16 13:53:00 by vabad-ro         #+#    #+#              #
#    Updated: 2026/10/19 16:23:44 by vabad-ro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion
HEADERS = codexion.h

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

SRC = main.c parser.c init.c coder.c time.c heap.c monitor.c tools.c
OBJ = $(SRC:.c=.o)

RM = rm -f

all: $(NAME)

$(NAME): $(OBJ) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ) 

fclean: clean
	$(RM) $(NAME) *.out

re: fclean all

.PHONY: all clean fclean re
