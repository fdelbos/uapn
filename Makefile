NAME	= 	uapn
SRC		=	net.c		\
			uapn.c		\
			utils.c

OBJ		=	$(SRC:.c=.o)
CFLAGS	=	-W -Wall -O3
RM		=	rm -fr

all:	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) -lssl

clean:
	$(RM) $(OBJ) *~ *.core

fclean:	clean
	$(RM) $(NAME)

re: fclean all
