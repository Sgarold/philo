NAME = philosophers
SRC =	philosophers.c threads.c utils.c

OBJ = $(SRC:.c=.o)

FLAGS =	-Wall -Wextra -Werror -pthread

all: $(NAME)

$(NAME): $(OBJ)
	gcc $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.c philosophers.h
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
	
re: fclean all

norm:
	norminette $(SRC) philosophers.h

.PHONY: all clean fclean re norm