# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/08 22:57:06 by lakdogan          #+#    #+#              #
#    Updated: 2025/09/15 02:22:31 by lakdogan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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

MINISHELL_MANDATORY_FILES := \
				$(BUILTINS_DIR)builtins.c \
				$(BUILTINS_DIR)cd.c \
				$(BUILTINS_DIR)cd_path_utils.c \
				$(BUILTINS_DIR)cd_utils.c \
				$(BUILTINS_DIR)echo.c \
				$(BUILTINS_DIR)env_utils_a.c \
				$(BUILTINS_DIR)env_utils_b.c \
				$(BUILTINS_DIR)env_utils_c.c \
				$(BUILTINS_DIR)env.c \
				$(BUILTINS_DIR)env_modifier.c \
				$(BUILTINS_DIR)env_parser.c \
				$(BUILTINS_DIR)env_exec.c \
				$(BUILTINS_DIR)exit.c \
				$(BUILTINS_DIR)export_print.c \
				$(BUILTINS_DIR)export.c \
				$(BUILTINS_DIR)export_process.c \
				$(BUILTINS_DIR)pwd.c \
				$(BUILTINS_DIR)unset_utils.c \
				$(BUILTINS_DIR)unset.c \
				$(DISPATCHER_DIR)builtin_handler.c \
				$(DISPATCHER_DIR)command_utils.c \
				$(DISPATCHER_DIR)external_command.c \
				$(DISPATCHER_DIR)redirect_utils.c \
				$(EXECUTOR_DIR)build_cmd_array.c \
				$(EXECUTOR_DIR)execute_left_cmd_prepare.c \
				$(EXECUTOR_DIR)execute_left_cmd.c \
				$(EXECUTOR_DIR)execute_right_cmd_prepare.c \
				$(EXECUTOR_DIR)execute_right_cmd.c \
				$(EXECUTOR_DIR)external_executor.c \
				$(EXECUTOR_DIR)node_executors.c \
				$(EXECUTOR_DIR)arg_expansion.c \
				$(EXECUTOR_DIR)file_expansion.c \
				$(EXECUTOR_DIR)cmd_executor.c \
				$(EXECUTOR_DIR)path_resolver.c \
				$(EXECUTOR_DIR)cmd_node_prep.c \
				$(EXECUTOR_DIR)builtin_executor.c \
				$(EXECUTOR_DIR)execute_subshell.c \
				$(EXECUTOR_DIR)execute_subshell_utils.c \
				$(PATH_DIR)path_resolver.c \
				$(PATH_DIR)path_utils.c \
				$(PIPE_DIR)handle_pipe_utils.c \
				$(PIPE_DIR)handle_pipe.c \
				$(PIPE_DIR)nested_pipe_process.c \
				$(PIPE_DIR)nested_pipe_setup.c \
				$(PIPE_DIR)pipe_command_executor.c \
				$(PIPE_DIR)pipe_process_manager.c \
				$(PIPE_DIR)pipe_chain_executor.c \
				$(PIPE_DIR)pipe_path_utils.c \
				$(PIPE_DIR)pipe_redirections.c \
				$(PIPE_DIR)pipe_builtin.c \
				$(PIPE_DIR)pipe_external.c \
				$(PIPE_DIR)pipe_env.c \
				$(CMD_UTILS_DIR)command_utils.c \
				$(CMD_UTILS_DIR)cd_pipe_utils.c \
				$(CORE_DIR)execute_tree.c \
				$(CORE_DIR)env_assignment.c \
				$(CORE_DIR)heredoc_detection.c \
				$(CORE_DIR)node_execution.c \
				$(CORE_DIR)tree_validation.c \
				$(ENV_EXP_DIR)exp_single_var.c \
				$(ENV_EXP_DIR)exp_utils.c \
				$(ENV_EXP_DIR)exp_var_process.c \
				$(ENV_EXP_DIR)exp_var_utils.c \
				$(ENV_EXP_DIR)exp_vars.c \
				$(ENV_EXP_DIR)exp_var_helpers.c \
				$(ENV_EXP_DIR)exp_vars_helpers.c \
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
				$(PARSE_RD_PARSER_DIR)parse_logic_ops.c \
				$(PARSE_RD_PARSER_DIR)merge_tokens.c \
				$(PARSE_RD_PARSER_DIR)arg_proc_utils.c \
				$(PARSE_RD_PARSER_DIR)expand_token_value_utils.c \
				$(PARSE_RD_PARSER_DIR)expand_token_value.c \
				$(PARSE_RD_PARSER_DIR)parse_sequence.c \
				$(PARSE_RD_PARSER_DIR)parse_subshell.c \
				$(PARSE_RD_PARSER_DIR)parser_init.c \
				$(PARSE_RD_PARSER_DIR)parser_utils.c \
				$(PARSE_RD_PARSER_DIR)redirection_handler.c \
				$(PARSE_RD_PARSER_DIR)arg_processor.c \
				$(PARSE_RD_PARSER_DIR)arg_list_utils.c \
				$(PARSE_RD_PARSER_DIR)arg_node_utils.c \
				$(PARSE_RD_PARSER_DIR)lst_to_argv.c \
				$(PARSE_RD_PARSER_DIR)redirection_handlers.c \
				$(PARSE_RD_PARSER_DIR)pipeline_parser.c \
				$(PARSE_RD_PARSER_DIR)command_parser.c \
				$(PARSE_RD_PARSER_DIR)assignment_parser.c \
				$(PARSE_RD_PARSER_DIR)redirection_parser.c \
				$(PARSE_RD_PARSER_DIR)cmd_token_processor.c \
				$(PARSE_RD_PARSER_DIR)word_processor.c \
				$(PARSE_RD_PARSER_DIR)outfile_utils.c \
				$(PARSE_RD_PARSER_DIR)infile_utils.c \
				$(PARSE_TOKENIZER_DIR)token_init.c \
				$(PARSE_TOKENIZER_DIR)token_quotes.c \
				$(PARSE_TOKENIZER_DIR)token_len.c \
				$(PARSE_TOKENIZER_DIR)token_helper.c \
				$(PARSE_TOKENIZER_DIR)token.c \
				$(PARSE_TOKENIZER_DIR)lexer_loop.c \
				$(PARSE_TOKENIZER_DIR)token_boundaries.c \
				$(PARSE_TOKENIZER_DIR)token_operators.c \
				$(PARSE_TOKENIZER_DIR)is_valid_assignment.c \
				$(PARSE_TOKENIZER_DIR)get_tok_type.c \
				$(PARSE_TOKENIZER_DIR)get_tok_state.c \
				$(PARSE_TOKENIZER_DIR)get_env_var.c \
				$(PARSE_TOKENIZER_DIR)restore_escaped_dollars.c \
				$(PARSE_TOKENIZER_DIR)process_dollar.c \
				$(PARSE_TOKENIZER_DIR)special_vars.c \
				$(PARSE_TOKENIZER_DIR)numeric_utils.c \
				$(PARSE_TOKENIZER_DIR)backslash_processor.c \
				$(PARSE_TOKENIZER_DIR)double_quote_processor.c \
				$(PARSE_TOKENIZER_DIR)unquoted_processor.c \
				$(PARSE_TOKENIZER_DIR)single_quote_processor.c \
				$(PARSE_TOKENIZER_DIR)token_processor.c \
				$(PARSE_TOKENIZER_DIR)token_properties.c \
				$(PARSE_TOKENIZER_DIR)token_special_handlers.c \
				$(PARSE_TOKENIZER_DIR)token_standard.c \
				$(UTILS_DIR)arg_validator.c \
				$(UTILS_DIR)ast_handler.c \
				$(UTILS_DIR)env_handler.c \
				$(UTILS_DIR)input_handler.c \
				$(UTILS_DIR)reset_terminal.c \
				$(SRCS_DIR)minishell.c \

