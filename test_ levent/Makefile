


CC 					:= 	cc 
CFLAGS				:=	-Wall -Wextra -Werror

LIBFT_URL			:=	https://github.com/lakdogan/libft.git	
LIBFT_DIR			:=	libft
	
INCLUDES			:=	-I./mandatory/inc -I./$(LIBFT_DIR)
BONUS_INCLUDES		:=	-I./bonus/includes_bonus/core_bonus -I./$(LIBFT_DIR)
	
NAME				:=	minishell
BONUS_NAME			:=	minishell_bonus

LIBFT				:=	$(LIBFT_DIR)/libft_ftpf_gnl.a
	
OBJECTS_DIR				:=	./objs/
BONUS_OBJECTS_DIR   	:=  ./bonus_objs/
MANDATORY_DIR			:=	./mandatory/
BONUS_DIR				:=	./bonus/
SRCS_DIR				:=	$(MANDATORY_DIR)srcs/
BUILTINS_DIR			:=	$(SRCS_DIR)builtins/
EXECUTION_DIR			:=	$(SRCS_DIR)exec/
ERROR_DIR				:=	$(SRCS_DIR)error/
COMMAND_DIR				:=	$(EXECUTION_DIR)command/
DISPATCHER_DIR			:=	$(COMMAND_DIR)dispatcher/
EXECUTOR_DIR			:=	$(COMMAND_DIR)executor/
PATH_DIR				:=	$(COMMAND_DIR)path/
PIPE_DIR				:=	$(COMMAND_DIR)pipe/
CMD_UTILS_DIR			:=	$(COMMAND_DIR)utils/
CORE_DIR				:=	$(EXECUTION_DIR)core/
EX_ERROR_DIR			:=	$(EXECUTION_DIR)error/
IO_UTILS_DIR			:=	$(EXECUTION_DIR)io_utils/
REDIRECTION_DIR			:=	$(EXECUTION_DIR)redirection/
SIGNALS_DIR				:=	$(EXECUTION_DIR)signals/
INIT_DIR				:=	$(SRCS_DIR)init/
MEM_DIR					:=	$(SRCS_DIR)mem/
ALLOC_DIR				:=	$(MEM_DIR)alloc/
FREE_DIR				:=	$(MEM_DIR)free/
PARSING_DIR				:=	$(SRCS_DIR)parse/
UTILS_DIR				:=	$(SRCS_DIR)utils/
VALID_DIR				:=	$(SRCS_DIR)valid/
BONUS_SRCS_DIR			:=	$(BONUS_DIR)srcs_bonus/
BONUS_BUILTINS_DIR		:=	$(BONUS_SRCS_DIR)builtins_bonus/
BONUS_EXEC_DIR			:=	$(BONUS_SRCS_DIR)exec_bonus/
BONUS_COMMAND_DIR		:=	$(BONUS_EXEC_DIR)command_bonus/
BONUS_CORE_DIR			:=	$(BONUS_EXEC_DIR)core_bonus/
BONUS_EX_ERROR_DIR		:=	$(BONUS_EXEC_DIR)error_bonus/
BONUS_IO_UTILS_DIR		:=	$(BONUS_EXEC_DIR)io_utils_bonus/
BONUS_REDIRECTION_DIR	:=	$(BONUS_EXEC_DIR)redirection_bonus/
BONUS_SIGNALS_DIR		:=	$(BONUS_EXEC_DIR)signals_bonus/

MINISHELL_MANDATORY_FILES := \
				$(BUILTINS_DIR)builtins.c \
				$(BUILTINS_DIR)cd.c \
				$(BUILTINS_DIR)echo.c \
				$(BUILTINS_DIR)env_utils.c \
				$(BUILTINS_DIR)env.c \
				$(BUILTINS_DIR)exit.c \
				$(BUILTINS_DIR)export_print.c \
				$(BUILTINS_DIR)export_utils.c \
				$(BUILTINS_DIR)export.c \
				$(BUILTINS_DIR)pwd.c \
				$(BUILTINS_DIR)unset_utils.c \
				$(BUILTINS_DIR)unset.c \
				$(DISPATCHER_DIR)command_dispatcher.c \
				$(EXECUTOR_DIR)execute_left_cmd_prepare.c \
				$(EXECUTOR_DIR)execute_left_cmd.c \
				$(EXECUTOR_DIR)execute_right_cmd_prepare.c \
				$(EXECUTOR_DIR)execute_right_cmd.c \
				$(EXECUTOR_DIR)external_executor.c \
				$(EXECUTOR_DIR)node_executors.c \
				$(PATH_DIR)path_resolver.c \
				$(PATH_DIR)path_utils.c \
				$(PIPE_DIR)handle_pipe_utils.c \
				$(PIPE_DIR)handle_pipe.c \
				$(PIPE_DIR)nested_pipe_process.c \
				$(PIPE_DIR)nested_pipe_setup.c \
				$(CMD_UTILS_DIR)command_utils.c \
				$(CORE_DIR)execute_tree.c \
				$(EX_ERROR_DIR)error_utils.c \
				$(IO_UTILS_DIR)fd_operations.c \
				$(IO_UTILS_DIR)pipe_operations.c \
				$(IO_UTILS_DIR)read_from_pipe.c \
				$(REDIRECTION_DIR)handle_redirections.c \
				$(REDIRECTION_DIR)heredoc_content.c \
				$(REDIRECTION_DIR)heredoc.c \
				$(REDIRECTION_DIR)redirection_files.c \
				$(SIGNALS_DIR)signal_handlers.c \
				$(INIT_DIR)init_minishell.c \
				$(SRCS_DIR)minishell.c \

