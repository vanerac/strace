##
## EPITECH PROJECT, 2019
## base_repo
## File description:
## Makefile file
##

.NOTPARALLEL:

SRC		=		src/strace.c		\
				src/tracer.c		\
				src/tracee.c		\

OBJ		=		$(SRC:%.c=%.o)

TRC		=		NULL

OTRC	=		$(TRC:.c=.o)

MAIN	=		src/main.c
OMAIN	=		$(MAIN:.c=.o)

CFLAGS	=		-Iinclude/ -Iinclude/generic/ -Wextra -Wall

NAME	=		strace


all:	$(NAME)

$(NAME): $(OBJ) $(OMAIN)
	gcc -o $(NAME) $(OBJ) $(OMAIN) $(CFLAGS) -lpthread

update_headers:
		mkdir -p include/32/
		mkdir -p include/generic/
		mkdir -p include/i386/
		mkdir -p include/x86_64/
		./updateSyscalls.sh
.PHONY:	update_headers

clean_headers:
		rm -rf include/32/
		rm -rf include/generic/
		rm -rf include/i386/
		rm -rf include/x86_64/
		rm -rf include/sysent_shorthand_defs.h
		rm -rf include/sysent.h
.PHONY:	clean_headers

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
