NAME        = philo

SRC         = main.c parse.c init.c monitor.c routine.c time_utils.c free_error.c

OBJ         = $(SRC:.c=.o)

CC          = cc
FLAGS      	= -Wall -Wextra -Werror
DEL			= rm -rf

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(MLX_FLAGS)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(DEL) $(OBJ)

fclean: clean
	$(DEL) $(NAME)

re: fclean all

.PHONY: all clean fclean re