##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## compiles program
##

CC		=	g++

SRC		=	main.cpp			\
			src/ThreadPool.cpp

OBJ		=	$(SRC:.cpp=.o)

NAME	=	plazza

CPPFLAGS	=	-std=c++20 -iquote ./src -Wall -Wextra

all:	$(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean:		clean
	$(RM) $(NAME)

re:		fclean all

.PHONY:	all clean fclean re
