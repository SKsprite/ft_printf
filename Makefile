NAME = libftprintf.a

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a


SRC = ft_printf.c\
	  handler.c\
	  handle_unsigned.c\
	  handle_signed.c\
	  format_spec.c

OBJ = ${SRC:.c=.o}

HEADER = ft_printf.h

CC = cc
AR = ar rcs
RMF = rm -f

CFLAGS = -Wall -Werror -Wextra -I$(LIBFT_DIR)


all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	cp $(LIBFT) $(NAME)
	$(AR) $(NAME) $(OBJ)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

bonus: all

%.o:%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(RMF) $(OBJ)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(RMF) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
