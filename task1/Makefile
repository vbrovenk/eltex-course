NAME = task1
SRC =	main.c \
		ft_strsplit.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) 
	@gcc $(OBJ) -o $(NAME)

%.o: %.c
	@gcc -c $< -o $@

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all