MINISHELL_BONUS_FILES := \
				$(BONUS_SRCS_DIR)execute_node_by_type_bonus.c \
				$(BONUS_SRCS_DIR)execute_tree_bonus.c \
				$(BONUS_SRCS_DIR)logical_operators_bonus.c \
				$(BONUS_SRCS_DIR)get_tok_type_bonus.c \
                $(BONUS_SRCS_DIR)token_operators_bonus.c \
                $(BONUS_SRCS_DIR)token_special_handlers_bonus.c \
                $(BONUS_SRCS_DIR)token_special_handler_bonus.c \
				$(BONUS_SRCS_DIR)token_utilities_bonus.c \
				$(BONUS_SRCS_DIR)execute_subshell_bonus.c \
				$(BONUS_SRCS_DIR)pipe_in_subshell_bonus.c \
				$(BONUS_SRCS_DIR)wildcard_exp_bonus.c \
				$(BONUS_SRCS_DIR)wildcard_utils_bonus.c \
				$(BONUS_SRCS_DIR)external_command_bonus.c \
				$(BONUS_SRCS_DIR)token_processor_bonus.c \
				$(BONUS_SRCS_DIR)pipe_command_executor_bonus.c \
				$(BONUS_SRCS_DIR)cmd_executor_bonus.c \
				$(BONUS_SRCS_DIR)execute_with_path.c \


