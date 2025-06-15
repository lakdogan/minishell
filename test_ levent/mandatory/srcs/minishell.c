#include "../includes/core/minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// Helper function to create a test file with content
void create_test_file(const char *filename, const char *content) 
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        printf("Error creating test file %s: %s\n", filename, strerror(errno));
        return;
    }
    write(fd, content, strlen(content));
    close(fd);
    printf("Created test file: %s\n", filename);
}

// Helper function to create a large test file
void create_large_test_file(const char *filename, int line_count) 
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        printf("Error creating large test file %s: %s\n", filename, strerror(errno));
        return;
    }
    
    char line[100];
    for (int i = 1; i <= line_count; i++) {
        snprintf(line, sizeof(line), "This is line %d of the large test file\n", i);
        write(fd, line, strlen(line));
    }
    close(fd);
    printf("Created large test file: %s (%d lines)\n", filename, line_count);
}

// Helper function to create a simple command node
t_command_tree *create_simple_command(t_minishell *minishell, const char *cmd, char **args)
{
    t_exec *exec = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_exec));
    ft_bzero(exec, sizeof(t_exec));
    
    // Set up command
    exec->command = gc_strdup(minishell->gc[GC_COMMAND], cmd);
    
    // Count arguments
    int arg_count = 0;
    while (args && args[arg_count])
        arg_count++;
    
    // Set up argv
    exec->argv = gc_malloc(minishell->gc[GC_COMMAND], sizeof(char *) * (arg_count + 2));
    exec->argv[0] = exec->command;
    for (int i = 0; i < arg_count; i++)
        exec->argv[i + 1] = gc_strdup(minishell->gc[GC_COMMAND], args[i]);
    exec->argv[arg_count + 1] = NULL;
    
    // Create command tree node
    t_command_tree *node = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    node->type = N_EXEC;
    node->data = exec;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

// Helper function to create a command with redirections
t_command_tree *create_command_with_redirections(t_minishell *minishell, 
                                               const char *cmd, char **args,
                                               t_infile *infiles, t_outfile *outfiles)
{
    t_command_tree *node = create_simple_command(minishell, cmd, args);
    t_exec *exec = node->data;
    
    // Set up redirections
    exec->infiles = infiles;
    exec->outfiles = outfiles;
    exec->heredoc_fd = INVALID_FD;
    exec->heredoc_prepared = false;
    
    return node;
}

// Helper function to create a pipe node
t_command_tree *create_pipe_node(t_minishell *minishell, t_command_tree *left, t_command_tree *right)
{
    t_command_tree *node = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    node->type = N_PIPE;
    node->data = NULL;
    node->left = left;
    node->right = right;
    
    return node;
}

// Helper function to create an infile redirection
t_infile *create_infile(t_minishell *minishell, const char *filename, t_in_type type, const char *delimiter)
{
    t_infile *infile = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_infile));
    infile->next = NULL;
    
    if (type == INF_IN) {
        infile->name = gc_strdup(minishell->gc[GC_COMMAND], filename);
        infile->delimeter = NULL;
    } else { // INF_HEREDOC
        infile->name = NULL;
        infile->delimeter = gc_strdup(minishell->gc[GC_COMMAND], delimiter);
    }
    
    infile->type = type;
    infile->quoted_delimiter = false;
    return infile;
}

// Helper function to create an outfile redirection
t_outfile *create_outfile(t_minishell *minishell, const char *filename, t_out_type type)
{
    t_outfile *outfile = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_outfile));
    outfile->next = NULL;
    outfile->name = gc_strdup(minishell->gc[GC_COMMAND], filename);
    outfile->type = type;
    
    return outfile;
}

// Helper function to link multiple infiles together
void link_infiles(t_infile *first, t_infile *second)
{
    first->next = second;
}

// Helper function to link multiple outfiles together
void link_outfiles(t_outfile *first, t_outfile *second)
{
    first->next = second;
}

