// //-------------------------------------------------------------------------------
// //-------------------------------LEXER_TESTS-------------------------------------
// //-------------------------------------------------------------------------------
// #include "../includes/core/minishell.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <strings.h> // für bzero

// const char *type_to_str(t_token_type type)
// {
// 	static const char *types[] = {
// 		"WORD", "PIPE", "APPEND", "OUTFILE", "INFILE",
// 		"HEREDOC", "AND_IF", "OR", "END", "PARENTHESSIS"
// 	};
// 	if (type >= 0 && type <= L_PAREN)
// 		return (types[type]);
// 	return ("UNKNOWN_TYPE");
// }

// const char *state_to_str(t_token_state state)
// {
// 	static const char *states[] = {
// 		"GENERAL", "IN_DQUOTES", "IN_SQUOTES", "UNCLOSED_QUOTES", "EXPAND"
// 	};
// 	if (state >= 0 && state <= EXPAND)
// 		return (states[state]);
// 	return ("UNKNOWN_STATE");
// }

// // Deine überarbeitete print_tokens-Funktion
// void print_tokens(t_tokens *tokens)
// {
// 	if (!tokens)
// 	{
// 		printf("Lexer returned NULL\n");
// 		return;
// 	}
// 	printf("Token count: %d\n", tokens->count);
// 	for (int i = 0; i < tokens->count; i++)
// 	{
// 		printf("Token[%d]: '%s' Type: %s State: %s\n",
// 			i,
// 			tokens->arr[i].value,
// 			type_to_str(tokens->arr[i].type),
// 			state_to_str(tokens->arr[i].state));
// 	}
// }

// // Test 1: Einfaches Kommando mit Argumenten
// void test_simple_command(t_minishell *shell)
// {
//     printf("Test 1: Simple command\n");
//     const char *cmd = "echo hello world";
//     t_tokens *tokens = lexer(cmd, shell);
//     print_tokens(tokens);
// }

// // Test 2: Operatoren
// void test_operators(t_minishell *shell)
// {
//     printf("Test 2: Operators\n");
//     const char *cmd = "ls | grep txt > outfile";
//     t_tokens *tokens = lexer(cmd, shell);
//     print_tokens(tokens);
// }

// // Test 3: Quotes
// void test_quotes(t_minishell *shell)
// {
//     printf("Test 3: Quotes\n");
//     const char *cmd = "echo \"hello world\" 'and more'";
//     t_tokens *tokens = lexer(cmd, shell);
//     print_tokens(tokens);
// }

// // Test 4: Special Operators (AND_IF, OR)
// void test_logical_operators(t_minishell *shell)
// {
//     printf("Test 4: Logical operators\n");
//     const char *cmd = "cmd1 && cmd2 || cmd3";
//     t_tokens *tokens = lexer(cmd, shell);
//     print_tokens(tokens);
// }

// // Test 5: Unclosed Quotes (laut Subject nicht zwingend behandeln)
// void test_unclosed_quote(t_minishell *shell)
// {
//     printf("Test 5: Unclosed quote\n");
//     const char *cmd = "echo \"hello world";
//     t_tokens *tokens = lexer(cmd, shell);
//     print_tokens(tokens);
// }

// // Test 6: INFILE ('<')
// void test_infile(t_minishell *shell)
// {
//     printf("Test 6: Infile '<'\n");
//     const char *cmd = "cat < input.txt";
//     t_tokens *tokens = lexer(cmd, shell);
//     print_tokens(tokens);
// }

// // Test 7: OUTFILE ('>')
// void test_outfile(t_minishell *shell)
// {
//     printf("Test 7: Outfile '>'\n");
//     const char *cmd = "echo hello > output.txt";
//     t_tokens *tokens = lexer(cmd, shell);
//     print_tokens(tokens);
// }

// // Test 8: APPEND ('>>')
// void test_append(t_minishell *shell)
// {
//     printf("Test 8: Append '>>'\n");
//     const char *cmd = "echo again >> log.txt";
//     t_tokens *tokens = lexer(cmd, shell);
//     print_tokens(tokens);
// }

