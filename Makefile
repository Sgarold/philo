NAME = philosophers

B_NAME = bonus_philosophers

SRC =		mandatory/philosophers.c \
			mandatory/threads.c \
			mandatory/utils.c

B_SRC =	bonus/bonus_philosophers.c \
			bonus/utils.c \
			bonus/parser.c \
			bonus/philo_func.c 

OBJ = $(SRC:.c=.o)

B_OBJ = $(B_SRC:.c=.o)

FLAGS =	-Wall -Wextra -Werror -pthread

all: $(NAME)

bonus: $(B_NAME)

$(NAME): $(OBJ)
	gcc $(FLAGS) $(OBJ) -o $(NAME)

$(B_NAME): $(B_OBJ)
	gcc $(FLAGS) $(B_OBJ) -o $(B_NAME)

%.o: %.c philosophers.h
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(B_OBJ)

fclean: clean
	rm -rf $(NAME) $(B_NAME)
	
re: fclean all

norm:
	norminette

.PHONY: all clean fclean re norm