MANDATORY_WITHOUT_MAIN 			:= $(filter-out $(SRCS_DIR)minishell.c,$(MINISHELL_MANDATORY_FILES))
MANDATORY_WITHOUT_EXECUTE_TREE 	:= $(filter-out $(SRCS_DIR)exec/core/execute_tree.c,$(MANDATORY_WITHOUT_MAIN))

MINISHELL_OBJS					:= $(patsubst $(SRCS_DIR)%.c,$(OBJECTS_DIR)srcs/%.o,$(MINISHELL_MANDATORY_FILES))
MINISHELL_BONUS_OBJS := \
	$(patsubst $(BONUS_SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs_bonus/%.o,$(BONUS_SPECIFIC_FILES)) \
	$(patsubst $(SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs/%.o,$(MANDATORY_WITHOUT_MAIN))


REPLACED_BY_BONUS := \
    $(SRCS_DIR)exec/core/execute_tree.c \
    $(SRCS_DIR)parse/tokenizer/token_operators.c \
    $(SRCS_DIR)parse/tokenizer/get_tok_type.c \
    $(SRCS_DIR)parse/tokenizer/token_special_handlers.c \
    $(SRCS_DIR)parse/tokenizer/token_special_handler.c \
	$(DISPATCHER_DIR)external_command.c \
	$(PARSE_TOKENIZER_DIR)token_processor.c \
	$(PIPE_DIR)pipe_command_executor.c \
	$(EXECUTOR_DIR)cmd_executor.c \
	$(EXECUTOR_DIR)execute_subshell.c \


MANDATORY_FOR_BONUS := $(filter-out $(REPLACED_BY_BONUS),$(MANDATORY_WITHOUT_MAIN))


BONUS_OBJS := $(patsubst $(BONUS_SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs_bonus/%.o,$(MINISHELL_BONUS_FILES)) \
              $(patsubst $(SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs/%.o,$(MANDATORY_FOR_BONUS)) \
              $(BONUS_OBJECTS_DIR)srcs/minishell.o

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

$(BONUS_OBJECTS_DIR)srcs/minishell.o: $(SRCS_DIR)minishell.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(BONUS_INCLUDES) -DBONUS_BUILD -c $< -o $@

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
	rm -rf $(LIBFT_DIR)

re: fclean all

bonusre: fclean bonus

run: all
	@echo "Running $(NAME)...\n"
	@./$(NAME)

runbonus: bonus
	@echo "Running $(BONUS_NAME)...\n"
	@./$(BONUS_NAME)

valgrind: all
	@echo "Running $(NAME) with valgrind...\n"
	@valgrind --leak-check=full ./$(NAME)

.PHONY: all bonus clean fclean re bonusre run runbonus valgrind
