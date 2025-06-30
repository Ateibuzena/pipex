# ==== Variables ====
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g

SRC_DIR     = src
BONUS_DIR   = src/bonus
OBJ_DIR     = objs
OBJ_BONUS   = objs_bonus

LIBFT_DIR   = ./libft
LIBFT       = $(LIBFT_DIR)/libft.a
INCLUDES    = -I$(LIBFT_DIR)

NAME        = pipex
BONUS_NAME  = pipex_bonus

# ==== Archivos comunes y específicos ====
COMMON_SRCS = ft_execute.c ft_process.c ft_utils.c
COMMON_SRCS := $(addprefix $(SRC_DIR)/, $(COMMON_SRCS))

NORMAL_SRCS = $(SRC_DIR)/pipex.c
BONUS_SRCS  = $(BONUS_DIR)/pipex_bonus.c

SRCS        = $(NORMAL_SRCS) $(COMMON_SRCS)
BONUS_ALL   = $(BONUS_SRCS) $(COMMON_SRCS)

OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BONUS_OBJS  = $(BONUS_ALL:$(SRC_DIR)/%.c=$(OBJ_BONUS)/%.o)
BONUS_OBJS  := $(BONUS_OBJS:$(BONUS_DIR)/%.c=$(OBJ_BONUS)/%.o)

# ==== Colores ====
RED     = \033[0;31m
GREEN   = \033[0;32m
YELLOW  = \033[0;33m
CYAN    = \033[0;36m
RESET   = \033[0m

# ==== Targets ====
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@printf "$(CYAN)[Linking] $@...\n$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@
	@printf "$(GREEN)[Success] $@ built!\n$(RESET)"

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(LIBFT) $(BONUS_OBJS)
	@printf "$(CYAN)[Linking Bonus] $@...\n$(RESET)"
	@$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $@
	@printf "$(GREEN)[Success] $@ built!\n$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "$(YELLOW)[Compiling] $<\n$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_BONUS)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "$(YELLOW)[Compiling Bonus Common] $<\n$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_BONUS)/%.o: $(BONUS_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "$(YELLOW)[Compiling Bonus Specific] $<\n$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@printf "$(CYAN)[Libft] Compiling...\n$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@printf "$(RED)[Cleaning] Object files...\n$(RESET)"
	@rm -rf $(OBJ_DIR) $(OBJ_BONUS)

fclean: clean
	@printf "$(RED)[Cleaning] Binaries and libft...\n$(RESET)"
	@rm -f $(NAME) $(BONUS_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all bonus clean fclean re
