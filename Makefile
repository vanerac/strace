##
## EPITECH PROJECT, 2019
## base_repo
## File description:
## Makefile file
##

SRC		=		src/print.c			\
				src/strace.c		\

OBJ		=		$(SRC:%.c=%.o)

TRC		=		NULL

OTRC	=		$(TRC:.c=.o)

MAIN	=		src/main.c
OMAIN	=		$(MAIN:.c=.o)

CFLAGS	=		-Iinclude/ -Wextra -Wall

NAME	=		strace


all:	$(NAME)

$(NAME): $(OBJ) $(OMAIN)
	gcc -o $(NAME) $(OBJ) $(OMAIN) $(CFLAGS) -lpthread

clean:
	rm -rf $(OBJ)
	rm -rf $(OTRC)
	rm -rf $(OMAIN)

fclean:	clean
	rm -rf $(NAME)
	rm -rf unit_test

gclean:	fclean clean
	find . -name "*.gc*" -delete

re: gclean all

tests_run:	$(OTRC) $(OBJ)
	gcc -o unit_test $(TRC) $(SRC) $(CFLAGS) -lcriterion --coverage
	./unit_test

.PHONY:			gclean clean fclean re tests_run all