// // Test 9: INVALID REDIR:
// void test_invalid_redirect(t_minishell *shell)
// {
//     printf("Test 9: Invalid redirect '>>>'\n");
//     const char *cmd = ">>>>|";
//     t_tokens *tokens = lexer(cmd, shell);
//     print_tokens(tokens);
// }

// // Test 10: HEREDOC ('<<')
// void test_heredoc(t_minishell *shell)
// {
//     printf("Test 10: Heredoc '<<'\n");
//     const char *cmd = "cat << EOF";
//     t_tokens *tokens = lexer(cmd, shell);
//     print_tokens(tokens);
// }

// // Test 11: Parenthesis
// void test_parenthesis(t_minishell *shell)
// {
//     printf("Test 11: Parenthesis '('\n");
//     const char *cmd = "(echo test) && (ls)";
//     t_tokens *tokens = lexer(cmd, shell);
//     print_tokens(tokens);
// }

// void test_weird_case(t_minishell *shell)
// {
//     printf("Test 12: echo $'$'$'$'\n");
//     const char *cmd = "echo $'$'$'$'";
//     t_tokens *tokens = lexer(cmd, shell);
//     print_tokens(tokens);
// }

// int main(void)
// {
//     t_minishell shell;
//     shell.gc[GC_COMMAND] = gc_create();

//     test_simple_command(&shell);
//     test_operators(&shell);
//     test_quotes(&shell);
//     test_logical_operators(&shell);
//     test_unclosed_quote(&shell);
//     test_infile(&shell);
//     test_outfile(&shell);
//     test_append(&shell);
//     test_invalid_redirect(&shell);
//     test_heredoc(&shell);
//     test_parenthesis(&shell);
//     test_weird_case(&shell);

//     gc_cleanup(&shell.gc[GC_COMMAND]);
//     return 0;
// }
// //-------------------------------------------------------------------------------
// //-------------------------------LEXER_TESTS-------------------------------------
// //-------------------------------------------------------------------------------



// // //-------------------------------------------------------------------------------
// // //-------------------------------PARSE_CMD_TESTS---------------------------------
// // //-------------------------------------------------------------------------------
// #include "../includes/core/minishell.h"
// #include <stdio.h>
// #include <stdlib.h>
// /*
// ================================================================================
//                         HELPER FUNCTIONS TO PRINT THE TREE
// ================================================================================
// */

// // NEU: Konvertiert Outfile-Typen in Strings
// static const char *get_outfile_type_str(t_out_type type)
// {
//     if (type == OUT_APPEND)
//         return "APPEND";
//     if (type == OUT_TRUNC)
//         return "TRUNCATE";
//     return "UNKNOWN";
// }

// // NEU: Konvertiert Infile-Typen in Strings
// static const char *get_infile_type_str(t_in_type type)
// {
//     if (type == INF_IN)
//         return "INFILE";
//     if (type == INF_HEREDOC)
//         return "HEREDOC";
//     return "UNKNOWN";
// }


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

//     // Drucke Infile-Umleitungen (AKTUALISIERT)
//     if (exec->infiles)
//     {
//         print_indent(level);
//         printf("-> INFILES: ");
//         for (t_infile *curr = exec->infiles; curr; curr = curr->next)
//             printf("['%s', type=%s] ", curr->name, get_infile_type_str(curr->type));
//         printf("\n");
//     }

//     // Drucke Outfile-Umleitungen (AKTUALISIERT)
//     if (exec->outfiles)
//     {
//         print_indent(level);
//         printf("-> OUTFILES: ");
//         for (t_outfile *curr = exec->outfiles; curr; curr = curr->next)
//             printf("['%s', type=%s] ", curr->name, get_outfile_type_str(curr->type));
//         printf("\n");
//     }
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
//         default:
//             printf("NODE: UNKNOWN\n");
//             break;
//     }
// }

// /*
// ================================================================================
//                                 PARSER TESTS
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

