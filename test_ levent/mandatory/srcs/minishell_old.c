#include "../includes/core/minishell.h"
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>

// Global variables for statistics
int g_total_tests = 0;
int g_passed_tests = 0;
int g_category_tests = 0;
int g_category_passed = 0;
clock_t g_start_time;

// ANSI color codes for pretty output
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_BOLD    "\x1b[1m"

// Helper functions
char **create_args(int count, ...) {
    va_list ap;
    char **args = malloc(sizeof(char *) * (count + 1));
    if (!args) exit(1);
    
    va_start(ap, count);
    for (int i = 0; i < count; i++)
        args[i] = ft_strdup(va_arg(ap, char *));
    args[count] = NULL;
    va_end(ap);
    
    return args;
}

t_infile *create_infile(char *name, char *delim, int type) {
    t_infile *in = malloc(sizeof(t_infile));
    if (!in) exit(1);
    in->name = name ? ft_strdup(name) : NULL;
    in->delimeter = delim ? ft_strdup(delim) : NULL;
    in->type = type;
    in->next = NULL;
    return in;
}

t_outfile *create_outfile(char *name, int type) {
    t_outfile *out = malloc(sizeof(t_outfile));
    if (!out) exit(1);
    out->name = ft_strdup(name);
    out->type = type;
    out->next = NULL;
    return out;
}

t_command_tree *create_cmd_node(char *command, char **argv, t_infile *infiles, t_outfile *outfiles) {
    t_exec *exec = malloc(sizeof(t_exec));
    if (!exec) exit(1);
    exec->command = command ? ft_strdup(command) : NULL;
    exec->argv = argv;
    exec->infiles = infiles;
    exec->outfiles = outfiles;
    exec->heredoc_fd = INVALID_FD;
    exec->heredoc_prepared = false;
    
    t_command_tree *node = malloc(sizeof(t_command_tree));
    if (!node) exit(1);
    node->type = N_EXEC;
    node->data = exec;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

t_command_tree *create_pipe_node(t_command_tree *left, t_command_tree *right) {
    t_command_tree *node = malloc(sizeof(t_command_tree));
    if (!node) exit(1);
    node->type = N_PIPE;
    node->data = NULL;
    node->left = left;
    node->right = right;
    return node;
}

t_command_tree *create_and_node(t_command_tree *left, t_command_tree *right) {
    t_command_tree *node = malloc(sizeof(t_command_tree));
    if (!node) exit(1);
    node->type = N_ANDIF;
    node->data = NULL;
    node->left = left;
    node->right = right;
    return node;
}

t_command_tree *create_or_node(t_command_tree *left, t_command_tree *right) {
    t_command_tree *node = malloc(sizeof(t_command_tree));
    if (!node) exit(1);
    node->type = N_OR;
    node->data = NULL;
    node->left = left;
    node->right = right;
    return node;
}

void create_test_file(const char *filename, const char *content) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error creating test file %s\n", filename);
        exit(1);
    }
    fprintf(file, "%s", content);
    fclose(file);
}

int verify_file_content(const char *filename, const char *expected_content) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("❌ File %s not found\n", filename);
        return 0;
    }
    
    char buffer[4096] = {0};
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file);
    buffer[bytes_read] = '\0';
    fclose(file);
    
    if (strcmp(buffer, expected_content) == 0) {
        return 1;
    } else {
        printf("❌ File content mismatch:\nExpected: %s\nActual: %s\n", expected_content, buffer);
        return 0;
    }
}

int file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

void print_progress_bar(int current, int total, const char *message) {
    int bar_width = 30;
    float progress = (float)current / total;
    int filled_width = bar_width * progress;
    
    printf("\r%s [", message);
    for (int i = 0; i < bar_width; i++) {
        if (i < filled_width) printf("█");
        else printf(" ");
    }
    printf("] %d/%d (%d%%)", current, total, (int)(progress * 100));
    if (current == total) printf("\n");
    fflush(stdout);
}

void print_category_header(const char *title) {
    printf("\n%s%s======== %s ========%s\n", COLOR_BOLD, COLOR_BLUE, title, COLOR_RESET);
    g_category_tests = 0;
    g_category_passed = 0;
}

