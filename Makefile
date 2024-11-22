# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = /home/azubieta/sgoinfre/azubieta/Utils/libft
LIBFT = $(LIBFT_DIR)/libft.a


INCLUDES = -I$(LIBFT_DIR)

SRC_DIR = ./src
SRCS = $(SRC_DIR)/pipex.c\
		$(SRC_DIR)/ft_execute.c\
		$(SRC_DIR)/ft_process.c\
		$(SRC_DIR)/ft_utils.c\
		$(SRC_DIR)/ft_parse.c\

OBJ_DIR = ./objs
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

NAME = pipex

OBJ_DIR_BONUS = ./objs_bonus
BONUS_DIR = ./src
BONUS_SRCS = $(BONUS_DIR)/bonus/pipex_bonus.c\
			$(BONUS_DIR)/ft_execute.c\
			$(BONUS_DIR)/ft_process.c\
			$(BONUS_DIR)/ft_utils.c\
			$(BONUS_DIR)/ft_parse.c\

BONUS_OBJS = $(BONUS_SRCS:$(BONUS_DIR)/%.c=$(OBJ_DIR_BONUS)/%.o)
#BONUS_OBJS = $(BONUS_SRCS:./%.c=$(OBJ_DIR)/%.o)

BONUS_NAME = pipex_bonus
# Excluir la carpeta bonus
# SRCS := $(filter-out $(SRC_DIR)/bonus/%.c, $(SRCS))



# Reglas
all: $(OBJ_DIR) $(NAME)

bonus: $(LIBFT) $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $(BONUS_NAME)

$(NAME): $(LIBFT)  $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

# Crea los directorios necesarios para archivos del proyecto principal
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)   # Crea el subdirectorio si no existe
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR_BONUS)/%.o: $(BONUS_DIR)/%.c | $(OBJ_DIR_BONUS)
	@mkdir -p $(dir $@)   # Crea el subdirectorio si no existe
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Regla para crear el directorio de objetos
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR_BONUS):
	@mkdir -p $(OBJ_DIR_BONUS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

clean:
	rm -rf $(OBJ_DIR) $(OBJ_DIR_BONUS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re
