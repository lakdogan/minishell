


CC 					:= 	cc 
CFLAGS				:=	-Wall -Wextra -Werror

LIBFT_URL			:=	https://github.com/lakdogan/libft.git	
LIBFT_DIR			:=	libft
	
INCLUDES			:=	-I./mandatory/inc -I./$(LIBFT_DIR)
BONUS_INCLUDES		:=	-I./bonus/inc_bonus -I./$(LIBFT_DIR)
	
NAME				:=	minishell
BONUS_NAME			:=	minishell_bonus

LIBFT				:=	$(LIBFT_DIR)/libft_ftpf_gnl.a
	
OBJECTS_DIR			:=	./objs/
BONUS_OBJECTS_DIR   :=  ./bonus_objs/
MANDATORY_DIR		:=	./mandatory/
BONUS_DIR			:=	./bonus/
SRCS_DIR			:=	$(MANDATORY_DIR)srcs/
BUILTINS_DIR		:=	$(SRCS_DIR)builtins/
EXECUTION_DIR		:=	$(SRCS_DIR)exec/
ERROR_DIR			:=	$(SRCS_DIR)error/
COMMAND_DIR			:=	$(EXECUTION_DIR)command/
CORE_DIR			:=	$(EXECUTION_DIR)core/
EX_ERROR_DIR		:=	$(EXECUTION_DIR)error/
IO_UTILS_DIR		:=	$(EXECUTION_DIR)io_utils/
REDIRECTION_DIR		:=	$(EXECUTION_DIR)redirection/
SIGNALS_DIR			:=	$(EXECUTION_DIR)signals/
INIT_DIR			:=	$(SRCS_DIR)init/
MEM_DIR				:=	$(SRCS_DIR)mem/
ALLOC_DIR			:=	$(MEM_DIR)alloc/
FREE_DIR			:=	$(MEM_DIR)free/
PARSING_DIR			:=	$(SRCS_DIR)parse/
UTILS_DIR			:=	$(SRCS_DIR)utils/
VALID_DIR			:=	$(SRCS_DIR)valid/
BONUS_SRCS_DIR		:=	$(BONUS_DIR)srcs_bonus/
BONUS_UTILS_DIR		:=	$(BONUS_SRCS_DIR)utils_bonus/

MINISHELL_MANDATORY_FILES := \
				$(BUILTINS_DIR)builtins.c \
				$(BUILTINS_DIR)cd.c \
				$(BUILTINS_DIR)echo.c \
				$(BUILTINS_DIR)env.c \
				$(BUILTINS_DIR)exit.c \
				$(BUILTINS_DIR)export_print.c \
				$(BUILTINS_DIR)export_utils.c \
				$(BUILTINS_DIR)export.c \
				$(BUILTINS_DIR)pwd.c \
				$(BUILTINS_DIR)unset.c \
				$(COMMAND_DIR)command_dispatcher.c \
				$(COMMAND_DIR)command_utils.c \
				$(COMMAND_DIR)external_executor.c \
				$(COMMAND_DIR)path_resolver.c \
				$(COMMAND_DIR)path_utils.c \
				$(IO_UTILS_DIR)fd_operations.c \
				$(IO_UTILS_DIR)pipe_operations.c \
				$(REDIRECTION_DIR)handle_redirections.c \
				$(REDIRECTION_DIR)heredoc_content.c \
				$(REDIRECTION_DIR)heredoc.c \
				$(REDIRECTION_DIR)redirection_files.c \

				$(SRCS_DIR)minishell.c \

MINISHELL_BONUS_FILES := \
				$(BONUS_SRCS_DIR)minishell_bonus.c \

BONUS_UTILS_FILES := \


MINISHELL_OBJS	:= $(patsubst $(SRCS_DIR)%.c,$(OBJECTS_DIR)srcs/%.o,$(MINISHELL_MANDATORY_FILES))
MINISHELL_BONUS_OBJS := $(patsubst $(BONUS_SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs_bonus/%.o,$(MINISHELL_BONUS_FILES))
MINISHELL_BONUS_UTILS_OBJS := $(patsubst $(SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs/%.o,$(BONUS_UTILS_FILES))

all: $(LIBFT) $(NAME)

$(LIBFT_DIR):
	@if [ ! -d "$(LIBFT_DIR)" ]; \
	then git clone $(LIBFT_URL)	$(LIBFT_DIR); \
	fi
	# @cd $(LIBFT_DIR) && make

$(LIBFT): $(LIBFT_DIR)
	@$(MAKE) -C $(LIBFT_DIR)
	@cd $(LIBFT_DIR) && make
	@touch $(LIBFT)
		
$(NAME): $(LIBFT) $(MINISHELL_OBJS)
	$(CC) $(CFLAGS) $(MINISHELL_OBJS) $(LIBFT) -o $(NAME)
	
$(OBJECTS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

bonus: $(LIBFT) $(BONUS_NAME)

$(BONUS_NAME): $(LIBFT) $(MINISHELL_BONUS_OBJS) $(MINISHELL_BONUS_UTILS_OBJS)
	$(CC) $(CFLAGS) $(MINISHELL_BONUS_OBJS) $(MINISHELL_BONUS_UTILS_OBJS) $(LIBFT) -o $(BONUS_NAME)

$(OBJECTS_DIR)srcs/%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BONUS_OBJECTS_DIR)srcs_bonus/%.o: $(BONUS_SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(BONUS_INCLUDES) -c $< -o $@

$(BONUS_OBJECTS_DIR)srcs/%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(BONUS_INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJECTS_DIR)
	rm -rf $(BONUS_OBJECTS_DIR)
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) -C $(LIBFT_DIR) clean; fi

fclean: clean
	rm -f $(NAME)
	rm -f $(BONUS_NAME)
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) -C $(LIBFT_DIR) clean; fi
	rm -rf $(LIBFT_DIR)

re: fclean all
	

.PHONY: all bonus clean fclean re