/*
 * ============================================================================
 * Lab 8: LEADING and TRAILING Sets Computation
 * ============================================================================
 *
 * DESCRIPTION:
 * This laboratory implements computation of LEADING and TRAILING sets for
 * operator grammars. LEADING(A) contains terminals that can appear immediately
 * before A in any sentential form. TRAILING(A) contains terminals that can
 * appear immediately after A. These sets are used in operator-precedence
 * parsing to determine when to shift or reduce based on operators adjacent
 * to stack symbols. The algorithm uses iterative fixed-point computation
 * similar to FIRST/FOLLOW computation.
 *
 * INPUT:
 * Operator grammar: E -> E+T | T, T -> T*F | F, F -> id
 *
 * OUTPUT:
 * LEADING and TRAILING sets for each non-terminal
 *
 * COMPILATION AND EXECUTION:
 * gcc -std=c99 -Wall -Wextra -O2 -o leading_trailing leading_trailing.c
 * ./leading_trailing
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ======================== Type Definitions ======================== */

#define MAX_NONTERMINALS 26
#define MAX_PRODUCTIONS  50
#define MAX_RHS_LENGTH   100
#define MAX_ALPHABET     256

typedef struct {
    char lhs;
    char rhs[MAX_RHS_LENGTH];
} Production;

typedef struct {
    Production productions[MAX_PRODUCTIONS];
    int production_count;
} Grammar;

typedef struct {
    int terminals[MAX_ALPHABET / 8];
} TerminalSet;

/*
 * FUNCTION: set_init
 * PURPOSE: Initialize empty set
 * PARAMETERS: void
 * RETURN: TerminalSet
 * COMPLEXITY: O(n)
 */
TerminalSet set_init(void)
{
    TerminalSet s;
    memset(s.terminals, 0, sizeof(s.terminals));
    return s;
}

/*
 * FUNCTION: set_add
 * PURPOSE: Add terminal to set
 * PARAMETERS: s - set, term - terminal
 * RETURN: Modified set
 * COMPLEXITY: O(1)
 */
TerminalSet set_add(TerminalSet s, char term)
{
    if (term > 0 && term < MAX_ALPHABET) {
        s.terminals[term / 8] |= (1 << (term % 8));
    }
    return s;
}

/*
 * FUNCTION: set_contains
 * PURPOSE: Check membership
 * PARAMETERS: s - set, term - terminal
 * RETURN: bool
 * COMPLEXITY: O(1)
 */
bool set_contains(TerminalSet s, char term)
{
    return (term > 0 && term < MAX_ALPHABET) &&
           ((s.terminals[term / 8] & (1 << (term % 8))) != 0);
}

/*
 * FUNCTION: set_union
 * PURPOSE: Union two sets
 * PARAMETERS: s1, s2 - sets
 * RETURN: Union
 * COMPLEXITY: O(n)
 */
TerminalSet set_union(TerminalSet s1, TerminalSet s2)
{
    TerminalSet result = s1;
    for (int i = 0; i < MAX_ALPHABET / 8; i++) {
        result.terminals[i] |= s2.terminals[i];
    }
    return result;
}

/*
 * FUNCTION: compute_leading
 * PURPOSE: Compute LEADING set for non-terminal
 * PARAMETERS: grammar - grammar, nonterm - non-terminal
 * RETURN: LEADING set
 * COMPLEXITY: O(p²) amortized
 */
TerminalSet compute_leading(Grammar *grammar, char nonterm)
{
    TerminalSet leading = set_init();

    for (int i = 0; i < grammar->production_count; i++) {
        if (grammar->productions[i].lhs == nonterm) {
            const char *rhs = grammar->productions[i].rhs;
            if (rhs[0] >= 'A' && rhs[0] <= 'Z') {
                /* Non-terminal: recursively get its leading */
                if (rhs[0] != nonterm) {  /* Avoid infinite loop */
                    leading = set_union(leading, compute_leading(grammar, rhs[0]));
                }
            } else {
                /* Terminal: add directly */
                leading = set_add(leading, rhs[0]);
            }
        }
    }

    return leading;
}

/*
 * FUNCTION: compute_trailing
 * PURPOSE: Compute TRAILING set for non-terminal
 * PARAMETERS: grammar - grammar, nonterm - non-terminal
 * RETURN: TRAILING set
 * COMPLEXITY: O(p²) amortized
 */
