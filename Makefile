# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: danevans <danevans@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/15 09:58:47 by beredzhe          #+#    #+#              #
#    Updated: 2024/12/22 13:12:36 by danevans         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        =   ircserv

SRC_PATH    =   src/
OBJ_PATH    =   obj/

SRC         =   $(shell find $(SRC_PATH) -name '*.cpp')
OBJ         =   $(patsubst $(SRC_PATH)%.cpp, $(OBJ_PATH)%.o, $(SRC))

CC          =   c++
CFLAGS      =   -std=c++98
INCLUDES    =   -I include

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
