


CC 					:= 	cc
CFLAGS				:=	-Wall -Wextra -Werror

LIBFT_URL			:=	https://github.com/lakdogan/libft.git
LIBFT_DIR			:=	libft

INCLUDES			:=	-I./mandatory/inc -I./$(LIBFT_DIR)
BONUS_INCLUDES		:=	-I./bonus/inc_bonus -I./$(LIBFT_DIR)

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
ENV_EXP_DIR				:=	$(EXECUTION_DIR)env_expansion/
EX_ERROR_DIR			:=	$(EXECUTION_DIR)error/
IO_UTILS_DIR			:=	$(EXECUTION_DIR)io_utils/
REDIRECTION_DIR			:=	$(EXECUTION_DIR)redirection/
SIGNALS_DIR				:=	$(EXECUTION_DIR)signals/
INIT_DIR				:=	$(SRCS_DIR)init/
MEM_DIR					:=	$(SRCS_DIR)mem/
ALLOC_DIR				:=	$(MEM_DIR)alloc/
FREE_DIR				:=	$(MEM_DIR)free/
PARSING_DIR				:=	$(SRCS_DIR)parse/
PARSE_ERROR_DIR			:=	$(PARSING_DIR)error/
PARSE_RD_PARSER_DIR		:=	$(PARSING_DIR)rd_parser/
PARSE_TOKENIZER_DIR		:=	$(PARSING_DIR)tokenizer/
UTILS_DIR				:=	$(SRCS_DIR)utils/
VALID_DIR				:=	$(SRCS_DIR)valid/
BONUS_SRCS_DIR			:=	$(BONUS_DIR)srcs_bonus/
BONUS_BUILTINS_DIR		:=	$(BONUS_SRCS_DIR)builtins_bonus/
BONUS_EXEC_DIR			:=	$(BONUS_SRCS_DIR)exec_bonus/

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
				$(ENV_EXP_DIR)exp_single_var.c \
				$(ENV_EXP_DIR)exp_utils.c \
				$(ENV_EXP_DIR)exp_var_process.c \
				$(ENV_EXP_DIR)exp_var_utils.c \
				$(ENV_EXP_DIR)exp_vars.c \
				$(ENV_EXP_DIR)exp_find_env_value.c \
				$(EX_ERROR_DIR)error_utils.c \
				$(IO_UTILS_DIR)fd_operations.c \
				$(IO_UTILS_DIR)pipe_operations.c \
				$(IO_UTILS_DIR)read_from_pipe.c \
				$(REDIRECTION_DIR)file_utils.c \
				$(REDIRECTION_DIR)handle_redirections.c \
				$(REDIRECTION_DIR)heredoc_content.c \
				$(REDIRECTION_DIR)heredoc.c \
				$(REDIRECTION_DIR)process_heredoc.c \
				$(SIGNALS_DIR)signal_handlers.c \
				$(INIT_DIR)create_env_node.c \
				$(INIT_DIR)init_env.c \
				$(INIT_DIR)init_minishell.c \
				$(PARSE_ERROR_DIR)error.c \
				$(PARSE_RD_PARSER_DIR)parse_cmd.c \
				$(PARSE_RD_PARSER_DIR)parse_logic_ops.c \
				$(PARSE_RD_PARSER_DIR)parse_pipeline.c \
				$(PARSE_RD_PARSER_DIR)parse_subshell.c \
				$(PARSE_RD_PARSER_DIR)parser_init.c \
				$(PARSE_RD_PARSER_DIR)parser_utils_2.c \
				$(PARSE_RD_PARSER_DIR)parser_utils.c \
				$(PARSE_TOKENIZER_DIR)token_init.c \
				$(PARSE_TOKENIZER_DIR)token_quotes.c \
				$(PARSE_TOKENIZER_DIR)token_len.c \
				$(PARSE_TOKENIZER_DIR)token_helper.c \
				$(PARSE_TOKENIZER_DIR)token.c \
				$(SRCS_DIR)minishell.c \

MINISHELL_BONUS_FILES := \
				$(BONUS_SRCS_DIR)minishell_bonus.c \
				$(BONUS_EXEC_DIR)execute_tree_bonus.c \
				$(BONUS_EXEC_DIR)logical_operators_bonus.c \

MANDATORY_WITHOUT_MAIN 			:= $(filter-out $(SRCS_DIR)minishell.c,$(MINISHELL_MANDATORY_FILES))
MANDATORY_WITHOUT_EXECUTE_TREE 	:= $(filter-out $(SRCS_DIR)exec/core/execute_tree.c,$(MANDATORY_WITHOUT_MAIN))

MINISHELL_OBJS					:= $(patsubst $(SRCS_DIR)%.c,$(OBJECTS_DIR)srcs/%.o,$(MINISHELL_MANDATORY_FILES))
MINISHELL_BONUS_OBJS := \
	$(patsubst $(BONUS_SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs_bonus/%.o,$(BONUS_SPECIFIC_FILES)) \
	$(patsubst $(SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs/%.o,$(MANDATORY_WITHOUT_MAIN))

all: $(LIBFT) $(NAME)

$(LIBFT_DIR):
	git clone $(LIBFT_URL) $(LIBFT_DIR)

$(LIBFT): $(LIBFT_DIR)
	@if [ ! -f $(LIBFT_DIR)/.built ]; then \
		echo "First time build of libft - forcing clean build"; \
		cd $(LIBFT_DIR) && make clean && make && touch .built; \
	else \
		cd $(LIBFT_DIR) && make; \
	fi
	@touch $(LIBFT)

$(NAME): $(LIBFT) $(MINISHELL_OBJS)
	$(CC) $(CFLAGS) $(MINISHELL_OBJS) $(LIBFT) -lreadline -o $(NAME)

$(OBJECTS_DIR)srcs/%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

bonus: $(LIBFT) $(BONUS_NAME)

BONUS_OBJS := $(patsubst $(BONUS_SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs_bonus/%.o,$(MINISHELL_BONUS_FILES)) \
			  $(patsubst $(SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs/%.o,$(MANDATORY_WITHOUT_EXECUTE_TREE))

$(BONUS_NAME): $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) libft/libft_ftpf_gnl.a -lreadline -o $(BONUS_NAME)

$(BONUS_OBJECTS_DIR)srcs_bonus/%.o: $(BONUS_SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(BONUS_INCLUDES) -DBONUS_BUILD -c $< -o $@

$(BONUS_OBJECTS_DIR)srcs/%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(BONUS_INCLUDES) -DBONUS_BUILD -c $< -o $@

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
