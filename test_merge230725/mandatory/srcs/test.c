// #include "../includes/core/minishell.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h> // Für bool in t_exec

// // Annahme: Diese Funktionen sind in deinen Headern deklariert
// t_command_tree  *start_parser(t_tokens *tokens, t_gc *gc);
// t_tokens        *lexer(const char *line, t_minishell *shell);
// void            gc_cleanup(t_gc **gc);
// t_gc            *gc_create(void);


// /*
// ================================================================================
//                         HILFSFUNKTIONEN ZUM DRUCKEN DES BAUMS
// ================================================================================
// */

// // Deklaration, da sich die Funktionen gegenseitig aufrufen
// void print_command_tree(t_command_tree *node, int level);

// // Hilfsfunktion, um die Einrückung für die Baumstruktur zu drucken
// static void print_indent(int level)
// {
//     for (int i = 0; i < level; i++)
//         printf("    ");
// }

// // Druckt die Details eines EXEC-Knotens (Argumente und Umleitungen)
// static void print_exec_node(t_exec *exec, int level)
// {
//     if (!exec)
//     {
//         print_indent(level);
//         printf("-> (NULL exec data)\n");
//         return;
//     }

//     // Drucke Argumente
//     print_indent(level);
//     printf("-> ARGV: ");
//     if (!exec->argv || !exec->argv[0])
//         printf("(empty)\n");
//     else
//     {
//         for (int i = 0; exec->argv[i]; i++)
//             printf("'%s' ", exec->argv[i]);
//         printf("\n");
//     }

//     // Drucke Infile-Umleitungen
//     if (exec->infiles)
//     {
//         print_indent(level);
//         printf("-> INFILES: ");
//         for (t_infile *curr = exec->infiles; curr; curr = curr->next)
//             printf("['%s'] ", curr->name); // Angepasst an deine t_exec-Struktur
//         printf("\n");
//     }

//     // Drucke Outfile-Umleitungen
//     if (exec->outfiles)
//     {
//         print_indent(level);
//         printf("-> OUTFILES: ");
//         for (t_outfile *curr = exec->outfiles; curr; curr = curr->next)
//             printf("['%s'] ", curr->name); // Angepasst an deine t_exec-Struktur
//         printf("\n");
//     }

//     // Drucke Heredoc-Informationen
//     print_indent(level);
//     printf("-> HEREDOC: prepared=%s, fd=%d\n",
//         exec->heredoc_prepared ? "true" : "false", exec->heredoc_fd);
// }

// // Rekursive Hauptfunktion zum Drucken des Befehlsbaums
// void print_command_tree(t_command_tree *node, int level)
// {
//     if (!node)
//     {
//         print_indent(level);
//         printf("(NULL Tree)\n");
//         return;
//     }

//     print_indent(level);
//     switch (node->type)
//     {
//         case N_EXEC:
//             printf("NODE: EXEC\n");
//             print_exec_node((t_exec *)node->data, level + 1);
//             break;
//         case N_PIPE:
//             printf("NODE: PIPE\n");
//             print_command_tree(node->left, level + 1);
//             print_command_tree(node->right, level + 1);
//             break;
//         case N_ANDIF:
//             printf("NODE: AND_IF (&&)\n");
//             print_command_tree(node->left, level + 1);
//             print_command_tree(node->right, level + 1);
//             break;
//         case N_OR:
//             printf("NODE: OR (||)\n");
//             print_command_tree(node->left, level + 1);
//             print_command_tree(node->right, level + 1);
//             break;
//         case N_SUBSHELL:
//             printf("NODE: SUBSHELL\n");
//             if (node->data)
//             {
//                 print_indent(level + 1);
//                 printf("Subshell Redirections:\n");
//                 print_exec_node((t_exec *)node->data, level + 2);
//             }
//             print_indent(level + 1);
//             printf("Subshell Content:\n");
//             print_command_tree(node->left, level + 2);
//             break;
//         default:
//             printf("NODE: UNKNOWN\n");
//             break;
//     }
// }

// /*
// ================================================================================
//                                 PARSER-TESTS
// ================================================================================
// */

// // Wrapper-Funktion, die einen Test ausführt und den Baum druckt
// void run_parser_test(const char *name, const char *cmd, t_minishell *shell)
// {
//     t_command_tree *tree;
//     t_tokens *tokens;

//     printf("--- Test: %s ---\n", name);
//     printf("Input: %s\n", cmd);

//     // 1. Lexer aufrufen
//     tokens = lexer(cmd, shell);
//     if (!tokens)
//     {
//         printf("Lexer failed!\n");
//         return;
//     }

//     // 2. Top-Level-Parser aufrufen
//     tree = start_parser(tokens, shell->gc[GC_COMMAND]);

//     // 3. Ergebnis drucken
//     printf("Output Tree:\n");
//     print_command_tree(tree, 0);
//     printf("----------------------------------------\n\n");

//     // Speicher für den nächsten Test freigeben
//     gc_cleanup(&shell->gc[GC_COMMAND]);
//     shell->gc[GC_COMMAND] = gc_create();
// }

// int main(void)
// {
//     t_minishell shell;
//     shell.gc[GC_COMMAND] = gc_create();

//     // --- GRUNDLAGEN ---
//     run_parser_test("Simple Command", "ls -l", &shell);
//     run_parser_test("Command with >", "echo hello > outfile.txt", &shell);
//     run_parser_test("Command with <", "wc -l < infile.txt", &shell);

//     // --- KOMPLEXERE FÄLLE ---
//     run_parser_test("Multiple Redirections", "< in.txt echo hello world >> out.log", &shell);
//     run_parser_test("Only Redirections", "> file1 < file2", &shell);

//     // --- FEHLERFÄLLE UND LEERE EINGABEN ---
//     run_parser_test("Empty Input", "", &shell);
//     run_parser_test("Syntax Error", "ls >", &shell);
//     run_parser_test("Syntax Error with Pipe", "ls | >", &shell);
//     run_parser_test("Syntax Error with AND", "ls &&", &shell);

//     // --- TESTS FÜR KOMPLETTEN PARSER ---
//     run_parser_test("Simple Pipe", "ls -l | grep test", &shell);
//     run_parser_test("Pipe with Redirections", "cat < in.txt | wc -l > out.txt", &shell);
//     run_parser_test("Logical AND", "make && ./a.out", &shell);
//     run_parser_test("Complex Command", "cat file | grep -i 'error' && echo 'Found errors' > errors.log", &shell);
// 	run_parser_test("unclosed quotes", "echo 'hi", &shell);

//     // --- TESTS FÜR SUBSHELLS ---
//     printf("\n================ SUBSHELL TESTS ================\n\n");
//     run_parser_test("Simple Subshell", "(ls -l)", &shell);
//     run_parser_test("Subshell in Pipe", "(echo hello) | wc -c", &shell);
//     run_parser_test("Subshell with Redirection", "(date) > date.txt", &shell);
//     run_parser_test("Nested Subshells", "((echo nested))", &shell);
//     run_parser_test("Complex Subshell", "(cat < file.txt | grep 'a') && echo 'done'", &shell);
//     run_parser_test("Syntax Error (Unclosed Paren)", "(ls", &shell);

//     // Finales Cleanup
//     gc_cleanup(&shell.gc[GC_COMMAND]);
//     return 0;
// }
