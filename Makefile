# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/30 16:08:21 by whendrik          #+#    #+#              #
#    Updated: 2024/01/15 10:23:36 by jhurpy           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC = @gcc
C_FLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address
#S_FLAGS = -g -fsanitize=address,undefined,leak

# Commands
RM = @rm -rf
AR = ar rc

# ---------------------------------------------------------------------------- #

# Target library name and directories
NAME = minishell
LIBFT = ./libft/libft.a
SRC_DIR = src
SRC_FILES = main.c \
			checker_and_split/check_line.c \
			checker_and_split/split_tokens.c \
			identify_and_syntax/token_identify.c\
			identify_and_syntax/token_syntax.c\
			var_expander/expand_var.c \
			var_expander/expander.c \
			quote_trimmer/quote_trim.c \
			struct_filler/struct_fill.c \
			utils/utils_token_is.c \
			utils/utils_token_len.c \
			builtins/ft_cd.c \
			builtins/ft_echo.c \
			builtins/ft_env.c \
			builtins/ft_exit.c \
			builtins/ft_export.c \
			builtins/ft_pwd.c \
			builtins/ft_unset.c \
			builtins/utils_builtins.c \
			execution/exec_builtins.c \
			execution/execute_cmd.c \
			execution/fork_process.c \
			execution/separate_op.c \
			message/error_system.c \
			redirection/dup_files.c \
			redirection/here_doc.c \
			redirection/open_infiles.c \
			redirection/open_outfiles.c \
			redirection/redirection.c \
			redirection/redirection_files.c \
			environment/set_env.c \
			environment/free_env.c \
			free_functions.c \
			signal.c

OBJ_DIR = obj
INC_DIR = includes
LIBFT_DIR = ./libft
HEAD = -I./includes -I$(READLINE_DIR)include/
INCS = -I$(INC_DIR) -I$(LIBFT_DIR)

# ### INCLUDE ###
LIB 	= -lreadline -L$(READLINE_DIR)lib/
READLINE_DIR = /usr/local/opt/readline/

# Sources, objects and dependencies
SOURCES = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJECTS = $(SOURCES:$(SRC_DIR)%.c=$(OBJ_DIR)/%.o)

# ---------------------------------------------------------------------------- #

# Default target, build the library
all: $(LIBFT_DIR) $(NAME)

# Rule to build each personal library
$(LIBFT):
	@make -C $(LIBFT_DIR)

# Object file build rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $(HEAD) -c $< -o $@

# Target library build rule
$(NAME): $(OBJECTS) $(LIBFT)
	$(CC) $(C_FLAGS) $(LIB) $^ $(INCS) -o $(NAME)

# ---------------------------------------------------------------------------- #

# Clean object files
clean:
	$(RM) $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)

# Clean object files and target library
fclean: clean
	$(RM) $(NAME)
	$(RM) $(LIBFT_DIR)/libft.a
	@make fclean -C $(LIBFT_DIR)

# Clean and rebuild the target library
re: fclean all

# Check code style
norm:
	@norminette -R CheckForbiddenSourceHeader $(SRC_DIR) ;
	@norminette -R CheckDefine $(INC_DIR)/*.h ;
	@norminette -R CheckForbiddenSourceHeader $(LIBFT_DIR)/src/*.c ;
	@norminette -R CheckDefine $(LIBFT_DIR)/includes/*.h

# Phony targets
.PHONY: all clean fclean re norm