//     // 2. Parser aufrufen (hier wird parse_cmd intern verwendet)
//     // HINWEIS: Ich nehme an, du hast eine Top-Level `parser`-Funktion.
//     // Falls nicht, kannst du hier direkt `parse_cmd` aufrufen.
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

//     // Test 1: Einfacher Befehl
//     run_parser_test("Simple Command", "ls -l", &shell);

//     // Test 2: Befehl mit Output-Umleitung
//     run_parser_test("Command with >", "echo hello > outfile.txt", &shell);

//     // Test 3: Befehl mit Input-Umleitung
//     run_parser_test("Command with <", "wc -l < infile.txt", &shell);

//     // Test 4: Befehl mit Append-Umleitung
//     run_parser_test("Command with >>", "echo more >> log.txt", &shell);

//     // Test 5: Befehl mit Heredoc
//     run_parser_test("Command with <<", "cat << EOF", &shell);

//     // Test 6: Befehl mit mehreren gemischten Umleitungen
//     run_parser_test("Multiple Redirections", "< in.txt echo hello world >> out.log", &shell);

//     // Test 7: Befehl, der nur aus Umleitungen besteht
//     run_parser_test("Only Redirections", "> file1 < file2", &shell);

//     // Test 8: Komplette Pipe
//     run_parser_test("Simple Pipe", "ls -l | grep test", &shell);

//     // Test 9: Komplexere Pipe mit Umleitungen
//     run_parser_test("Pipe with Redirections", "cat < in.txt | wc -l > out.txt", &shell);

//     // Test 10: Logisches AND
//     run_parser_test("Logical AND", "make && ./a.out", &shell);

//     // Test 11: Syntaxfehler (sollte NULL-Baum oder Fehler ausgeben)
//     run_parser_test("Syntax Error", "ls >", &shell);

//     // Finales Cleanup
//     gc_cleanup(&shell.gc[GC_COMMAND]);
//     return 0;
// }
//-------------------------------------------------------------------------------
//-------------------------------PARSE_CMD_TESTS---------------------------------
//-------------------------------------------------------------------------------

#include "../includes/core/minishell.h"
#include <stdbool.h>

// HINWEIS: Du musst die Deklarationen für alle deine Parser-Funktionen
// in einer Header-Datei haben, die hier inkludiert wird.
// Zum Beispiel in "parser.h".
t_command_tree  *start_parser(t_tokens *tokens, t_gc *gc);
t_tokens        *lexer(const char *line, t_minishell *shell);
void            gc_cleanup(t_gc **gc);
t_gc            *gc_create(void);

/*
================================================================================
                        HELFERFUNKTIONEN ZUM DRUCKEN DES BAUMS
================================================================================
*/

// Deklaration, da sich die Funktionen gegenseitig aufrufen
void print_command_tree(t_command_tree *node, int level);

// Hilfsfunktion, um die Einrückung für die Baumstruktur zu drucken
static void print_indent(int level)
{
    for (int i = 0; i < level; i++)
        printf("    ");
}

// Druckt die Details eines EXEC-Knotens (Argumente und Umleitungen)
static void print_exec_node(t_exec *exec, int level)
{
    if (!exec)
    {
        print_indent(level);
        printf("-> (NULL exec data)\n");
        return;
    }

    // Drucke Argumente
    print_indent(level);
    printf("-> ARGV: ");
    if (!exec->argv || !exec->argv[0])
        printf("(empty)\n");
    else
    {
        for (int i = 0; exec->argv[i]; i++)
            printf("'%s' ", exec->argv[i]);
        printf("\n");
    }

    // Drucke Infile-Umleitungen
    if (exec->infiles)
    {
        print_indent(level);
        printf("-> INFILES: ");
        for (t_infile *curr = exec->infiles; curr; curr = curr->next)
            printf("['%s'] ", curr->name);
        printf("\n");
    }

    // Drucke Outfile-Umleitungen
    if (exec->outfiles)
    {
        print_indent(level);
        printf("-> OUTFILES: ");
        for (t_outfile *curr = exec->outfiles; curr; curr = curr->next)
            printf("['%s'] ", curr->name);
        printf("\n");
    }

    // Drucke Heredoc-Informationen
    print_indent(level);
    printf("-> HEREDOC: prepared=%s, fd=%d\n",
        exec->heredoc_prepared ? "true" : "false", exec->heredoc_fd);
}

