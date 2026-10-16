NAME = libftprintf.a

SRC = \

OBJ = ${SRC:.c=.o}

HEADER = ft_printf.h

BONUS_FILE = *_bonus.c

BONUS_OBJ = ${BONUS_FILE:.c=.o}

BONUS_HEADER = ft_printf_bonus.h

CC = cc
AR = ar rcs
RM = rm -rf

CFLAGS = -Wall -Werror -Wextra
LIBFT = -Lft

all:
	$(AR)

$(NAME): all

bonus:
	$(AR) 

%.o:%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	RM $(OBJ)

fclean:
	RM $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