void print_category_summary() {
    float percentage = (float)g_category_passed / g_category_tests * 100;
    if (percentage == 100) {
        printf("%s[✅ ALL PASSED] %d/%d tests passed (%.1f%%)%s\n\n", 
            COLOR_GREEN, g_category_passed, g_category_tests, percentage, COLOR_RESET);
    } else if (percentage >= 80) {
        printf("%s[🟢 MOSTLY PASSED] %d/%d tests passed (%.1f%%)%s\n\n", 
            COLOR_YELLOW, g_category_passed, g_category_tests, percentage, COLOR_RESET);
    } else {
        printf("%s[❌ FAILING] %d/%d tests passed (%.1f%%)%s\n\n", 
            COLOR_RED, g_category_passed, g_category_tests, percentage, COLOR_RESET);
    }
}

int run_test(const char *test_name, t_command_tree *tree, t_minishell *sh, 
            int expected_exit_code, const char *output_file, const char *expected_content) {
    int exit_code_ok;
    int output_ok = 1;
    g_total_tests++;
    g_category_tests++;
    
    printf("  %s%sTest %d:%s %s... ", COLOR_CYAN, COLOR_BOLD, g_total_tests, COLOR_RESET, test_name);
    fflush(stdout);
    
    execute_tree(tree, sh);
    
    exit_code_ok = (sh->exit_code == expected_exit_code);
    
    if (output_file && expected_content) {
        output_ok = verify_file_content(output_file, expected_content);
    } else if (output_file) {
        output_ok = file_exists(output_file);
    }
    
    if (exit_code_ok && output_ok) {
        printf("%s[PASS]%s\n", COLOR_GREEN, COLOR_RESET);
        g_passed_tests++;
        g_category_passed++;
        return 1;
    } else {
        printf("%s[FAIL]%s", COLOR_RED, COLOR_RESET);
        if (!exit_code_ok) {
            printf(" (Exit code: expected %d, got %d)", expected_exit_code, sh->exit_code);
        }
        printf("\n");
        return 0;
    }
}

int run_interactive_test(const char *test_name, t_command_tree *tree, t_minishell *sh) {
    g_total_tests++;
    g_category_tests++;
    
    printf("\n  %s%s== Interactive Test %d ==%s\n", COLOR_MAGENTA, COLOR_BOLD, g_total_tests, COLOR_RESET);
    printf("  %s%s%s%s\n", COLOR_CYAN, COLOR_BOLD, test_name, COLOR_RESET);
    execute_tree(tree, sh);
    
    char answer[3];
    printf("\n  Did the test pass? (y/n): ");
    scanf("%2s", answer);
    while (getchar() != '\n');  // Clear input buffer
    
    if (answer[0] == 'y' || answer[0] == 'Y') {
        g_passed_tests++;
        g_category_passed++;
        printf("  %s[PASS]%s\n", COLOR_GREEN, COLOR_RESET);
        return 1;
    } else {
        printf("  %s[FAIL]%s\n", COLOR_RED, COLOR_RESET);
        return 0;
    }
}

// Signal handler for tests
void test_signal_handler(int sig) {
    printf("\n Signal %d (SIGINT) received\n", sig);
}

