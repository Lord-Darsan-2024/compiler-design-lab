/*
 * ============================================================================
 * Lab 7: Shift-Reduce Parsing
 * ============================================================================
 *
 * DESCRIPTION:
 * This laboratory implements a shift-reduce parser for bottom-up parsing.
 * The parser maintains a stack and uses shift and reduce actions guided by
 * parsing rules based on recognizing production right-hand sides (handles)
 * on the stack. The program uses simple pattern matching to recognize handles
 * and reduce when appropriate, demonstrating the principles of operator
 * precedence and shift-reduce parsing. Parsing progresses from bottom (input
 * symbols) to top (start symbol), building the parse tree bottom-up.
 *
 * INPUT:
 * Grammar: E -> E+T | T, T -> T*F | F, F -> (E) | id
 * Input: id*id+id
 *
 * OUTPUT:
 * Stack contents at each step, shift/reduce actions, and final parse result
 *
 * COMPILATION AND EXECUTION:
 * gcc -std=c99 -Wall -Wextra -O2 -o shift_reduce_parser shift_reduce_parser.c
 * ./shift_reduce_parser
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ======================== Type Definitions ======================== */

#define MAX_STACK        100
#define MAX_RULES        20
#define MAX_INPUT        50
#define MAX_PROD_RHS     20

typedef struct {
    char lhs;
    char rhs[MAX_PROD_RHS];
    int rhs_len;
} Production;

typedef struct {
    Production productions[MAX_RULES];
    int production_count;
} Grammar;

/*
 * FUNCTION: initialize_grammar
 * PURPOSE: Load hardcoded expression grammar
 * PARAMETERS: grammar - grammar to initialize
 * RETURN: void
 * COMPLEXITY: O(1)
 */
void initialize_grammar(Grammar *grammar)
{
    grammar->production_count = 0;

    /* E -> E+T */
    grammar->productions[0].lhs = 'E';
    strcpy(grammar->productions[0].rhs, "E+T");
    grammar->productions[0].rhs_len = 3;
    grammar->production_count++;

    /* E -> T */
    grammar->productions[1].lhs = 'E';
    strcpy(grammar->productions[1].rhs, "T");
    grammar->productions[1].rhs_len = 1;
    grammar->production_count++;

    /* T -> T*F */
    grammar->productions[2].lhs = 'T';
    strcpy(grammar->productions[2].rhs, "T*F");
    grammar->productions[2].rhs_len = 3;
    grammar->production_count++;

    /* T -> F */
    grammar->productions[3].lhs = 'T';
    strcpy(grammar->productions[3].rhs, "F");
    grammar->productions[3].rhs_len = 1;
    grammar->production_count++;

    /* F -> (E) */
    grammar->productions[4].lhs = 'F';
    strcpy(grammar->productions[4].rhs, "(E)");
    grammar->productions[4].rhs_len = 3;
    grammar->production_count++;

    /* F -> id */
    grammar->productions[5].lhs = 'F';
    strcpy(grammar->productions[5].rhs, "id");
    grammar->productions[5].rhs_len = 2;
    grammar->production_count++;
}

/*
 * FUNCTION: find_handle
 * PURPOSE: Check if stack top matches any production RHS
 * PARAMETERS: grammar - grammar, stack - parser stack, stack_top - stack position
 * RETURN: production index if match found, -1 otherwise
 * COMPLEXITY: O(r*n) where r is rule count, n is max RHS length
 */
int find_handle(Grammar *grammar, char *stack, int stack_top)
{
    for (int i = 0; i < grammar->production_count; i++) {
        int len = grammar->productions[i].rhs_len;
        if (stack_top >= len) {
            if (strncmp(&stack[stack_top - len], grammar->productions[i].rhs, len) == 0) {
                return i;
            }
        }
    }
    return -1;
}

/*
 * FUNCTION: shift_reduce_parse
 * PURPOSE: Perform shift-reduce parsing on input string
 * PARAMETERS: grammar - grammar, input - input string
 * RETURN: true if accepted, false otherwise
 * COMPLEXITY: O(n*p) where n is input length, p is production count
 */
bool shift_reduce_parse(Grammar *grammar, const char *input)
{
    char stack[MAX_STACK];
    int stack_top = 0;
    int input_pos = 0;

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("SHIFT-REDUCE PARSING TRACE\n");
    printf("------------------------------------------------------------------------\n");
    printf("%-25s %-25s %-20s\n", "Stack", "Input", "Action");
    printf("------------------------------------------------------------------------\n");

    while (true) {
        /* Display state */
        stack[stack_top] = '\0';
        printf("%-25s ", (stack_top == 0) ? "$" : stack);
        printf("%-25s ", &input[input_pos]);

        /* Check for reduce */
        int handle_prod = find_handle(grammar, stack, stack_top);
        if (handle_prod >= 0) {
            int len = grammar->productions[handle_prod].rhs_len;
            char lhs = grammar->productions[handle_prod].lhs;
            printf("Reduce %c->", lhs);
            printf("%s\n", grammar->productions[handle_prod].rhs);

            /* Remove RHS from stack, add LHS */
            stack_top -= len;
            stack[stack_top++] = lhs;

            continue;
        }

        /* Check for shift */
        if (input[input_pos] != '$' && input[input_pos] != '\0') {
            printf("Shift %c\n", input[input_pos]);
            stack[stack_top++] = input[input_pos];
            if (input[input_pos] == 'd' && input[input_pos + 1] == '\0') {
                /* Handle "id" token */
                stack[stack_top++] = input[++input_pos];
            } else {
                input_pos++;
            }
            continue;
        }

        /* Check for accept */
        if (input[input_pos] == '$' || input[input_pos] == '\0') {
            if (stack_top == 1 && stack[0] == 'E') {
                printf("Accept\n");
                return true;
            } else if (stack_top == 1 && (stack[0] == 'T' || stack[0] == 'F')) {
                printf("Accept (T/F also valid start)\n");
                return true;
            }
        }

        printf("Error\n");
        return false;
    }
}

/*
 * FUNCTION: display_grammar
 * PURPOSE: Display all grammar productions
 * PARAMETERS: grammar - grammar to display
 * RETURN: void
 * COMPLEXITY: O(p*n) where p is production count, n is avg RHS length
 */
void display_grammar(Grammar *grammar)
{
    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("GRAMMAR PRODUCTIONS\n");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < grammar->production_count; i++) {
        printf("  %d. %c -> %s\n", i, grammar->productions[i].lhs, grammar->productions[i].rhs);
    }
    printf("------------------------------------------------------------------------\n");
}

/* ======================== Main Program ======================== */

int main(void)
{
    /*
     * EXAMPLE USED:
     * Input: id*id+id
     * Grammar:
     * E -> E+T | T
     * T -> T*F | F
     * F -> (E) | id
     */

    printf("\n");
    printf("========================================================================\n");
    printf("         SHIFT-REDUCE  PARSING\n");
    printf("         COMPILER DESIGN LABORATORY\n");
    printf("========================================================================\n");

    Grammar grammar;
    initialize_grammar(&grammar);

    display_grammar(&grammar);

    const char *input = "id*id+id$";
    printf("\nInput string: %s\n", input);
    printf("\nParsing process:\n");

    bool accepted = shift_reduce_parse(&grammar, input);

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    if (accepted) {
        printf("Parsing Result: ACCEPTED\n");
    } else {
        printf("Parsing Result: REJECTED\n");
    }
    printf("------------------------------------------------------------------------\n");

    printf("\nStatus: Shift-reduce parsing completed.\n");

    return 0;
}