TerminalSet compute_trailing(Grammar *grammar, char nonterm)
{
    TerminalSet trailing = set_init();

    for (int i = 0; i < grammar->production_count; i++) {
        const char *rhs = grammar->productions[i].rhs;
        int len = strlen(rhs);

        for (int j = 0; j < len; j++) {
            if (rhs[j] == nonterm) {
                /* Found nonterm in production */
                if (j + 1 < len) {
                    if (rhs[j + 1] >= 'A' && rhs[j + 1] <= 'Z') {
                        /* Non-terminal follows */
                        if (rhs[j + 1] != nonterm) {
                            trailing = set_union(trailing, compute_leading(grammar, rhs[j + 1]));
                        }
                    } else {
                        /* Terminal follows */
                        trailing = set_add(trailing, rhs[j + 1]);
                    }
                }
            }
        }
    }

    return trailing;
}

/*
 * FUNCTION: grammar_init
 * PURPOSE: Load example grammar
 * PARAMETERS: grammar - grammar to initialize
 * RETURN: void
 * COMPLEXITY: O(1)
 */
void grammar_init(Grammar *grammar)
{
    grammar->production_count = 0;

    grammar->productions[0].lhs = 'E';
    strcpy(grammar->productions[0].rhs, "E+T");
    grammar->production_count++;

    grammar->productions[1].lhs = 'E';
    strcpy(grammar->productions[1].rhs, "T");
    grammar->production_count++;

    grammar->productions[2].lhs = 'T';
    strcpy(grammar->productions[2].rhs, "T*F");
    grammar->production_count++;

    grammar->productions[3].lhs = 'T';
    strcpy(grammar->productions[3].rhs, "F");
    grammar->production_count++;

    grammar->productions[4].lhs = 'F';
    strcpy(grammar->productions[4].rhs, "id");
    grammar->production_count++;
}

/*
 * FUNCTION: display_set
 * PURPOSE: Print set contents
 * PARAMETERS: s - set
 * RETURN: void
 * COMPLEXITY: O(n)
 */
void display_set(TerminalSet s)
{
    printf("{");
    bool first = true;
    for (int i = 32; i < 128; i++) {
        if (set_contains(s, i)) {
            if (!first) printf(", ");
            printf("%c", (char)i);
            first = false;
        }
    }
    printf("}");
}

/* ======================== Main Program ======================== */

int main(void)
{
    /*
     * EXAMPLE USED:
     * E -> E+T | T
     * T -> T*F | F
     * F -> id
     */

    printf("\n");
    printf("========================================================================\n");
    printf("         LEADING AND TRAILING SETS COMPUTATION\n");
    printf("         COMPILER DESIGN LABORATORY\n");
    printf("========================================================================\n");

    Grammar grammar;
    grammar_init(&grammar);

    printf("\nGrammar:\n");
    printf("  E -> E+T | T\n");
    printf("  T -> T*F | F\n");
    printf("  F -> id\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("LEADING SETS:\n");
    printf("------------------------------------------------------------------------\n");

    TerminalSet leading_E = compute_leading(&grammar, 'E');
    TerminalSet leading_T = compute_leading(&grammar, 'T');
    TerminalSet leading_F = compute_leading(&grammar, 'F');

    printf("LEADING(E) = "); display_set(leading_E); printf("\n");
    printf("LEADING(T) = "); display_set(leading_T); printf("\n");
    printf("LEADING(F) = "); display_set(leading_F); printf("\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("TRAILING SETS:\n");
    printf("------------------------------------------------------------------------\n");

    TerminalSet trailing_E = compute_trailing(&grammar, 'E');
    TerminalSet trailing_T = compute_trailing(&grammar, 'T');
    TerminalSet trailing_F = compute_trailing(&grammar, 'F');

    printf("TRAILING(E) = "); display_set(trailing_E); printf("\n");
    printf("TRAILING(T) = "); display_set(trailing_T); printf("\n");
    printf("TRAILING(F) = "); display_set(trailing_F); printf("\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("Analysis:\n");
    printf("  LEADING sets show what terminals can start derivations\n");
    printf("  TRAILING sets show what terminals can follow in productions\n");
    printf("  These are used in operator-precedence parsing\n");
    printf("------------------------------------------------------------------------\n");

    printf("\nStatus: LEADING and TRAILING computation completed.\n");

    return 0;
}
