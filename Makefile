CC=gcc
CFLAGS=-Wall -Wextra -Werror
RM=rm -rf
OBJS=file.o main.o magic.o fat.o \
	 machfile.o machfile_32.o machfile_64.o \
	 sort_symtab.o sort_symtab_32.o sort_symtab_64.o
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
