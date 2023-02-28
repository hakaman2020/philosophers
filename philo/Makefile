#default config
CC = gcc
CFLAGS = -g -Wall -Wextra -Werror #-fsanitize=thread

#project info
NAME = philo

#library flags
LIB_FLAGS = -lpthread
#sourc code
SRC_DIR = src
SRC_FILES = $(addprefix $(SRC_DIR)/,\
			philo.c \
			philo_func.c \
			philo_actions.c \
			validation.c \
			init_sim.c \
			release_res.c\
			timecalc.c)

#object code
OBJ_DIR = obj
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all : $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(NAME) $(LIB_FLAGS)


$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
