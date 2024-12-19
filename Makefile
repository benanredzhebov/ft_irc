# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/15 09:58:47 by beredzhe          #+#    #+#              #
#    Updated: 2024/12/15 09:58:50 by beredzhe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ircserv

SRC_PATH	=	src/
OBJ_PATH	=	obj/

SRC		=	$(shell find . $(SRC_PATH) -name '*.cpp')
OBJ		=	$(SRC:$(SRC_PATH)%.cpp=$(OBJ_PATH)%.o)

CC		=	c++
CFLAGS	=	-Wall -Wextra - Werror -std=c++98


all: $(NAME)

$(NAME) = $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re