##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## compiles program
##

CXX		=	g++

SRC		=	main.cpp				\
			src/ThreadPool.cpp		\
			src/Waiter.cpp			\
			src/Arguments.cpp 		\
			src/Stock.cpp			\
			src/Process.cpp			\
			src/Kitchen.cpp 		\
			src/PlazzaMenu.cpp 		\
			src/Pizza/Pizza.cpp 	\

OBJ		=	$(SRC:.cpp=.o)

NAME	=	plazza

CPPFLAGS	=	-std=c++20 -iquote ./src -iquote ./src/Pizza -iquote ./include

all:	$(NAME)

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean:		clean
	$(RM) $(NAME)

re:		fclean all

.PHONY:	all clean fclean re