int main(int argc, char **argv, char **envp) {
    t_minishell sh;
    g_start_time = clock();
    
    (void)argc;
    (void)argv;
    
    ft_bzero(&sh, sizeof(t_minishell));
    
    // Default environment if none provided
    if (!envp || !*envp) {
        static char *dummy_env[] = {"PATH=/usr/bin:/bin:/usr/local/bin", NULL};
        sh.envp_arr = dummy_env;
    } else {
        sh.envp_arr = envp;
    }
    
    // Setup test files
    create_test_file("input1.txt", "This is a test file\nwith multiple lines\nfor testing");
    create_test_file("multiline.txt", "line 1\nline 2\nline 3\nline 4\nline 5\n");
    
    printf("%s%s\n", COLOR_BOLD, COLOR_MAGENTA);
    printf("  __  __ _       _     _          _ _   _   _ _ _   _                 _       _____          _   \n");
    printf(" |  \\/  (_)_ __ (_)___| |__   ___| | | | | | | | |_(_)_ __ ___   __ _| |_ ___|_   _|__  ___| |_ \n");
    printf(" | |\\/| | | '_ \\| / __| '_ \\ / _ \\ | | | | | | | __| | '_ ` _ \\ / _` | __/ _ \\ | |/ _ \\/ __| __|\n");
    printf(" | |  | | | | | | \\__ \\ | | |  __/ | | | |_| | | |_| | | | | | | (_| | ||  __/ | |  __/\\__ \\ |_ \n");
    printf(" |_|  |_|_|_| |_|_|___/_| |_|\\___|_|_|  \\___/|_|\\__|_|_| |_| |_|\\__,_|\\__\\___| |_|\\___||___/\\__|\n");
    printf("\n%s%s", COLOR_RESET, COLOR_BOLD);
    printf("═════════════════════════════════════════════════════════════════════════════════\n");
    printf("                   ALL-IN-ONE MINISHELL TEST SUITE v1.0                         \n");
    printf("═════════════════════════════════════════════════════════════════════════════════\n");
    printf("%s\n", COLOR_RESET);
    
    //======================= BASIC COMMAND EXECUTION ======================
    print_category_header("BASIC COMMAND EXECUTION");
    
    // Simple echo
    t_command_tree *echo_cmd = create_cmd_node(
        "/bin/echo", 
        create_args(2, "echo", "Hello World"), 
        NULL, 
        NULL
    );
    run_test("Simple echo command", echo_cmd, &sh, 0, NULL, NULL);
    
    // Echo with multiple arguments
    t_command_tree *multi_arg_echo = create_cmd_node(
        "/bin/echo", 
        create_args(5, "echo", "arg1", "arg2", "arg3", "arg4"), 
        NULL, 
        NULL
    );
    run_test("Echo with multiple arguments", multi_arg_echo, &sh, 0, NULL, NULL);
    
    // Running non-existent command
    t_command_tree *nonexistent = create_cmd_node(
        "nonexistentcommand", 
        create_args(1, "nonexistentcommand"), 
        NULL, 
        NULL
    );
    run_test("Non-existent command", nonexistent, &sh, 127, NULL, NULL);
    
    print_category_summary();
    
    //======================= REDIRECTIONS ======================
    print_category_header("REDIRECTIONS");
    
    // Output redirection
    t_outfile *outfile1 = create_outfile("output1.txt", OUT_TRUNC);
    t_command_tree *echo_redirect = create_cmd_node(
        "/bin/echo", 
        create_args(2, "echo", "Redirected output"), 
        NULL, 
        outfile1
    );
    run_test("Output redirection >", echo_redirect, &sh, 0, "output1.txt", "Redirected output\n");
    
    // Append redirection
    t_outfile *outfile2 = create_outfile("output1.txt", OUT_APPEND);
    t_command_tree *echo_append = create_cmd_node(
        "/bin/echo", 
        create_args(2, "echo", "Appended line"), 
        NULL, 
        outfile2
    );
    run_test("Output append >>", echo_append, &sh, 0, "output1.txt", "Redirected output\nAppended line\n");
    
    // Input redirection
    t_infile *infile1 = create_infile("input1.txt", NULL, INFILE);
    t_outfile *outfile3 = create_outfile("output2.txt", OUT_TRUNC);
    t_command_tree *cat_in = create_cmd_node(
        "/bin/cat", 
        create_args(1, "cat"), 
        infile1, 
        outfile3
    );
    run_test("Input redirection <", cat_in, &sh, 0, "output2.txt", "This is a test file\nwith multiple lines\nfor testing");
    
    print_category_summary();
    
    //======================= PIPES ======================
    print_category_header("PIPES");
    
    // Simple pipe
    t_command_tree *ls_cmd = create_cmd_node(
        "/bin/ls", 
        create_args(1, "ls"), 
        NULL, 
        NULL
    );
    
    t_outfile *outfile4 = create_outfile("output3.txt", OUT_TRUNC);
    t_command_tree *grep_cmd = create_cmd_node(
        "/usr/bin/grep", 
        create_args(2, "grep", "txt"), 
        NULL, 
        outfile4
    );
    
    t_command_tree *pipe1 = create_pipe_node(ls_cmd, grep_cmd);
    run_test("Simple pipe with redirection", pipe1, &sh, 0, "output3.txt", NULL);
    
    // Complex pipe chain
    t_command_tree *cat_multi = create_cmd_node(
        "/bin/cat", 
        create_args(2, "cat", "multiline.txt"), 
        NULL, 
        NULL
    );
    
    t_command_tree *grep_line = create_cmd_node(
        "/usr/bin/grep", 
        create_args(2, "grep", "line"), 
        NULL, 
        NULL
    );
    
    t_command_tree *sed_cmd = create_cmd_node(
        "/usr/bin/sed", 
        create_args(2, "sed", "s/line/LINE/g"), 
        NULL, 
        NULL
    );
    
    t_outfile *outfile5 = create_outfile("output4.txt", OUT_TRUNC);
    t_command_tree *head_cmd = create_cmd_node(
        "/usr/bin/head", 
        create_args(3, "head", "-n", "3"), 
        NULL, 
        outfile5
    );
    
    t_command_tree *pipe2 = create_pipe_node(cat_multi, grep_line);
    t_command_tree *pipe3 = create_pipe_node(pipe2, sed_cmd);
    t_command_tree *pipe4 = create_pipe_node(pipe3, head_cmd);
    
    run_test("Complex pipe chain", pipe4, &sh, 0, "output4.txt", "LINE 1\nLINE 2\nLINE 3\n");
    
    print_category_summary();
    
    //======================= HEREDOCS ======================
    print_category_header("HEREDOCS");
    
    // Simple heredoc test with interactive input
    t_infile *heredoc = create_infile(NULL, "ENDMARKER", INF_HEREDOC);
    t_outfile *out8 = create_outfile("heredoc_output.txt", OUT_TRUNC);
    t_command_tree *cat_heredoc = create_cmd_node(
        "/bin/cat", 
        create_args(1, "cat"), 
        heredoc, 
        out8
    );
    
    printf("\n  %s%s== Heredoc Test ==%s\n", COLOR_MAGENTA, COLOR_BOLD, COLOR_RESET);
    printf("  Please type 'Test heredoc content' and then 'ENDMARKER' to end input:\n");
    execute_tree(cat_heredoc, &sh);
    
    int heredoc_ok = verify_file_content("heredoc_output.txt", "Test heredoc content\n");
    g_total_tests++;
    g_category_tests++;
    
    if (heredoc_ok) {
        printf("  %s[PASS]%s Simple heredoc test\n", COLOR_GREEN, COLOR_RESET);
        g_passed_tests++;
        g_category_passed++;
    } else {
        printf("  %s[FAIL]%s Simple heredoc test\n", COLOR_RED, COLOR_RESET);
    }
    
    // Heredoc in pipeline
    t_infile *heredoc_pipe = create_infile(NULL, "PIPEEND", INF_HEREDOC);
    t_command_tree *cat_heredoc_pipe = create_cmd_node(
        "/bin/cat", 
        create_args(1, "cat"), 
        heredoc_pipe, 
        NULL
    );
    
    t_command_tree *grep_heredoc = create_cmd_node(
        "/usr/bin/grep", 
        create_args(2, "grep", "test"), 
        NULL, 
        NULL
    );
    
    t_outfile *heredoc_pipe_out = create_outfile("heredoc_pipe.txt", OUT_TRUNC);
    t_command_tree *wc_heredoc = create_cmd_node(
        "/usr/bin/wc", 
        create_args(2, "wc", "-l"), 
        NULL, 
        heredoc_pipe_out
    );
    
    t_command_tree *pipe_h1 = create_pipe_node(cat_heredoc_pipe, grep_heredoc);
    t_command_tree *pipe_h2 = create_pipe_node(pipe_h1, wc_heredoc);
    
    printf("\n  %s%s== Heredoc Pipeline Test ==%s\n", COLOR_MAGENTA, COLOR_BOLD, COLOR_RESET);
    printf("  Please type these 3 lines:\n  'test line 1'\n  'not a test'\n  'test line 2'\n");
    printf("  Then type 'PIPEEND' to end input\n");
    execute_tree(pipe_h2, &sh);
    
    int heredoc_pipe_ok = verify_file_content("heredoc_pipe.txt", "       3\n");
    g_total_tests++;
    g_category_tests++;
    
    if (heredoc_pipe_ok) {
        printf("  %s[PASS]%s Heredoc pipeline test\n", COLOR_GREEN, COLOR_RESET);
        g_passed_tests++;
        g_category_passed++;
    } else {
        printf("  %s[FAIL]%s Heredoc pipeline test\n", COLOR_RED, COLOR_RESET);
    }
    
    print_category_summary();
    
    //======================= SIGNAL HANDLING ======================
    print_category_header("SIGNAL HANDLING");
    
    // Setup signal handler
    struct sigaction old_act, new_act;
    sigemptyset(&new_act.sa_mask);
    new_act.sa_handler = signal_handler;
    new_act.sa_flags = 0;
    sigaction(SIGINT, &new_act, &old_act);
    
    // Test signal handling with sleep command
    t_command_tree *sleep_cmd = create_cmd_node(
        "/bin/sleep", 
        create_args(2, "sleep", "2"), 
        NULL, 
        NULL
    );
    
    printf("\n  %s%s== Signal Handling Test ==%s\n", COLOR_MAGENTA, COLOR_BOLD, COLOR_RESET);
    printf("  Running: sleep 2 (sending SIGINT after 1 second)\n");
    
    pid_t test_pid = fork();
    if (test_pid == 0) {
        // Reset the signal handler in the child to default
        struct sigaction sa;
        sa.sa_handler = SIG_DFL;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGINT, &sa, NULL);
        
        // Execute sleep
        execute_tree(sleep_cmd, &sh);
        exit(sh.exit_code);
    } else {
        // Send SIGINT after a delay
        sleep(1);
        printf("  Sending SIGINT to child process %d...\n", test_pid);
        kill(test_pid, SIGINT);
        
        int status;
        waitpid(test_pid, &status, 0);
        
        g_total_tests++;
        g_category_tests++;
        
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) {
            printf("  %s[PASS]%s Signal handling test\n", COLOR_GREEN, COLOR_RESET);
            g_passed_tests++;
            g_category_passed++;
        } else {
            printf("  %s[FAIL]%s Signal handling test\n", COLOR_RED, COLOR_RESET);
        }
    }
    
    // Restore original signal handler
    sigaction(SIGINT, &old_act, NULL);
    
    print_category_summary();
    
    //======================= ENVIRONMENT VARIABLES ======================
    print_category_header("ENVIRONMENT VARIABLES");
    
    // export TEST=value
    t_command_tree *export_cmd = create_cmd_node(
        "export", 
        create_args(2, "export", "TEST=value"), 
        NULL, 
        NULL
    );
    run_test("export command", export_cmd, &sh, 0, NULL, NULL);
    
    // env | grep TEST (should find it)
    t_command_tree *env_pipe = create_cmd_node(
        "env", 
        create_args(1, "env"), 
        NULL, 
        NULL
    );
    
    t_command_tree *grep_env = create_cmd_node(
        "/usr/bin/grep", 
        create_args(2, "grep", "TEST"), 
        NULL, 
        NULL
    );
    
    t_command_tree *env_pipe_node = create_pipe_node(env_pipe, grep_env);
    run_test("env | grep TEST (should find it)", env_pipe_node, &sh, 0, NULL, NULL);
    
    // unset TEST
    t_command_tree *unset_cmd = create_cmd_node(
        "unset", 
        create_args(2, "unset", "TEST"), 
        NULL, 
        NULL
    );
    run_test("unset command", unset_cmd, &sh, 0, NULL, NULL);
    
    // env | grep TEST (should not find it)
    t_command_tree *env_pipe2 = create_cmd_node(
        "env", 
        create_args(1, "env"), 
        NULL, 
        NULL
    );
    
    t_command_tree *grep_env2 = create_cmd_node(
        "/usr/bin/grep", 
        create_args(2, "grep", "TEST"), 
        NULL, 
        NULL
    );
    
    t_command_tree *env_pipe_node2 = create_pipe_node(env_pipe2, grep_env2);
    run_test("env | grep TEST (should not find it)", env_pipe_node2, &sh, 1, NULL, NULL);
    
    print_category_summary();
    
    //======================= ERROR HANDLING ======================
    print_category_header("ERROR HANDLING");
    
    // Exit code propagation - SIMPLIFIED VERSION
    system("echo '#!/bin/bash\nexit 42' > exit42.sh");
    system("chmod +x exit42.sh");
    
    t_command_tree *exit42_cmd = create_cmd_node(
        "./exit42.sh", 
        create_args(1, "./exit42.sh"), 
        NULL, 
        NULL
    );
    
    run_test("Exit code propagation", exit42_cmd, &sh, 42, NULL, NULL);
    
    // Access denied error
    system("echo '#!/bin/bash\necho test' > non_executable.sh");
    system("chmod -x non_executable.sh");
    
    t_command_tree *non_exec_cmd = create_cmd_node(
        "./non_executable.sh", 
        create_args(1, "./non_executable.sh"), 
        NULL, 
        NULL
    );
    run_test("Permission denied error", non_exec_cmd, &sh, 126, NULL, NULL);
    
    // Non-existent input file
    t_infile *infile_nonexist = create_infile("nonexistent_file.txt", NULL, INFILE);
    t_command_tree *cat_nonexist = create_cmd_node(
        "/bin/cat", 
        create_args(1, "cat"), 
        infile_nonexist, 
        NULL
    );
    run_test("Non-existent input file", cat_nonexist, &sh, 1, NULL, NULL);
    
    print_category_summary();
    
    //======================= EDGE CASES ======================
    print_category_header("EDGE CASES");
    
    // Empty arguments
    t_command_tree *empty_arg = create_cmd_node(
        "/bin/echo", 
        create_args(2, "echo", ""), 
        NULL, 
        NULL
    );
    run_test("Empty arguments", empty_arg, &sh, 0, NULL, NULL);
    
    // Special characters
    t_command_tree *special_chars = create_cmd_node(
        "/bin/echo", 
        create_args(5, "echo", "\"quote\"", "`backtick`", "*wildcard*", ";semicolon;"), 
        NULL, 
        NULL
    );
    run_test("Special characters", special_chars, &sh, 0, NULL, NULL);
    
    // Very long command
    char **many_args = malloc(sizeof(char *) * 31);
    many_args[0] = ft_strdup("echo");
    for (int i = 1; i < 30; i++) {
        char num[20];
        sprintf(num, "arg%d", i);
        many_args[i] = ft_strdup(num);
    }
    many_args[30] = NULL;
    
    t_command_tree *many_args_node = create_cmd_node(
        "/bin/echo", 
        many_args, 
        NULL, 
        NULL
    );
    run_test("Very long command (30 arguments)", many_args_node, &sh, 0, NULL, NULL);
    
    print_category_summary();
    
    //======================= FINAL SUMMARY ======================
    clock_t end_time = clock();
    double total_time = (double)(end_time - g_start_time) / CLOCKS_PER_SEC;
    
    printf("\n%s%s═════════════════════════════════════════════════════════════════════════════════%s\n", 
           COLOR_BOLD, COLOR_BLUE, COLOR_RESET);
    printf("%s%s                               FINAL RESULTS                                  %s\n", 
           COLOR_BOLD, COLOR_BLUE, COLOR_RESET);
    printf("%s%s═════════════════════════════════════════════════════════════════════════════════%s\n\n", 
           COLOR_BOLD, COLOR_BLUE, COLOR_RESET);
    
    float success_rate = (float)g_passed_tests / g_total_tests * 100;
    
    printf("Total tests run:  %s%d%s\n", COLOR_BOLD, g_total_tests, COLOR_RESET);
    printf("Tests passed:     %s%d%s\n", COLOR_BOLD, g_passed_tests, COLOR_RESET);
    printf("Success rate:     %s%.1f%%%s\n", COLOR_BOLD, success_rate, COLOR_RESET);
    printf("Total time:       %s%.2f%s seconds\n\n", COLOR_BOLD, total_time, COLOR_RESET);
    
    if (g_passed_tests == g_total_tests) {
        printf("%s%s╔══════════════════════════════════════════════════════════════════════════════╗%s\n", 
               COLOR_GREEN, COLOR_BOLD, COLOR_RESET);
        printf("%s%s║                    🎉 🎉 🎉  ALL TESTS PASSED!  🎉 🎉 🎉                     ║%s\n", 
               COLOR_GREEN, COLOR_BOLD, COLOR_RESET);
        printf("%s%s║                                                                              ║%s\n", 
               COLOR_GREEN, COLOR_BOLD, COLOR_RESET);
        printf("%s%s║           Your minishell implementation meets all requirements!              ║%s\n", 
               COLOR_GREEN, COLOR_BOLD, COLOR_RESET);
        printf("%s%s╚══════════════════════════════════════════════════════════════════════════════╝%s\n\n", 
               COLOR_GREEN, COLOR_BOLD, COLOR_RESET);
    } else if (success_rate >= 90) {
        printf("%s%s╔══════════════════════════════════════════════════════════════════════════════╗%s\n", 
               COLOR_YELLOW, COLOR_BOLD, COLOR_RESET);
        printf("%s%s║                       🥈  EXCELLENT WORK!  🥈                                ║%s\n", 
               COLOR_YELLOW, COLOR_BOLD, COLOR_RESET);
        printf("%s%s║                                                                              ║%s\n", 
               COLOR_YELLOW, COLOR_BOLD, COLOR_RESET);
        printf("%s%s║           Your minishell is almost perfect! Just a few fixes needed.         ║%s\n", 
               COLOR_YELLOW, COLOR_BOLD, COLOR_RESET);
        printf("%s%s╚══════════════════════════════════════════════════════════════════════════════╝%s\n\n", 
               COLOR_YELLOW, COLOR_BOLD, COLOR_RESET);
    } else if (success_rate >= 75) {
        printf("%s%s╔══════════════════════════════════════════════════════════════════════════════╗%s\n", 
               COLOR_YELLOW, COLOR_BOLD, COLOR_RESET);
        printf("%s%s║                         🥉  GOOD PROGRESS!  🥉                               ║%s\n", 
               COLOR_YELLOW, COLOR_BOLD, COLOR_RESET);
        printf("%s%s║                                                                              ║%s\n", 
               COLOR_YELLOW, COLOR_BOLD, COLOR_RESET);
        printf("%s%s║              Your minishell is getting there! Keep improving.                ║%s\n", 
               COLOR_YELLOW, COLOR_BOLD, COLOR_RESET);
        printf("%s%s╚══════════════════════════════════════════════════════════════════════════════╝%s\n\n", 
               COLOR_YELLOW, COLOR_BOLD, COLOR_RESET);
    } else {
        printf("%s%s╔══════════════════════════════════════════════════════════════════════════════╗%s\n", 
               COLOR_RED, COLOR_BOLD, COLOR_RESET);
        printf("%s%s║                      ❌  NEEDS IMPROVEMENT  ❌                               ║%s\n", 
               COLOR_RED, COLOR_BOLD, COLOR_RESET);
        printf("%s%s║                                                                              ║%s\n", 
               COLOR_RED, COLOR_BOLD, COLOR_RESET);
        printf("%s%s║         Your minishell implementation still needs significant work.          ║%s\n", 
               COLOR_RED, COLOR_BOLD, COLOR_RESET);
        printf("%s%s╚══════════════════════════════════════════════════════════════════════════════╝%s\n\n", 
               COLOR_RED, COLOR_BOLD, COLOR_RESET);
    }
    
    // Cleanup
    system("rm -f output1.txt output2.txt output3.txt output4.txt");
    system("rm -f heredoc_output.txt heredoc_pipe.txt");
    system("rm -f and_test.txt or_test.txt");
    system("rm -f exit42.sh non_executable.sh");
    system("rm -f input1.txt multiline.txt");
    
    return (success_rate == 100 ? 0 : 1);
}