# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror 
# -fsanitize=address
LIBFT_DIR = /home/azubieta/sgoinfre/azubieta/utils/libft
LIBFT = $(LIBFT_DIR)/libft.a


INCLUDES = -I$(LIBFT_DIR)

SRC_DIR = ./src
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/ft_process.c $(SRC_DIR)/ft_utils.c\

OBJ_DIR = ./objs
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

NAME = pipex

# Reglas
all: $(NAME)

$(NAME): $(LIBFT)  $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

# Crea los directorios necesarios dentro de OBJ_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)   # Crea el subdirectorio si no existe
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Regla para crear el directorio de objetos
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

