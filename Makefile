NAME = libftprintf.a

SRC = ft_printf.c

OBJ = ${SRC:.c=.o}

HEADER = ft_printf.h

BONUS_FILE = *_bonus.c

BONUS_OBJ = ${BONUS_FILE:.c=.o}

BONUS_HEADER = ft_printf_bonus.h

CC = cc
AR = ar rcs
RMF = rm -f

CFLAGS = -Wall -Werror -Wextra
LIBFT = -Lft

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

bonus: $(NAME)

%.o:%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RMF) $(OBJ)

fclean: clean
	$(RMF) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