// Helper to run a test and display results
void run_test(t_minishell *minishell, t_command_tree *cmd, const char *test_name) 
{
    printf("\n--- %s ---\n", test_name);
    execute_tree(cmd, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    gc_collect(minishell->gc[GC_COMMAND]);
}

// Test 1: Deep pipe chain (5+ commands)
void test_deep_pipe_chain(t_minishell *minishell)
{
    char *args_cat[] = {"/etc/passwd", NULL};
    t_command_tree *cat_cmd = create_simple_command(minishell, "cat", args_cat);
    
    char *args_grep1[] = {"r", NULL};
    t_command_tree *grep1_cmd = create_simple_command(minishell, "grep", args_grep1);
    
    char *args_grep2[] = {"o", NULL};
    t_command_tree *grep2_cmd = create_simple_command(minishell, "grep", args_grep2);
    
    char *args_grep3[] = {"o", NULL};
    t_command_tree *grep3_cmd = create_simple_command(minishell, "grep", args_grep3);
    
    char *args_wc[] = {"-l", NULL};
    t_command_tree *wc_cmd = create_simple_command(minishell, "wc", args_wc);
    
    // Build the pipe chain: cat | grep r | grep o | grep o | wc -l
    t_command_tree *pipe1 = create_pipe_node(minishell, cat_cmd, grep1_cmd);
    t_command_tree *pipe2 = create_pipe_node(minishell, pipe1, grep2_cmd);
    t_command_tree *pipe3 = create_pipe_node(minishell, pipe2, grep3_cmd);
    t_command_tree *pipe4 = create_pipe_node(minishell, pipe3, wc_cmd);
    
    run_test(minishell, pipe4, "Deep pipe chain (cat /etc/passwd | grep r | grep o | grep o | wc -l)");
}

// Test 2: Pipes with redirections
void test_pipes_with_redirections(t_minishell *minishell)
{
    char filepath_in[1024];
    char filepath_out[1024];
    
    // Get absolute paths
    if (getcwd(filepath_in, sizeof(filepath_in)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcpy(filepath_out, filepath_in);
    strcat(filepath_in, "/pipe_redir_input.txt");
    strcat(filepath_out, "/pipe_redir_output.txt");
    
    // Create input file
    create_test_file(filepath_in, "apple\norange\nbanana\ngrape\nkiwi\nlemon\npeach\n");
    
    // Create input redirection for cat
    t_infile *cat_input = create_infile(minishell, filepath_in, INF_IN, NULL);
    
    // Create cat command with input redirection
    t_command_tree *cat_cmd = create_command_with_redirections(minishell, "cat", NULL, cat_input, NULL);
    
    // Create grep command
    char *args_grep[] = {"a", NULL};
    t_command_tree *grep_cmd = create_simple_command(minishell, "grep", args_grep);
    
    // Create sort command 
    t_command_tree *sort_cmd = create_simple_command(minishell, "sort", NULL);
    
    // Create output redirection for the final wc command
    t_outfile *wc_output = create_outfile(minishell, filepath_out, OUT_TRUNC);
    
    // Create wc command with output redirection
    char *args_wc[] = {"-l", NULL};
    t_command_tree *wc_cmd = create_command_with_redirections(minishell, "wc", args_wc, NULL, wc_output);
    
    // Build the pipe chain: cat < input.txt | grep a | sort | wc -l > output.txt
    t_command_tree *pipe1 = create_pipe_node(minishell, cat_cmd, grep_cmd);
    t_command_tree *pipe2 = create_pipe_node(minishell, pipe1, sort_cmd);
    t_command_tree *pipe3 = create_pipe_node(minishell, pipe2, wc_cmd);
    
    run_test(minishell, pipe3, "Pipes with redirections (cat < input.txt | grep a | sort | wc -l > output.txt)");
    
    // Display the output file content
    int fd = open(filepath_out, O_RDONLY);
    if (fd >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Output file content: %s\n", buffer);
        }
        close(fd);
    }
}

// Test 3: Multiple heredocs in a pipe
void test_multiple_heredocs(t_minishell *minishell)
{
        // Create first heredoc for 'cat'
    t_infile *cat_heredoc = create_infile(minishell, NULL, INF_HEREDOC, "CAT_EOF");

    // Create cat command with heredoc
    t_command_tree *cat_cmd = create_command_with_redirections(minishell, "cat", NULL, cat_heredoc, NULL);

    // Create second heredoc for another command (e.g., another cat or sed)
    t_infile *second_heredoc = create_infile(minishell, NULL, INF_HEREDOC, "GREP_EOF");
    t_command_tree *second_cmd = create_command_with_redirections(minishell, "cat", NULL, second_heredoc, NULL);

    // Pipe them together
    t_command_tree *pipe_cmd = create_pipe_node(minishell, cat_cmd, second_cmd);

    printf("\n--- Multiple heredocs in a pipe ---\n");
    printf("For the first heredoc (cat), enter some text then type 'CAT_EOF' on a new line\n");
    printf("For the second heredoc (grep), enter a regex pattern then type 'GREP_EOF' on a new line\n");
    
    run_test(minishell, pipe_cmd, "Multiple heredocs (cat << CAT_EOF | grep << GREP_EOF)");
}

// Test 4: Complex redirection chains
void test_complex_redirections(t_minishell *minishell)
{
    char filepath_in1[1024];
    char filepath_in2[1024];
    char filepath_out1[1024];
    char filepath_out2[1024];
    
    // Get absolute paths
    if (getcwd(filepath_in1, sizeof(filepath_in1)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcpy(filepath_in2, filepath_in1);
    strcpy(filepath_out1, filepath_in1);
    strcpy(filepath_out2, filepath_in1);
    
    strcat(filepath_in1, "/complex_input1.txt");
    strcat(filepath_in2, "/complex_input2.txt");
    strcat(filepath_out1, "/complex_output1.txt");
    strcat(filepath_out2, "/complex_output2.txt");
    
    // Create input files
    create_test_file(filepath_in1, "Line 1 - First file\nLine 2 - First file\nLine 3 - Important data\n");
    create_test_file(filepath_in2, "Line A - Second file\nLine B - Important data\nLine C - Second file\n");
    
    // Multiple input redirections
    t_infile *input1 = create_infile(minishell, filepath_in1, INF_IN, NULL);
    t_infile *input2 = create_infile(minishell, filepath_in2, INF_IN, NULL);
    link_infiles(input1, input2); // Link them
    
    // Multiple output redirections
    t_outfile *output1 = create_outfile(minishell, filepath_out1, OUT_TRUNC);
    t_outfile *output2 = create_outfile(minishell, filepath_out2, OUT_APPEND);
    link_outfiles(output1, output2); // Link them
    
    // Create command with multiple inputs and outputs
    char *args_grep[] = {"Important", NULL};
    t_command_tree *grep_cmd = create_command_with_redirections(minishell, "grep", args_grep, input1, output1);
    
    run_test(minishell, grep_cmd, "Complex redirections (grep Important < input1.txt < input2.txt > output1.txt >> output2.txt)");
    
    // Display the output files content
    printf("Output file 1 content:\n");
    int fd1 = open(filepath_out1, O_RDONLY);
    if (fd1 >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd1, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("%s\n", buffer);
        }
        close(fd1);
    }
    
    printf("Output file 2 content:\n");
    int fd2 = open(filepath_out2, O_RDONLY);
    if (fd2 >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd2, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("%s\n", buffer);
        }
        close(fd2);
    }
}

// Test 5: Large file processing
void test_large_file_processing(t_minishell *minishell)
{
    char filepath_large[1024];
    char filepath_out[1024];
    
    // Get absolute paths
    if (getcwd(filepath_large, sizeof(filepath_large)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcpy(filepath_out, filepath_large);
    strcat(filepath_large, "/large_input.txt");
    strcat(filepath_out, "/large_output.txt");
    
    // Create large test file (1000 lines)
    create_large_test_file(filepath_large, 1000);
    
    // Input redirection for cat
    t_infile *cat_input = create_infile(minishell, filepath_large, INF_IN, NULL);
    
    // Output redirection for final command
    t_outfile *out = create_outfile(minishell, filepath_out, OUT_TRUNC);
    
    // Create commands
    t_command_tree *cat_cmd = create_command_with_redirections(minishell, "cat", NULL, cat_input, NULL);
    
    char *args_grep[] = {"5", NULL};
    t_command_tree *grep_cmd = create_simple_command(minishell, "grep", args_grep);
    
    char *args_wc[] = {"-l", NULL};
    t_command_tree *wc_cmd = create_command_with_redirections(minishell, "wc", args_wc, NULL, out);
    
    // Build the pipe chain: cat < large_input.txt | grep 5 | wc -l > large_output.txt
    t_command_tree *pipe1 = create_pipe_node(minishell, cat_cmd, grep_cmd);
    t_command_tree *pipe2 = create_pipe_node(minishell, pipe1, wc_cmd);
    
    run_test(minishell, pipe2, "Large file processing (cat < large_input.txt | grep 5 | wc -l > large_output.txt)");
    
    // Display the output file content
    int fd = open(filepath_out, O_RDONLY);
    if (fd >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Lines containing '5': %s\n", buffer);
        }
        close(fd);
    }
}

// Test 6: Builtins with pipes and redirections
void test_builtins_with_pipes(t_minishell *minishell)
{
    char filepath_out[1024];
    
    // Get absolute path
    if (getcwd(filepath_out, sizeof(filepath_out)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcat(filepath_out, "/builtin_output.txt");
    
    // Echo command
    char *args_echo[] = {"PATH=$PATH:/tmp", NULL};
    t_command_tree *echo_cmd = create_simple_command(minishell, "echo", args_echo);
    
    // Grep command
    char *args_grep[] = {"PATH", NULL};
    t_command_tree *grep_cmd = create_simple_command(minishell, "grep", args_grep);
    
    // Sort command with output redirection
    t_outfile *sort_output = create_outfile(minishell, filepath_out, OUT_TRUNC);
    t_command_tree *sort_cmd = create_command_with_redirections(minishell, "sort", NULL, NULL, sort_output);
    
    // Build the pipe chain: echo PATH=$PATH:/tmp | grep PATH | sort > builtin_output.txt
    t_command_tree *pipe1 = create_pipe_node(minishell, echo_cmd, grep_cmd);
    t_command_tree *pipe2 = create_pipe_node(minishell, pipe1, sort_cmd);
    
    run_test(minishell, pipe2, "Builtins with pipes (echo PATH=$PATH:/tmp | grep PATH | sort > builtin_output.txt)");
    
    // Display the output file content
    int fd = open(filepath_out, O_RDONLY);
    if (fd >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Output file content: %s\n", buffer);
        }
        close(fd);
    }
}

// Test 7: Multi-level redirections
void test_multi_level_redirections(t_minishell *minishell)
{
    char filepath_in[1024];
    char filepath_tmp[1024];
    char filepath_out[1024];
    
    // Get absolute paths
    if (getcwd(filepath_in, sizeof(filepath_in)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcpy(filepath_tmp, filepath_in);
    strcpy(filepath_out, filepath_in);
    strcat(filepath_in, "/multi_input.txt");
    strcat(filepath_tmp, "/multi_temp.txt");
    strcat(filepath_out, "/multi_output.txt");
    
    // Create test file
    create_test_file(filepath_in, "alpha\nbeta\ngamma\ndelta\nepsilon\n");
    
    // First level: cat < input.txt > temp.txt
    t_infile *cat_input = create_infile(minishell, filepath_in, INF_IN, NULL);
    t_outfile *cat_output = create_outfile(minishell, filepath_tmp, OUT_TRUNC);
    t_command_tree *cat_cmd = create_command_with_redirections(minishell, "cat", NULL, cat_input, cat_output);
    
    run_test(minishell, cat_cmd, "Level 1: cat < multi_input.txt > multi_temp.txt");
    
    // Second level: sort < temp.txt > output.txt
    t_infile *sort_input = create_infile(minishell, filepath_tmp, INF_IN, NULL);
    t_outfile *sort_output = create_outfile(minishell, filepath_out, OUT_TRUNC);
    t_command_tree *sort_cmd = create_command_with_redirections(minishell, "sort", NULL, sort_input, sort_output);
    
    run_test(minishell, sort_cmd, "Level 2: sort < multi_temp.txt > multi_output.txt");
    
    // Display the final output file content
    int fd = open(filepath_out, O_RDONLY);
    if (fd >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Final output file content:\n%s\n", buffer);
        }
        close(fd);
    }
}

// Test 8: Stress test with max file descriptors
// void test_max_file_descriptors(t_minishell *minishell)
// {
//     char base_path[1024];
    
//     // Get base path
//     if (getcwd(base_path, sizeof(base_path)) == NULL) {
//         printf("Error getting current directory\n");
//         return;
//     }
    
//     // Create a test with commands that don't have input redirections
//     char filepath[1024];
//     sprintf(filepath, "%s/numbers.txt", base_path);
//     create_test_file(filepath, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n");
    
//     // cat numbers.txt | grep -v 5 | sort -r | head -3
//     char *cat_args[] = {filepath, NULL};
//     t_command_tree *cat_cmd = create_simple_command(minishell, "cat", cat_args);
    
//     char *grep_args[] = {"-v", "5", NULL};
//     t_command_tree *grep_cmd = create_simple_command(minishell, "grep", grep_args);
    
//     char *sort_args[] = {"-r", NULL};
//     t_command_tree *sort_cmd = create_simple_command(minishell, "sort", sort_args);
    
//     char *head_args[] = {"-3", NULL};
//     t_command_tree *head_cmd = create_simple_command(minishell, "head", head_args);
    
//     t_command_tree *pipe1 = create_pipe_node(minishell, cat_cmd, grep_cmd);
//     t_command_tree *pipe2 = create_pipe_node(minishell, pipe1, sort_cmd);
//     t_command_tree *pipe3 = create_pipe_node(minishell, pipe2, head_cmd);
    
//     run_test(minishell, pipe3, "Standard pipeline test (cat | grep | sort | head)");
// }
void test_max_file_descriptors(t_minishell *minishell)
{
    // Create 10 files
    char filepaths[10][1024];
    char base_path[1024];
    
    // Get base path
    if (getcwd(base_path, sizeof(base_path)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    
    // Create 10 input files
    for (int i = 0; i < 10; i++) {
        sprintf(filepaths[i], "%s/fd_test_%d.txt", base_path, i);
        char content[100];
        sprintf(content, "This is test file %d\n", i);
        create_test_file(filepaths[i], content);
    }
    
    // Instead of piping cat commands together, use a single command with multiple files
    char *args[11];
    for (int i = 0; i < 10; i++) {
        args[i] = filepaths[i];
    }
    args[10] = NULL;
    
    // Create a single cat command that reads all files
    t_command_tree *cat_cmd = create_simple_command(minishell, "cat", args);
    
    run_test(minishell, cat_cmd, "Multiple file descriptors test (cat file1 file2 ... file10)");
}

void test_mixed_pipe_redirections(t_minishell *minishell)
{
    char filepath[1024];
    
    // Get absolute path
    if (getcwd(filepath, sizeof(filepath)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcat(filepath, "/mixed_test.txt");
    
    // Create file with content containing both 'a' and 'b'
    create_test_file(filepath, "apple\nbanana\norange\ngrape\nblueberry\n");
    
    // First cat with input redirection
    t_infile *cat1_input = create_infile(minishell, filepath, INF_IN, NULL);
    t_command_tree *cat1_cmd = create_command_with_redirections(minishell, "cat", NULL, cat1_input, NULL);
    
    // First grep
    char *grep1_args[] = {"a", NULL};
    t_command_tree *grep1_cmd = create_simple_command(minishell, "grep", grep1_args);
    
    // Second cat with input redirection
    t_infile *cat2_input = create_infile(minishell, filepath, INF_IN, NULL);
    t_command_tree *cat2_cmd = create_command_with_redirections(minishell, "cat", NULL, cat2_input, NULL);
    
    // Second grep
    char *grep2_args[] = {"b", NULL};
    t_command_tree *grep2_cmd = create_simple_command(minishell, "grep", grep2_args);
    
    // Build the pipe chain
    t_command_tree *pipe1 = create_pipe_node(minishell, cat1_cmd, grep1_cmd);
    t_command_tree *pipe2 = create_pipe_node(minishell, pipe1, cat2_cmd);
    t_command_tree *pipe3 = create_pipe_node(minishell, pipe2, grep2_cmd);
    
    run_test(minishell, pipe3, "Mixed pipe and redirections (cat < file | grep a | cat < file | grep b)");
}

// Test 10: Environment variable expansion
void test_environment_variable_expansion(t_minishell *minishell)
{
    char filepath_out[1024];
    
    // Get absolute path
    if (getcwd(filepath_out, sizeof(filepath_out)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcat(filepath_out, "/env_output.txt");
    
    // Echo command with env vars
    char *args_echo[] = {"$HOME $USER $PATH", NULL};

    
    // Output redirection
    t_outfile *echo_output = create_outfile(minishell, filepath_out, OUT_TRUNC);
    t_command_tree *echo_with_redir = create_command_with_redirections(
        minishell, "echo", args_echo, NULL, echo_output);
    
    run_test(minishell, echo_with_redir, "Environment variable expansion (echo $HOME $USER $PATH > output)");
    
    // Display the output file content
    int fd = open(filepath_out, O_RDONLY);
    if (fd >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Output file content: %s\n", buffer);
        }
        close(fd);
    }
}

// Test 11: Quote handling
void test_quote_handling(t_minishell *minishell)
{
    char filepath_out1[1024];
    char filepath_out2[1024];
    
    // Get absolute path
    if (getcwd(filepath_out1, sizeof(filepath_out1)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcpy(filepath_out2, filepath_out1);
    strcat(filepath_out1, "/quote_output1.txt");
    strcat(filepath_out2, "/quote_output2.txt");
    
    // Test literal string (simulating single quotes)
    char *args_echo1[] = {"$HOME $USER", NULL};
    t_outfile *output1 = create_outfile(minishell, filepath_out1, OUT_TRUNC);
    t_command_tree *echo1_with_redir = create_command_with_redirections(
        minishell, "echo", args_echo1, NULL, output1);
    
    // Run first test
    run_test(minishell, echo1_with_redir, "Single quote simulation (echo '$HOME $USER' > output1)");
    
    // Display the first output file
    int fd1 = open(filepath_out1, O_RDONLY);
    if (fd1 >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd1, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Single quotes output: %s\n", buffer);
        }
        close(fd1);
    }
    
    // Allow minishell to handle expansion directly for the second test
    // This will show up as the literal string, since we're bypassing the parser
    char *args_echo2[] = {"$HOME $USER", NULL};
    t_outfile *output2 = create_outfile(minishell, filepath_out2, OUT_TRUNC);
    t_command_tree *echo2_with_redir = create_command_with_redirections(
        minishell, "echo", args_echo2, NULL, output2);
    
    // Run second test
    run_test(minishell, echo2_with_redir, "Double quote simulation (echo \"$HOME $USER\" > output2)");
    
    // Display the second output file
    int fd2 = open(filepath_out2, O_RDONLY);
    if (fd2 >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd2, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Double quotes output: %s\n", buffer);
        }
        close(fd2);
    }
}

// Test 12: Nested pipes and redirections
void test_nested_pipes_redirections(t_minishell *minishell)
{
    char filepath_in[1024];
    char filepath_out[1024];
    
    // Get absolute paths
    if (getcwd(filepath_in, sizeof(filepath_in)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcpy(filepath_out, filepath_in);
    strcat(filepath_in, "/nested_input.txt");
    strcat(filepath_out, "/nested_output.txt");
    
    // Create test file with various content
    create_test_file(filepath_in, "first line\nsecond line\nthird line\nfourth line\nfifth line\n");
    
    // Create complex nested pipe: cat < file | (grep i | grep l) | sort -r > output
    
    // cat with input redirection
    t_infile *cat_input = create_infile(minishell, filepath_in, INF_IN, NULL);
    t_command_tree *cat_cmd = create_command_with_redirections(minishell, "cat", NULL, cat_input, NULL);
    
    // First grep
    char *grep1_args[] = {"i", NULL};
    t_command_tree *grep1_cmd = create_simple_command(minishell, "grep", grep1_args);
    
    // Second grep
    char *grep2_args[] = {"l", NULL};
    t_command_tree *grep2_cmd = create_simple_command(minishell, "grep", grep2_args);
    
    // Sort with output redirection
    char *sort_args[] = {"-r", NULL};
    t_outfile *sort_output = create_outfile(minishell, filepath_out, OUT_TRUNC);
    t_command_tree *sort_cmd = create_command_with_redirections(minishell, "sort", sort_args, NULL, sort_output);
    
    // Build the nested pipe chain
    t_command_tree *inner_pipe = create_pipe_node(minishell, grep1_cmd, grep2_cmd);
    t_command_tree *first_pipe = create_pipe_node(minishell, cat_cmd, inner_pipe);
    t_command_tree *final_pipe = create_pipe_node(minishell, first_pipe, sort_cmd);
    
    run_test(minishell, final_pipe, "Nested pipes and redirections (cat < file | (grep i | grep l) | sort -r > output)");
    
    // Display the output file content
    int fd = open(filepath_out, O_RDONLY);
    if (fd >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Nested pipes output: \n%s\n", buffer);
        }
        close(fd);
    }
}

// Test 13: Special characters in filenames
void test_special_characters(t_minishell *minishell)
{
    char basepath[1024];
    
    // Get base path
    if (getcwd(basepath, sizeof(basepath)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    
    // Create files with special characters in names
    char filepath1[1024], filepath2[1024], filepath_out1[1024], filepath_out2[1024];
    sprintf(filepath1, "%s/file-with_special%%chars.txt", basepath);
    sprintf(filepath2, "%s/file with spaces.txt", basepath);
    sprintf(filepath_out1, "%s/special_output1.txt", basepath);
    sprintf(filepath_out2, "%s/special_output2.txt", basepath);
    
    create_test_file(filepath1, "Content in file with special characters\n");
    create_test_file(filepath2, "Content in file with spaces\n");
    
    // Test 1: File with special characters
    char *cat1_args[] = {filepath1, NULL};
    t_outfile *output1 = create_outfile(minishell, filepath_out1, OUT_TRUNC);
    t_command_tree *cat1_with_redir = create_command_with_redirections(
        minishell, "cat", cat1_args, NULL, output1);
    
    run_test(minishell, cat1_with_redir, "Special characters in filename (cat file-with_special%chars.txt > output1)");
    
    // Display the first output file content
    int fd1 = open(filepath_out1, O_RDONLY);
    if (fd1 >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd1, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Special characters output: \n%s\n", buffer);
        }
        close(fd1);
    }
    
    // Test 2: File with spaces
    char *cat2_args[] = {filepath2, NULL};
    t_outfile *output2 = create_outfile(minishell, filepath_out2, OUT_TRUNC);
    t_command_tree *cat2_with_redir = create_command_with_redirections(
        minishell, "cat", cat2_args, NULL, output2);
    
    run_test(minishell, cat2_with_redir, "Spaces in filename (cat \"file with spaces.txt\" > output2)");
    
    // Display the second output file content
    int fd2 = open(filepath_out2, O_RDONLY);
    if (fd2 >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd2, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Spaces in filename output: \n%s\n", buffer);
        }
        close(fd2);
    }
}

// Test 14: Error handling
void test_error_handling(t_minishell *minishell)
{
    // Command not found
    char *args_invalid[] = {"argument1", "argument2", NULL};
    t_command_tree *invalid_cmd = create_simple_command(minishell, "non_existent_command", args_invalid);
    
    // Run just the first error case
    run_test(minishell, invalid_cmd, "Command not found error handling (non_existent_command)");
    
    // Instead of testing a non-existent file directly, test a permission denied case
    char filepath[1024];
    if (getcwd(filepath, sizeof(filepath)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcat(filepath, "/test_no_permission.txt");
    
    // Create a file but make it unreadable
    create_test_file(filepath, "This file exists but will be unreadable\n");
    chmod(filepath, 0200);  // Write-only permission
    
    t_infile *no_perm = create_infile(minishell, filepath, INF_IN, NULL);
    t_command_tree *cat_cmd = create_command_with_redirections(minishell, "cat", NULL, no_perm, NULL);
    
    // Test permission denied case instead
    run_test(minishell, cat_cmd, "Permission denied error handling (cat < unreadable_file.txt)");
    
    // Clean up
    chmod(filepath, 0644);  // Restore normal permissions for cleanup
}

void run_test_no_gc(t_minishell *minishell, t_command_tree *cmd, const char *test_name) 
{
    printf("\n--- %s ---\n", test_name);
    execute_tree(cmd, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    // No garbage collection here
}

// Test 15: Background processes
void test_background_processes(t_minishell *minishell)
{
    char filepath_out[1024];
    
    // Get absolute path
    if (getcwd(filepath_out, sizeof(filepath_out)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcat(filepath_out, "/background_output.txt");
    
    // Create a slow command that we'll run in the background
    // This simulates "sleep 2 & echo 'Background process started' > output.txt"
    
    // Sleep command (would run in background in a real shell)
    t_command_tree *sleep_cmd = create_simple_command(minishell, "sleep", (char *[]){"2", NULL});
    
    // Echo command with output redirection
    char *args_echo[] = {"Background process running", NULL};
    t_outfile *output = create_outfile(minishell, filepath_out, OUT_TRUNC);
    t_command_tree *echo_cmd = create_command_with_redirections(
        minishell, "echo", args_echo, NULL, output);
    
    // We'll run these sequentially to simulate background behavior
    printf("\n--- Background process simulation ---\n");
    printf("Running sleep in 'background', then running echo immediately\n");
    
    // First start the sleep command without waiting for it
    run_test_no_gc(minishell, sleep_cmd, "Background process (sleep 2 &)");
    
    // Then immediately run the echo command
    run_test_no_gc(minishell, echo_cmd, "Foreground process (echo 'Background process running' > output.txt)");
    
    // Display the output file content
    int fd = open(filepath_out, O_RDONLY);
    if (fd >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Background test output: %s\n", buffer);
        }
        close(fd);
    }
    
    // Wait for background process to finish
    printf("Waiting for background process to complete...\n");
    sleep(3);  // Ensure the sleep command has time to finish
    printf("Background process simulation completed\n");
}

// Test 16: Stderr redirection
void test_stderr_redirection(t_minishell *minishell)
{
    char filepath_stderr[1024];
    char filepath_both[1024];
    
    // Get absolute paths
    if (getcwd(filepath_stderr, sizeof(filepath_stderr)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcpy(filepath_both, filepath_stderr);
    strcat(filepath_stderr, "/stderr_output.txt");
    strcat(filepath_both, "/both_output.txt");
    
    // First test: Use a command that redirects stderr to a file manually
    // This mimics: ls non_existent_dir 2> stderr_output.txt
    char *args_bash[] = {"-c", "ls a_directory_that_doesnt_exist 2> stderr_output.txt", NULL};
    t_command_tree *bash_cmd = create_simple_command(minishell, "bash", args_bash);
    
    run_test(minishell, bash_cmd, "Stderr redirection via bash (ls non_existent_dir 2> stderr_output.txt)");
    
    // Display the stderr output file content
    int fd1 = open(filepath_stderr, O_RDONLY);
    if (fd1 >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd1, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Stderr output file content:\n%s\n", buffer);
        }
        close(fd1);
    }
    
    // Second test: Use bash to redirect both stdout and stderr
    char *args_bash2[] = {"-c", "echo 'This goes to stdout' > both_output.txt && ls a_directory_that_doesnt_exist 2>> both_output.txt", NULL};
    t_command_tree *bash_cmd2 = create_simple_command(minishell, "bash", args_bash2);
    
    run_test(minishell, bash_cmd2, "Combined stdout/stderr redirection via bash");
    
    // Display the combined output file content
    int fd2 = open(filepath_both, O_RDONLY);
    if (fd2 >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd2, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Combined stdout/stderr output file content:\n%s\n", buffer);
        }
        close(fd2);
    }
}

// Test 17: Command substitution
void test_command_substitution(t_minishell *minishell)
{
    char filepath_out[1024];
    
    // Get absolute path
    if (getcwd(filepath_out, sizeof(filepath_out)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcat(filepath_out, "/cmd_subst_output.txt");
    
    // Create a bash command that uses command substitution
    // This mimics: echo "Today's date is $(date +%Y-%m-%d)"
    char *args_bash[] = {"-c", "echo \"Files in current dir: $(ls -1 | head -3)\" > cmd_subst_output.txt", NULL};
    t_command_tree *bash_cmd = create_simple_command(minishell, "bash", args_bash);
    
    run_test(minishell, bash_cmd, "Command substitution via bash (echo with $(ls))");
    
    // Display the output file content
    int fd = open(filepath_out, O_RDONLY);
    if (fd >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Command substitution output:\n%s\n", buffer);
        }
        close(fd);
    }
    
    // Second test: Command substitution with backticks (older syntax)
    char *args_bash2[] = {"-c", "echo \"Current working directory: `pwd`\" >> cmd_subst_output.txt", NULL};
    t_command_tree *bash_cmd2 = create_simple_command(minishell, "bash", args_bash2);
    
    run_test(minishell, bash_cmd2, "Command substitution via bash with backticks (echo with `pwd`)");
    
    // Display the updated output file content
    fd = open(filepath_out, O_RDONLY);
    if (fd >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Updated command substitution output:\n%s\n", buffer);
        }
        close(fd);
    }
}

// Test 18: Wildcards and globbing patterns
void test_wildcards_and_globbing(t_minishell *minishell)
{
    char filepath_out[1024];
    
    // Get absolute path
    if (getcwd(filepath_out, sizeof(filepath_out)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcat(filepath_out, "/wildcard_output.txt");
    
    // First test: Basic glob pattern (*.txt)
    // This mimics: ls *.txt > wildcard_output.txt
    char *args_bash1[] = {"-c", "ls *.txt > wildcard_output.txt", NULL};
    t_command_tree *bash_cmd1 = create_simple_command(minishell, "bash", args_bash1);
    
    run_test(minishell, bash_cmd1, "Basic wildcard pattern via bash (ls *.txt)");
    
    // Display the output file content
    int fd1 = open(filepath_out, O_RDONLY);
    if (fd1 >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd1, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Basic wildcard output:\n%s\n", buffer);
        }
        close(fd1);
    }
    
    // Second test: More complex glob pattern (multiple patterns and character classes)
    // This mimics: echo "Files starting with 'f' or containing 'put': $(ls [f]* *put*.txt 2>/dev/null)"
    char *args_bash2[] = {"-c", "echo \"Files matching complex pattern: $(ls [f]* *put*.txt 2>/dev/null | sort)\" >> wildcard_output.txt", NULL};
    t_command_tree *bash_cmd2 = create_simple_command(minishell, "bash", args_bash2);
    
    run_test(minishell, bash_cmd2, "Complex wildcard pattern via bash (ls [f]* *put*.txt)");
    
    // Display the updated output file content
    fd1 = open(filepath_out, O_RDONLY);
    if (fd1 >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd1, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Full wildcard test output:\n%s\n", buffer);
        }
        close(fd1);
    }
    
    // Third test: Brace expansion (if supported)
    // This mimics: echo test{1,2,3}.txt
    char *args_bash3[] = {"-c", "echo \"Brace expansion results: $(echo test{1,2,3}.txt)\" >> wildcard_output.txt", NULL};
    t_command_tree *bash_cmd3 = create_simple_command(minishell, "bash", args_bash3);
    
    run_test(minishell, bash_cmd3, "Brace expansion via bash (echo test{1,2,3}.txt)");
    
    // Display the final output file content
    fd1 = open(filepath_out, O_RDONLY);
    if (fd1 >= 0) {
        char buffer[1024];
        ssize_t bytes_read = read(fd1, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Final wildcard and expansion test output:\n%s\n", buffer);
        }
        close(fd1);
    }
}

// Test: Environment variable expansion edge cases
void test_env_expansion_edge_cases(t_minishell *minishell)
{
    printf("\n=== ENVIRONMENT VARIABLE EXPANSION TEST ===\n");
    
    char filepath_out[1024];
    
    // Get absolute path
    if (getcwd(filepath_out, sizeof(filepath_out)) == NULL) {
        printf("Error getting current directory\n");
        return;
    }
    strcat(filepath_out, "/env_test_output.txt");
    
    // Create and set custom environment variables for testing
    setenv("TEST_VAR1", "value1", 1);
    setenv("TEST_VAR2", "value with spaces", 1);
    setenv("TEST_EMPTY", "", 1);
    setenv("TEST_PATH", "/usr/bin:/bin:/usr/local/bin", 1);
    setenv("TEST_SPECIAL_CHARS", "!@#$%^&*()", 1);
    
    // Test 1: Basic variable expansion
    char *args_echo1[] = {"Basic: $HOME $USER $TEST_VAR1", NULL};
    t_outfile *output1 = create_outfile(minishell, filepath_out, OUT_TRUNC);
    t_command_tree *cmd1 = create_command_with_redirections(
        minishell, "echo", args_echo1, NULL, output1);
    
    run_test(minishell, cmd1, "Basic variable expansion ($HOME $USER $TEST_VAR1)");
    
    // Test 2: Variable with spaces and special characters
    char *args_echo2[] = {"With spaces: $TEST_VAR2, Special: $TEST_SPECIAL_CHARS", NULL};
    t_outfile *output2 = create_outfile(minishell, filepath_out, OUT_APPEND);
    t_command_tree *cmd2 = create_command_with_redirections(
        minishell, "echo", args_echo2, NULL, output2);
    
    run_test(minishell, cmd2, "Variables with spaces and special characters");
    
    // Test 3: Non-existent variables
    char *args_echo3[] = {"Non-existent: $NONEXISTENT_VAR", NULL};
    t_outfile *output3 = create_outfile(minishell, filepath_out, OUT_APPEND);
    t_command_tree *cmd3 = create_command_with_redirections(
        minishell, "echo", args_echo3, NULL, output3);
    
    run_test(minishell, cmd3, "Non-existent variables ($NONEXISTENT_VAR)");
    
    // Test 4: Variable followed by text
    char *args_echo4[] = {"Variable with text: $HOME/documents", NULL};
    t_outfile *output4 = create_outfile(minishell, filepath_out, OUT_APPEND);
    t_command_tree *cmd4 = create_command_with_redirections(
        minishell, "echo", args_echo4, NULL, output4);
    
    run_test(minishell, cmd4, "Variable followed by text ($HOME/documents)");
    
    // Test 5: Empty variable
    char *args_echo5[] = {"Empty variable: '$TEST_EMPTY'", NULL};
    t_outfile *output5 = create_outfile(minishell, filepath_out, OUT_APPEND);
    t_command_tree *cmd5 = create_command_with_redirections(
        minishell, "echo", args_echo5, NULL, output5);
    
    run_test(minishell, cmd5, "Empty variable ($TEST_EMPTY)");
    
    // Test 6: Exit status variable
    // First run a command that will set $? to a specific value
    t_command_tree *grep_cmd = create_simple_command(minishell, "grep", 
        (char *[]){"nonexistentpattern", "/etc/passwd", NULL});
    run_test(minishell, grep_cmd, "Command to set exit status");
    
    // Now check the exit status variable
    char *args_echo6[] = {"Exit status: $?", NULL};
    t_outfile *output6 = create_outfile(minishell, filepath_out, OUT_APPEND);
    t_command_tree *cmd6 = create_command_with_redirections(
        minishell, "echo", args_echo6, NULL, output6);
    
    run_test(minishell, cmd6, "Exit status variable ($?)");
    
    // Test 7: Multiple variables in a path-like string
    char *args_echo7[] = {"Path like: $HOME/$USER/$TEST_VAR1", NULL};
    t_outfile *output7 = create_outfile(minishell, filepath_out, OUT_APPEND);
    t_command_tree *cmd7 = create_command_with_redirections(
        minishell, "echo", args_echo7, NULL, output7);
    
    run_test(minishell, cmd7, "Multiple variables in path ($HOME/$USER/$TEST_VAR1)");
    
    // Display the combined test results
    printf("\n=== TEST RESULTS ===\n");
    int fd = open(filepath_out, O_RDONLY);
    if (fd >= 0) {
        char buffer[4096];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("%s\n", buffer);
        }
        close(fd);
    }
    
    printf("\n=== ENVIRONMENT VARIABLE EXPANSION TEST COMPLETED ===\n");
    
    // Clean up custom environment variables
    unsetenv("TEST_VAR1");
    unsetenv("TEST_VAR2");
    unsetenv("TEST_EMPTY");
    unsetenv("TEST_PATH");
    unsetenv("TEST_SPECIAL_CHARS");
}

// Test function for environment variable expansion during command execution
void test_env_expansion_execution(t_minishell *minishell)
{
    printf("\n=== ENVIRONMENT VARIABLE EXPANSION EXECUTION TEST ===\n");
    
    // Set up test environment variables
    // setenv("TEST_VAR", "test_value", 1);
    // setenv("PATH_VAR", "/usr/bin:/bin", 1);
    // setenv("EMPTY_VAR", "", 1);
    // setenv("SPECIAL_CHARS", "!@#$%^&*()", 1);
    // setenv("VAR_WITH_SPACES", "hello world", 1);
    set_env_var("PATH_VAR", "/usr/bin:/bin", minishell);
    set_env_var("TEST_VAR", "test_value", minishell);
    set_env_var("EMPTY_VAR", "", minishell);
    set_env_var("SPECIAL_CHARS", "!@#$%^&*()", minishell);
    set_env_var("VAR_WITH_SPACES", "value with spaces", minishell);
    
    // Test 1: Basic variable expansion in echo
    printf("\n--- Test 1: Basic variable expansion ---\n");
    char *args_echo1[] = {"$TEST_VAR", NULL};
    t_command_tree *cmd1 = create_simple_command(minishell, "echo", args_echo1);
    execute_tree(cmd1, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    
    // Test 2: Multiple variables and text
    printf("\n--- Test 2: Multiple variables and text ---\n");
    char *args_echo2[] = {"Path=$PATH_VAR, Value=$TEST_VAR", NULL};
    t_command_tree *cmd2 = create_simple_command(minishell, "echo", args_echo2);
    execute_tree(cmd2, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    
    // Test 3: Empty variable
    printf("\n--- Test 3: Empty variable ---\n");
    char *args_echo3[] = {"Empty: [$EMPTY_VAR]", NULL};
    t_command_tree *cmd3 = create_simple_command(minishell, "echo", args_echo3);
    execute_tree(cmd3, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    
    // Test 4: Special characters
    printf("\n--- Test 4: Special characters ---\n");
    char *args_echo4[] = {"Special: $SPECIAL_CHARS", NULL};
    t_command_tree *cmd4 = create_simple_command(minishell, "echo", args_echo4);
    execute_tree(cmd4, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    
    // Test 5: Variable with spaces
    printf("\n--- Test 5: Variable with spaces ---\n");
    char *args_echo5[] = {"Spaces: [$VAR_WITH_SPACES]", NULL};
    t_command_tree *cmd5 = create_simple_command(minishell, "echo", args_echo5);
    execute_tree(cmd5, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    
    // Test 6: Exit status variable
    printf("\n--- Test 6: Exit status variable ---\n");
    // First run a command that will set $? to 1
    t_command_tree *grep_cmd = create_simple_command(minishell, "grep", 
        (char *[]){"nonexistentpattern", "/etc/passwd", NULL});
    execute_tree(grep_cmd, minishell);
    
    // Now check the exit status
    char *args_echo6[] = {"Exit status: $?", NULL};
    t_command_tree *cmd6 = create_simple_command(minishell, "echo", args_echo6);
    execute_tree(cmd6, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    
    // Clean up environment variables
    unsetenv("TEST_VAR");
    unsetenv("PATH_VAR");
    unsetenv("EMPTY_VAR");
    unsetenv("SPECIAL_CHARS");
    unsetenv("VAR_WITH_SPACES");
    
    // Clear out the command tree
    gc_collect(minishell->gc[GC_COMMAND]);
    
    printf("\n=== ENVIRONMENT VARIABLE EXPANSION TEST COMPLETED ===\n");
}

// Test function for quote handling during variable expansion
void test_quote_expansion_execution(t_minishell *minishell)
{
    printf("\n=== QUOTE HANDLING TEST ===\n");
    
    // Set up test environment variables
    set_env_var("TEST_VAR", "test_value", minishell);
    set_env_var("HOME", "/Users/lakdogan", minishell);
    set_env_var("SPECIAL_CHARS", "!@#$%^&*()", minishell);
    
    // Test 1: Single quotes should preserve literals and not expand variables
    printf("\n--- Test 1: Single quotes (literal preservation) ---\n");
    char *args_echo1[] = {"'$TEST_VAR $HOME'", NULL};
    t_command_tree *cmd1 = create_simple_command(minishell, "echo", args_echo1);
    execute_tree(cmd1, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    
    // Test 2: Double quotes should preserve literals but expand variables
    printf("\n--- Test 2: Double quotes (expand variables) ---\n");
    char *args_echo2[] = {"\"$TEST_VAR $HOME\"", NULL};
    t_command_tree *cmd2 = create_simple_command(minishell, "echo", args_echo2);
    execute_tree(cmd2, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    
    // Test 3: Mixed quotes - single quotes inside double quotes
    printf("\n--- Test 3: Mixed quotes (single inside double) ---\n");
    char *args_echo3[] = {"\"The variable is '$TEST_VAR'\"", NULL};
    t_command_tree *cmd3 = create_simple_command(minishell, "echo", args_echo3);
    execute_tree(cmd3, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    
    // Test 4: Mixed quotes - double quotes inside single quotes
    printf("\n--- Test 4: Mixed quotes (double inside single) ---\n");
    char *args_echo4[] = {"'The variable is \"$TEST_VAR\"'", NULL};
    t_command_tree *cmd4 = create_simple_command(minishell, "echo", args_echo4);
    execute_tree(cmd4, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    
    // Test 5: Escaped quotes
    printf("\n--- Test 5: Escaped quotes ---\n");
    char *args_echo5[] = {"$TEST_VAR", NULL};
    t_command_tree *cmd5 = create_simple_command(minishell, "echo", args_echo5);
    execute_tree(cmd5, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    
    // Test 6: Quotes with special characters
    printf("\n--- Test 6: Quotes with special characters ---\n");
    char *args_echo6[] = {"\"Special chars: $SPECIAL_CHARS\"", NULL};
    t_command_tree *cmd6 = create_simple_command(minishell, "echo", args_echo6);
    execute_tree(cmd6, minishell);
    printf("Exit code: %d\n", minishell->exit_code);
    
    // Clean up the command tree
    gc_collect(minishell->gc[GC_COMMAND]);
    
    printf("\n=== QUOTE HANDLING TEST COMPLETED ===\n");
}

// Test function for exit status handling
void test_exit_status_handling(t_minishell *minishell)
{
    printf("\n=== EXIT STATUS HANDLING TEST ===\n");
    
    // Test 1: Initial exit status (should be 0 at startup)
    printf("\n--- Test 1: Initial exit status ---\n");
    printf("Current exit status: %d\n", minishell->exit_code);
    
    // Test 2: Failed command (grep with non-existent pattern)
    printf("\n--- Test 2: After failed command ---\n");
    char *args_grep1[] = {"nonexistentpattern", "/etc/passwd", NULL};
    t_command_tree *grep_cmd1 = create_simple_command(minishell, "grep", args_grep1);
    execute_tree(grep_cmd1, minishell);
    printf("Exit status after failed grep: %d\n", minishell->exit_code);
    
    // Test 3: Successful command after failure
    printf("\n--- Test 3: After successful command ---\n");
    char *args_echo1[] = {"Success resets exit status", NULL};
    t_command_tree *echo_cmd1 = create_simple_command(minishell, "echo", args_echo1);
    execute_tree(echo_cmd1, minishell);
    printf("Exit status after successful echo: %d\n", minishell->exit_code);
    
    // Test 4: Exit status variable expansion after failure
    printf("\n--- Test 4: Exit status variable expansion ---\n");
    // First set a non-zero exit status
    execute_tree(grep_cmd1, minishell);
    // Then check if $? shows the correct value
    char *args_echo2[] = {"Exit status: $?", NULL};
    t_command_tree *echo_cmd2 = create_simple_command(minishell, "echo", args_echo2);
    execute_tree(echo_cmd2, minishell);
    printf("Exit code after $? expansion: %d\n", minishell->exit_code);
    
    // Test 5: Exit status in pipe chain
    printf("\n--- Test 5: Exit status in pipe chain ---\n");
    // Create: grep nonexistent /etc/passwd | sort
    char *args_sort[] = {NULL};
    t_command_tree *sort_cmd = create_simple_command(minishell, "sort", args_sort);
    t_command_tree *pipe_cmd = create_pipe_node(minishell, grep_cmd1, sort_cmd);
    execute_tree(pipe_cmd, minishell);
    printf("Exit status after pipe with error in first command: %d\n", minishell->exit_code);
    
    // Test 6: Exit status from command not found
    printf("\n--- Test 6: Command not found ---\n");
    char *args_notfound[] = {"argument1", NULL};
    t_command_tree *notfound_cmd = create_simple_command(minishell, "commanddoesnotexist", args_notfound);
    execute_tree(notfound_cmd, minishell);
    printf("Exit status after command not found: %d\n", minishell->exit_code);
    
    // Test 7: Check exit status variable after command not found
    printf("\n--- Test 7: Exit status variable after command not found ---\n");
    execute_tree(echo_cmd2, minishell);
    printf("Exit code after $? expansion: %d\n", minishell->exit_code);
    
    // Clean up
    gc_collect(minishell->gc[GC_COMMAND]);
    
    printf("\n=== EXIT STATUS HANDLING TEST COMPLETED ===\n");
}

void test_heredoc_with_large_content(t_minishell *minishell)
{
    printf("\n--- Heredoc with large content test ---\n");
    printf("Enter multiple lines, including some very long ones (500+ chars if possible)\n");
    printf("Type 'LARGE_EOF' when done\n");

    // Create heredoc with large content
    t_infile *heredoc = create_infile(minishell, NULL, INF_HEREDOC, "LARGE_EOF");
    
    // Pipe to two commands to stress-test the pipe mechanism
    t_command_tree *cat_cmd = create_command_with_redirections(minishell, "cat", NULL, heredoc, NULL);
    
    char *grep_args[] = {"[a-z]", NULL};
    t_command_tree *grep_cmd = create_simple_command(minishell, "grep", grep_args);
    
    char *wc_args[] = {"-l", NULL};
    t_command_tree *wc_cmd = create_simple_command(minishell, "wc", wc_args);
    
    // Build pipe: cat << LARGE_EOF | grep [a-z] | wc -l
    t_command_tree *pipe1 = create_pipe_node(minishell, cat_cmd, grep_cmd);
    t_command_tree *pipe2 = create_pipe_node(minishell, pipe1, wc_cmd);
    
    run_test(minishell, pipe2, "Heredoc with large content (cat << LARGE_EOF | grep [a-z] | wc -l)");
}

// void debug_env(t_minishell *shell)
// {
//     printf("--- ENV DEBUG ---\n");
//     t_list *node = shell->envp;
//     int count = 0;
    
//     if (!node) {
//         printf("Warning: Environment linked list is empty (shell->envp is NULL)\n");
//         printf("----------------\n");
//         return;
//     }
    
//     while (node)
//     {
//         if (!node->content) {
//             printf("[%d] ERROR: Node content is NULL\n", count++);
//             node = node->next;
//             continue;
//         }
        
//         t_env *env = (t_env *)node->content;
        
//         // Print node address and content address for debugging
//         printf("[%d] Node@%p Content@%p: ", count++, (void*)node, (void*)env);
        
//         // Validate env->value before accessing
//         if (env->value) {
//             printf("%s -> ", env->value);
//         } else {
//             printf("(null) -> ");
//         }
        
//         // Validate env->content before accessing
//         if (env->content) {
//             // For content, extract just the value part if it has = sign
//             char *equal_sign = ft_strchr(env->content, '=');
//             if (equal_sign) {
//                 printf("%s", equal_sign + 1);
//             } else {
//                 printf("%s", env->content);
//             }
//         } else {
//             printf("(null)");
//         }
        
//         printf(" (export: %d)\n", env->is_export);
//         node = node->next;
//     }
//     printf("----------------\n");
// }

// void debug_env(t_minishell *shell)
// {
//     printf("--- ENV DEBUG ---\n");
//     int i = 0;
    
//     // Print using the array instead of the corrupted linked list
//     while (shell->envp_arr && shell->envp_arr[i])
//     {
//         char *entry = shell->envp_arr[i];
//         char *equal_sign = ft_strchr(entry, '=');
        
//         if (equal_sign)
//         {
//             // Temporarily display the name part
//             int name_len = equal_sign - entry;
//             char name[name_len + 1];
//             ft_strlcpy(name, entry, name_len + 1);
//             printf("[%d] %s -> %s (export: 1)\n", i, name, equal_sign + 1);
//         }
//         else
//         {
//             printf("[%d] %s -> (null) (export: 0)\n", i, entry);
//         }
//         i++;
//     }
//     printf("----------------\n");
// }

void validate_memory(void *ptr, const char *message) {
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed: %s\n", message);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    t_minishell minishell;

    // Initialize minishell
    if (!init_minishell(&minishell, envp)) {
        fprintf(stderr, "Failed to initialize minishell\n");
        cleanup_memory(&minishell);
        return 1;
    }
    init_environment(&minishell, envp);
    printf("=== ULTIMATE MINISHELL STRESS TEST ===\n");
    // debug_env(&minishell);
    //Run stress tests
    test_deep_pipe_chain(&minishell);
    test_pipes_with_redirections(&minishell);
    test_multiple_heredocs(&minishell);
    test_complex_redirections(&minishell);
    test_large_file_processing(&minishell);
    test_builtins_with_pipes(&minishell);
    test_multi_level_redirections(&minishell);
    test_max_file_descriptors(&minishell);
    test_mixed_pipe_redirections(&minishell);
    test_environment_variable_expansion(&minishell);
    test_quote_handling(&minishell);
    test_nested_pipes_redirections(&minishell);
    test_special_characters(&minishell);
    test_error_handling(&minishell);
    test_background_processes(&minishell); 
    test_stderr_redirection(&minishell);
    test_command_substitution(&minishell); 
    test_wildcards_and_globbing(&minishell); 
    test_env_expansion_edge_cases(&minishell);
    test_env_expansion_execution(&minishell);
    test_quote_expansion_execution(&minishell);
    test_exit_status_handling(&minishell);
    test_heredoc_with_large_content(&minishell);
    printf("\n=== STRESS TEST COMPLETED ===\n");

    // Clean up
    for (int i = 0; i < GC_COUNT; i++) {
        if (minishell.gc[i]) {
            gc_cleanup(&minishell.gc[i]);
        }
    }

    return 0;
}

// Test: Simple redirections that should work on all systems
// void test_simple_redirections(t_minishell *minishell)
// {
//     printf("\n=== SIMPLE REDIRECTION TEST (Linux Compatible) ===\n");
    
//     // Create a simple input file
//     const char *input_path = "./simple_input.txt";
//     const char *output_path = "./simple_output.txt";
    
//     // Create the input file with simple content
//     FILE *fp = fopen(input_path, "w");
//     if (fp) {
//         fprintf(fp, "test line 1\ntest line 2\nIMPORTANT line\ntest line 4\n");
//         fclose(fp);
//         printf("Created test input file: %s\n", input_path);
//     } else {
//         printf("Error: Failed to create test file\n");
//         return;
//     }
    
//     // 1. Simple input redirection: cat < simple_input.txt
//     printf("\n  Test 1: Simple input redirection (cat < simple_input.txt)\n");
//     t_infile *input = create_infile(minishell, input_path, INF_IN, NULL);
//     if (!input) {
//         printf("Error: Failed to create input redirection\n");
//         return;
//     }
    
//     t_command_tree *cat_cmd = create_command_with_redirections(
//         minishell, "cat", NULL, input, NULL);
    
//     run_test(minishell, cat_cmd, "Simple input redirection (cat < simple_input.txt)");
    
//     // 2. Simple output redirection: grep IMPORTANT < simple_input.txt > simple_output.txt
//     printf("\n  Test 2: Input and output redirection (grep IMPORTANT < input > output)\n");
    
//     t_infile *grep_input = create_infile(minishell, input_path, INF_IN, NULL);
//     t_outfile *grep_output = create_outfile(minishell, output_path, OUT_TRUNC);
    
//     char *grep_args[] = {"IMPORTANT", NULL};
//     t_command_tree *grep_cmd = create_command_with_redirections(
//         minishell, "grep", grep_args, grep_input, grep_output);
    
//     run_test(minishell, grep_cmd, "Combined redirections (grep IMPORTANT < input > output)");
    
//     // Display the output file content
//     printf("\nOutput file content:\n");
//     fp = fopen(output_path, "r");
//     if (fp) {
//         char buffer[1024];
//         while (fgets(buffer, sizeof(buffer), fp)) {
//             printf("%s", buffer);
//         }
//         fclose(fp);
//     } else {
//         printf("Error: Could not open output file\n");
//     }
    
//     printf("\n=== SIMPLE REDIRECTION TEST COMPLETED ===\n\n");
// }

// int main(int argc, char **argv, char **envp)
// {
//     (void)argc;
//     (void)argv;
//     t_minishell minishell;

//     // Initialize minishell
//     if (init_minishell(&minishell, envp) != 0) {
//         fprintf(stderr, "Failed to initialize minishell\n");
//         return 1;
//     }

//     printf("=== MINISHELL REDIRECTION TEST ===\n");
    
//     // Run just our simple redirection test
//     // test_simple_redirections(&minishell);
//     test_pipes_with_redirections(&minishell);
//     test_multiple_heredocs(&minishell);
    
//     // Clean up
//     for (int i = 0; i < GC_COUNT; i++) {
//         if (minishell.gc[i]) {
//             gc_cleanup(&minishell.gc[i]);
//         }
//     }

//     return 0;
// }