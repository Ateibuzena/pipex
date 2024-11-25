# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

LIBFT_DIR = /home/azubieta/sgoinfre/azubieta/Utils/libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I$(LIBFT_DIR)

SRC_DIR = ./src
SRCS = $(SRC_DIR)/pipex.c\
		$(SRC_DIR)/ft_execute.c\
		$(SRC_DIR)/ft_process.c\
		$(SRC_DIR)/ft_utils.c\

OBJ_DIR = ./objs
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

NAME = pipex

OBJ_DIR_BONUS = ./objs_bonus
BONUS_DIR = ./src
BONUS_SRCS = $(BONUS_DIR)/bonus/pipex_bonus.c\
			$(BONUS_DIR)/ft_execute.c\
			$(BONUS_DIR)/ft_process.c\
			$(BONUS_DIR)/ft_utils.c\

BONUS_OBJS = $(BONUS_SRCS:$(BONUS_DIR)/%.c=$(OBJ_DIR_BONUS)/%.o)

BONUS_NAME = pipex_bonus

# Colors
RED     			= \033[0;31m
GREEN   			= \033[0;32m
YELLOW  			= \033[0;33m
CYAN    			= \033[0;36m
WHITE   			= \033[0;37m
RESET   			= \033[0m

# Reglas
all: $(NAME)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(OBJ_DIR_BONUS) $(LIBFT) $(BONUS_OBJS)
	@printf "$(CYAN)[Building Bonus]$(RESET) Creating $(BONUS_NAME)...\n"
	@$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $(BONUS_NAME)
	@printf "$(GREEN)[Success]$(RESET) $(BONUS_NAME) created successfully!\n"

$(NAME): $(OBJ_DIR) $(LIBFT) $(OBJS)
	@printf "$(CYAN)[Building Main]$(RESET) Creating $(NAME)...\n"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@printf "$(GREEN)[Success]$(RESET) $(NAME) created successfully!\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@printf "$(YELLOW)[Compiling]$(RESET) $<\n"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR_BONUS)/%.o: $(BONUS_DIR)/%.c | $(OBJ_DIR_BONUS)
	@mkdir -p $(dir $@)
	@printf "$(YELLOW)[Compiling Bonus]$(RESET) $<\n"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	@printf "$(CYAN)[Directory]$(RESET) Creating object directory $(OBJ_DIR)...\n"
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR_BONUS):
	@printf "$(CYAN)[Directory]$(RESET) Creating bonus object directory $(OBJ_DIR_BONUS)...\n"
	@mkdir -p $(OBJ_DIR_BONUS)

$(LIBFT):
	@printf "$(CYAN)[Building Libft]$(RESET) Compiling libft...\n"
	@$(MAKE) -C $(LIBFT_DIR)
	@$(MAKE) -C $(LIBFT_DIR) bonus
	@$(MAKE) -C $(LIBFT_DIR) clean
	@printf "$(GREEN)[Libft Ready]$(RESET) Libft compiled successfully!\n"

clean:
	@printf "$(RED)[Cleaning]$(RESET) Removing object files...\n"
	@rm -rf $(OBJ_DIR) $(OBJ_DIR_BONUS)
	@printf "$(GREEN)[Cleaned]$(RESET) Object files removed successfully!\n"

fclean: clean
	@printf "$(RED)[Full Clean]$(RESET) Removing binaries and $(LIBFT)...\n"
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME) $(BONUS_NAME)
	@printf "$(GREEN)[Cleaned]$(RESET) All binaries and libraries removed successfully!\n"

re: fclean all

.PHONY: all clean fclean re bonus
