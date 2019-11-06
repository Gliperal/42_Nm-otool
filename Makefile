CC=gcc
CFLAGS=
RM=rm -rf
OBJS=file.o machfile.o main.o
NAME=ft_nm ft_otool
LIBFT=libft

vpath libft.a $(LIBFT)

all: $(NAME)

$(NAME): ft_%: %.o $(OBJS) -lft
	$(CC) $(CFLAGS) $< $(OBJS) -L$(LIBFT) -lft -o $@

clean:
	$(RM) nm.o
	$(RM) otool.o
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