// Rekursive Hauptfunktion zum Drucken des Befehlsbaums
void print_command_tree(t_command_tree *node, int level)
{
    if (!node)
    {
        print_indent(level);
        printf("(NULL Tree)\n");
        return;
    }

    print_indent(level);
    switch (node->type)
    {
        case N_EXEC:
            printf("NODE: EXEC\n");
            print_exec_node((t_exec *)node->data, level + 1);
            break;
        case N_PIPE:
            printf("NODE: PIPE\n");
            print_command_tree(node->left, level + 1);
            print_command_tree(node->right, level + 1);
            break;
        case N_ANDIF:
            printf("NODE: AND_IF (&&)\n");
            print_command_tree(node->left, level + 1);
            print_command_tree(node->right, level + 1);
            break;
        case N_OR:
            printf("NODE: OR (||)\n");
            print_command_tree(node->left, level + 1);
            print_command_tree(node->right, level + 1);
            break;
        case N_SUBSHELL:
            printf("NODE: SUBSHELL\n");
            if (node->data)
            {
                print_indent(level + 1);
                printf("Subshell Redirections:\n");
                print_exec_node((t_exec *)node->data, level + 2);
            }
            print_indent(level + 1);
            printf("Subshell Content:\n");
            print_command_tree(node->left, level + 2);
            break;
        default:
            printf("NODE: UNKNOWN\n");
            break;
    }
}

/*
================================================================================
                                PARSER-TESTS
================================================================================
*/

// Wrapper-Funktion, die einen Test ausführt und den Baum druckt
void run_parser_test(const char *name, const char *cmd, t_minishell *shell)
{
    t_command_tree *tree;
    t_tokens *tokens;

    printf("--- Test: %s ---\n", name);
    printf("Input: %s\n\n", cmd);

    // 1. Lexer aufrufen
    tokens = lexer(cmd, shell);
    if (!tokens)
    {
        printf("Lexer failed!\n\n");
        return;
    }

    // 2. Top-Level-Parser aufrufen
    tree = start_parser(tokens, shell->gc[GC_COMMAND]);

    // 3. Ergebnis drucken
    printf("\nOutput Tree:\n");
    print_command_tree(tree, 0);
    printf("----------------------------------------\n\n");

    // Speicher für den nächsten Test freigeben
    gc_cleanup(&shell->gc[GC_COMMAND]);
    shell->gc[GC_COMMAND] = gc_create();
}

int main(void)
{
    t_minishell shell;
    shell.gc[GC_COMMAND] = gc_create();

    printf("\n================ PARSER TESTS ================\n\n");

    // Der kritische Testfall, der die Endlosschleife auslöst
    run_parser_test("Subshell Command", "(ls -la)", &shell);

    // Ein weiterer Test, um zu sehen, ob die Schleife auch hier auftritt
    run_parser_test("Syntax Error Pipe", "ls | | cat", &shell);

    // Ein paar andere Fälle, um die allgemeine Funktionalität zu prüfen
    run_parser_test("Simple Command", "echo hello", &shell);
    run_parser_test("Complex Command", "(cat < file.txt | grep 'a') && echo 'done'", &shell);
    run_parser_test("Syntax Error (Unclosed Paren)", "(ls", &shell);
    run_parser_test("Nested Subshell", "echo 'hi' && (((echo 'hello')))", &shell);
    run_parser_test("complicated syntax", "echo 'hi' && echo 'hello' || (ls -la && echo 'welcome')", &shell);
    run_parser_test("unclosed", "echo 'hi", &shell);


    // Finales Cleanup
    gc_cleanup(&shell.gc[GC_COMMAND]);
    return 0;
}
