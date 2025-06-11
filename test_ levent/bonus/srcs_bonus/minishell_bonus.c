#include "../inc_bonus/minishell_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Define these OUTSIDE any function, at global scope
t_command_tree *create_exit_cmd(t_minishell *minishell, int exit_code) {
    t_command_tree *cmd = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    t_exec *exec = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_exec));
    cmd->type = N_EXEC;
    cmd->left = NULL;
    cmd->right = NULL;
    cmd->data = exec;
    
    exec->command = gc_strdup(minishell->gc[GC_COMMAND], "exit");
    exec->argv = gc_malloc(minishell->gc[GC_COMMAND], sizeof(char *) * 3);
    
    char code_str[10];
    snprintf(code_str, sizeof(code_str), "%d", exit_code);
    
    exec->argv[0] = exec->command;
    exec->argv[1] = gc_strdup(minishell->gc[GC_COMMAND], code_str);
    exec->argv[2] = NULL;
    
    exec->infiles = NULL;
    exec->outfiles = NULL;
    
    return cmd;
}

void run_operator_test(t_minishell *minishell, t_command_tree *tree, 
                      const char *description, int expected) {
    printf("\n--- %s [Expected: %d] ---\n", description, expected);
    
    // Store original exit code
    int original_exit = minishell->exit_code;
    
    // Execute the tree
    execute_tree(tree, minishell);
    
    // Report results
    printf("Exit code: %d %s\n", minishell->exit_code, 
           minishell->exit_code == expected ? "✅ PASS" : "❌ FAIL");
    
    // Reset exit code for next test
    minishell->exit_code = original_exit;
}
/**
 * Simplified test for logical operator exit codes
 */
void test_exit_codes(t_minishell *minishell)
{
    printf("\n=== SIMPLIFIED LOGICAL OPERATORS EXIT CODE TEST ===\n");
        
    // Test 1: true && true => 0
    t_command_tree *test1 = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    test1->type = AND_OPERATOR;
    test1->data = NULL;
    test1->left = create_exit_cmd(minishell, 0);;
    test1->right = create_exit_cmd(minishell, 0);;
    run_operator_test(minishell, test1, "Test 1: (exit 0) && (exit 0)", 0);
    
    // Test 2: true && false => 1
    t_command_tree *test2 = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    test2->type = AND_OPERATOR;
    test2->data = NULL;
    test2->left = create_exit_cmd(minishell, 0);;
    test2->right = create_exit_cmd(minishell, 1);
    run_operator_test(minishell, test2, "Test 2: (exit 0) && (exit 1)", 1);
    
    // Test 3: false && true => 1 (short-circuit)
    t_command_tree *test3 = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    test3->type = AND_OPERATOR;
    test3->data = NULL;
    test3->left = create_exit_cmd(minishell, 1);
    test3->right = create_exit_cmd(minishell, 0);;
    run_operator_test(minishell, test3, "Test 3: (exit 1) && (exit 0)", 1);
    
    // Test 4: true || true => 0 (short-circuit)
    t_command_tree *test4 = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    test4->type = OR_OPERATOR;
    test4->data = NULL;
    test4->left = create_exit_cmd(minishell, 0);;
    test4->right = create_exit_cmd(minishell, 0);;
    run_operator_test(minishell, test4, "Test 4: (exit 0) || (exit 0)", 0);
    
    // Test 5: true || false => 0 (short-circuit)
    t_command_tree *test5 = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    test5->type = OR_OPERATOR;
    test5->data = NULL;
    test5->left = create_exit_cmd(minishell, 0);;
    test5->right = create_exit_cmd(minishell, 1);
    run_operator_test(minishell, test5, "Test 5: (exit 0) || (exit 1)", 0);
    
    // Test 6: false || true => 0
    t_command_tree *test6 = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    test6->type = OR_OPERATOR;
    test6->data = NULL;
    test6->left = create_exit_cmd(minishell, 1);
    test6->right = create_exit_cmd(minishell, 0);;
    run_operator_test(minishell, test6, "Test 6: (exit 1) || (exit 0)", 0);
    
    // Test 7: false || false => 1
    t_command_tree *test7 = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    test7->type = OR_OPERATOR;
    test7->data = NULL;
    test7->left = create_exit_cmd(minishell, 1);
    test7->right = create_exit_cmd(minishell, 1);
    run_operator_test(minishell, test7, "Test 7: (exit 1) || (exit 1)", 1);
    
    // Test 8: Custom exit codes (true && exit 2) => 2
    t_command_tree *test8 = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    test8->type = AND_OPERATOR;
    test8->data = NULL;
    test8->left = create_exit_cmd(minishell, 0);;
    test8->right = create_exit_cmd(minishell, 2);
    run_operator_test(minishell, test8, "Test 8: (exit 0) && (exit 2)", 2);
    
    // Test 9: Custom exit codes (false || exit 3) => 3
    t_command_tree *test9 = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    test9->type = OR_OPERATOR;
    test9->data = NULL;
    test9->left = create_exit_cmd(minishell, 1);
    test9->right = create_exit_cmd(minishell, 3);
    run_operator_test(minishell, test9, "Test 9: (exit 1) || (exit 3)", 3);
    
    // Test 10: Nested operators true && (false || exit 2) => 2
    t_command_tree *inner = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    inner->type = OR_OPERATOR;
    inner->data = NULL;
    inner->left = create_exit_cmd(minishell, 1);
    inner->right = create_exit_cmd(minishell, 2);
    
    t_command_tree *test10 = gc_malloc(minishell->gc[GC_COMMAND], sizeof(t_command_tree));
    test10->type = AND_OPERATOR;
    test10->data = NULL;
    test10->left = create_exit_cmd(minishell, 0);;
    test10->right = inner;
    run_operator_test(minishell, test10, "Test 10: (exit 0) && ((exit 1) || (exit 2))", 2);
    
    printf("\n=== SIMPLIFIED LOGICAL OPERATORS EXIT CODE TEST COMPLETED ===\n");
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    t_minishell minishell;

    // Initialize minishell
    if (!init_minishell(&minishell, envp)) {
        fprintf(stderr, "Failed to initialize minishell\n");
        return 1;
    }
    init_environment(&minishell, envp);
    
    printf("=== LOGICAL OPERATORS EXIT CODE TEST PROGRAM ===\n");
    test_exit_codes(&minishell);
    printf("\n=== TEST COMPLETED ===\n");

    // Clean up
    for (int i = 0; i < GC_COUNT; i++) {
        if (minishell.gc[i]) {
            gc_cleanup(&minishell.gc[i]);
        }
    }

    return 0;
}
