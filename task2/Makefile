NAME = shoping
SRC =	main.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) 
	@gcc -pthread $(OBJ) -o $(NAME)

%.o: %.c
	@gcc -c $< -o $@

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all