MINISHELL_BONUS_FILES := \
				$(BONUS_SRCS_DIR)minishell_bonus.c \
				$(BONUS_CORE_DIR)execute_tree_bonus.c \
				$(BONUS_CORE_DIR)logical_operators_bonus.c \
				$(BONUS_BUILTINS_DIR)builtins_bonus.c \
				$(BONUS_BUILTINS_DIR)cd_bonus.c \
				$(BONUS_BUILTINS_DIR)echo_bonus.c \
				$(BONUS_BUILTINS_DIR)env_bonus.c \
				$(BONUS_BUILTINS_DIR)exit_bonus.c \
				$(BONUS_BUILTINS_DIR)export_print_bonus.c \
				$(BONUS_BUILTINS_DIR)export_utils_bonus.c \
				$(BONUS_BUILTINS_DIR)export_bonus.c \
				$(BONUS_BUILTINS_DIR)pwd_bonus.c \
				$(BONUS_BUILTINS_DIR)unset_bonus.c \
				$(BONUS_EX_ERROR_DIR)error_utils_bonus.c \
				$(BONUS_COMMAND_DIR)command_dispatcher_bonus.c \
				$(BONUS_COMMAND_DIR)command_utils_bonus.c \
				$(BONUS_COMMAND_DIR)external_executor_bonus.c \
				$(BONUS_COMMAND_DIR)node_executors_bonus.c \
				$(BONUS_COMMAND_DIR)path_resolver_bonus.c \
				$(BONUS_COMMAND_DIR)path_utils_bonus.c \
				$(BONUS_IO_UTILS_DIR)fd_operations_bonus.c \
				$(BONUS_IO_UTILS_DIR)pipe_operations_bonus.c \
				$(BONUS_REDIRECTION_DIR)handle_redirections_bonus.c \
				$(BONUS_REDIRECTION_DIR)heredoc_content_bonus.c \
				$(BONUS_REDIRECTION_DIR)heredoc_bonus.c \
				$(BONUS_REDIRECTION_DIR)redirection_files_bonus.c \
				$(BONUS_SIGNALS_DIR)signal_handlers_bonus.c \


MINISHELL_OBJS	:= $(patsubst $(SRCS_DIR)%.c,$(OBJECTS_DIR)srcs/%.o,$(MINISHELL_MANDATORY_FILES))
MINISHELL_BONUS_OBJS := $(patsubst $(BONUS_SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs_bonus/%.o,$(MINISHELL_BONUS_FILES))

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
	$(CC) $(CFLAGS) $(MINISHELL_OBJS) $(LIBFT) -lreadline -o $(NAME)
	
$(OBJECTS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

bonus: $(LIBFT) $(BONUS_NAME)

$(BONUS_NAME): $(LIBFT) $(MINISHELL_BONUS_OBJS) $(MINISHELL_BONUS_UTILS_OBJS)
	$(CC) $(CFLAGS) $(MINISHELL_BONUS_OBJS) $(MINISHELL_BONUS_UTILS_OBJS) $(LIBFT) -lreadline -o $(BONUS_NAME)

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
	# rm -rf $(LIBFT_DIR)  REMOVE COMMENT BEFORE PUSH!!!!!!!!!!!!!!!!!!!!!!!

re: fclean all

bonusre: fclean bonus

run: all
	@echo "Running $(NAME)...\n"
	@./$(NAME)

runbonus: bonus
	@echo "Running $(NAME)...\n"
	@./$(NAME)

valgrind: all
	@echo "Running $(NAME) with valgrind...\n"
	@valgrind --leak-check=full ./$(NAME)

.PHONY: all bonus clean fclean re bonusre run runbonus valgrind