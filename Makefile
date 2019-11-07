CC=gcc
CFLAGS=
RM=rm -rf
OBJS=file.o machfile.o machfile2.o main.o magic.o
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
	make -C $(LIBFT) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re

$(LIBFT)/libft.a:
	make -C $(LIBFT)
