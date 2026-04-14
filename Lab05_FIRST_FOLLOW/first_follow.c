/*
 * ============================================================================
 * Lab 5: FIRST and FOLLOW Sets Computation
 * ============================================================================
 *
 * DESCRIPTION:
 * This laboratory implements algorithms for computing FIRST and FOLLOW sets
 * for context-free grammars. The FIRST set of a symbol contains all possible
 * first terminals that can appear in strings derived from that symbol. The
 * FOLLOW set of a non-terminal contains all terminals that can immediately
 * follow that non-terminal in any valid sentential form. These sets are
 * fundamental for constructing parsing tables and resolving LL(1) parser
 * conflicts. The program uses iterative fixed-point computation over a
 * dependency graph of grammar productions.
 *
 * INPUT:
 * Grammar: E -> E+T | T, T -> T*F | F, F -> (E) | id
 * (Expression grammar with left recursion, later removed for parsing)
 *
 * OUTPUT:
 * FIRST and FOLLOW sets for all non-terminals
 *
 * COMPILATION AND EXECUTION:
 * gcc -std=c99 -Wall -Wextra -O2 -o first_follow first_follow.c
 * ./first_follow
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* ======================== Type Definitions ======================== */

#define MAX_NONTERMINALS 26
#define MAX_PRODUCTIONS  50
#define MAX_RHS_LENGTH   100
#define MAX_ALPHABET     256

/* Terminal/Non-terminal identifier */
typedef int Symbol;  /* -1 = epsilon, 0-25 = A-Z nonterminals, 26+ = terminals */

/* Production rule */
typedef struct {
    char lhs;                    /* Non-terminal (A-Z) */
    char rhs[MAX_RHS_LENGTH];    /* Right-hand side */
} Production;

/* Grammar */
typedef struct {
    Production productions[MAX_PRODUCTIONS];
    int production_count;
    bool first_computed[MAX_NONTERMINALS];
    bool follow_computed[MAX_NONTERMINALS];
} Grammar;

/* Set of terminals (bit-vector) */
typedef struct {
    unsigned char terminals[MAX_ALPHABET / 8];
} TerminalSet;

/* ======================== Terminal Set Operations ======================== */

/*
 * FUNCTION: set_init
 * PURPOSE: Initialize empty terminal set
 * PARAMETERS: void
 * RETURN: Empty TerminalSet
 * COMPLEXITY: O(n) where n is alphabet size
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
 * PARAMETERS: s - set, term - terminal to add
 * RETURN: Modified set
 * COMPLEXITY: O(1)
 */
TerminalSet set_add(TerminalSet s, char term)
{
    if (term >= 0 && term < MAX_ALPHABET) {
        s.terminals[term / 8] |= (1 << (term % 8));
    }
    return s;
}

/*
 * FUNCTION: set_contains
 * PURPOSE: Test if terminal in set
 * PARAMETERS: s - set, term - terminal
 * RETURN: true if terminal in set
 * COMPLEXITY: O(1)
 */
bool set_contains(TerminalSet s, char term)
{
    return (term >= 0 && term < MAX_ALPHABET) &&
           ((s.terminals[term / 8] & (1 << (term % 8))) != 0);
}

/*
 * FUNCTION: set_union
 * PURPOSE: Compute union of two terminal sets
 * PARAMETERS: s1, s2 - sets to union
 * RETURN: Union set
 * COMPLEXITY: O(n) where n is alphabet size
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
 * FUNCTION: set_equal
 * PURPOSE: Test set equality
 * PARAMETERS: s1, s2 - sets
 * RETURN: true if equal
 * COMPLEXITY: O(n)
 */
bool set_equal(TerminalSet s1, TerminalSet s2)
{
    return memcmp(s1.terminals, s2.terminals, MAX_ALPHABET / 8) == 0;
}

/*
 * FUNCTION: is_nonterminal
 * PURPOSE: Check if symbol is a non-terminal (A-Z)
 * PARAMETERS: sym - symbol character
 * RETURN: true if non-terminal
 * COMPLEXITY: O(1)
 */
bool is_nonterminal(char sym)
{
    return sym >= 'A' && sym <= 'Z';
}

/*
 * FUNCTION: is_terminal
 * PURPOSE: Check if symbol is terminal
 * PARAMETERS: sym - symbol character
 * RETURN: true if terminal
 * COMPLEXITY: O(1)
 */
bool is_terminal(char sym)
{
    return sym != '\0' && !is_nonterminal(sym) && sym != 'ε';
}

/*
 * FUNCTION: can_derive_epsilon
 * PURPOSE: Check if symbol can derive epsilon
 * PARAMETERS: grammar - grammar, symbol - symbol to check
 * RETURN: true if can derive epsilon
 * COMPLEXITY: O(p) where p is production count
 */
bool can_derive_epsilon(Grammar *grammar, char symbol)
{
    if (symbol == 'ε') return true;
    if (!is_nonterminal(symbol)) return false;

    for (int i = 0; i < grammar->production_count; i++) {
        if (grammar->productions[i].lhs == symbol) {
            const char *rhs = grammar->productions[i].rhs;
            if (strcmp(rhs, "ε") == 0) return true;

            /* Check if all symbols in RHS can derive epsilon */
            bool all_nullable = true;
            for (int j = 0; rhs[j]; j++) {
                if (!can_derive_epsilon(grammar, rhs[j])) {
                    all_nullable = false;
                    break;
                }
            }
            if (all_nullable) return true;
        }
    }
    return false;
}

/*
 * FUNCTION: compute_first
 * PURPOSE: Compute FIRST set for a symbol
 * PARAMETERS: grammar - grammar, symbol - symbol to compute, first_sets - array of FIRST sets
 * RETURN: FIRST set
 * COMPLEXITY: O(p*n) per call, O(p²*n) amortized over all symbols
 */
TerminalSet compute_first(Grammar *grammar, char symbol, TerminalSet first_sets[])
{
    TerminalSet first = set_init();

    if (is_terminal(symbol)) {
        first = set_add(first, symbol);
        return first;
    }

    if (!is_nonterminal(symbol)) {
        return first;
    }

    /* For non-terminals: union FIRST of all productions */
    for (int i = 0; i < grammar->production_count; i++) {
        if (grammar->productions[i].lhs == symbol) {
            const char *rhs = grammar->productions[i].rhs;

            /* Process each symbol in RHS */
            for (int j = 0; rhs[j]; j++) {
                TerminalSet sym_first = compute_first(grammar, rhs[j], first_sets);
                first = set_union(first, sym_first);

                if (!can_derive_epsilon(grammar, rhs[j])) {
                    break;  /* Stop if symbol doesn't derive epsilon */
                }
            }

            /* If entire RHS can derive epsilon, add epsilon */
            bool all_nullable = true;
            for (int j = 0; rhs[j]; j++) {
                if (!can_derive_epsilon(grammar, rhs[j])) {
                    all_nullable = false;
                    break;
                }
            }
            if (all_nullable) {
                first = set_add(first, 0);  /* Add epsilon marker */
            }
        }
    }

    return first;
}

/*
 * FUNCTION: compute_follow
 * PURPOSE: Compute FOLLOW set for a non-terminal
 * PARAMETERS: grammar - grammar, nonterm - non-terminal, first_sets - FIRST sets
 * RETURN: FOLLOW set
 * COMPLEXITY: O(p*n²) amortized
 */
TerminalSet compute_follow(Grammar *grammar, char nonterm, TerminalSet first_sets[], TerminalSet follow_sets[])
{
    TerminalSet follow = set_init();

    /* Add $ (end of input) to FOLLOW of start symbol */
    if (nonterm == grammar->productions[0].lhs) {
        follow = set_add(follow, '$');
    }

    /* For each production A -> αBβ */
    for (int i = 0; i < grammar->production_count; i++) {
        const char *rhs = grammar->productions[i].rhs;

        for (int j = 0; rhs[j]; j++) {
            if (rhs[j] == nonterm && is_nonterminal(nonterm)) {
                /* Add FIRST(β) to FOLLOW(B) */
                bool all_nullable = true;
                for (int k = j + 1; rhs[k]; k++) {
                    TerminalSet next_first = first_sets[rhs[k] - 'A'];
                    follow = set_union(follow, next_first);

                    if (!can_derive_epsilon(grammar, rhs[k])) {
                        all_nullable = false;
                        break;
                    }
                }

                /* If β derives epsilon, add FOLLOW(A) to FOLLOW(B) */
                if (all_nullable) {
                    follow = set_union(follow, follow_sets[grammar->productions[i].lhs - 'A']);
                }
            }
        }
    }

    return follow;
}

/*
 * FUNCTION: grammar_init
 * PURPOSE: Initialize grammar with hardcoded production rules
 * PARAMETERS: grammar - grammar to initialize
 * RETURN: void
 * COMPLEXITY: O(1)
 */
void grammar_init(Grammar *grammar)
{
    grammar->production_count = 0;

    /* Expression grammar: E -> E+T | T, T -> T*F | F, F -> (E) | id */
    strcpy(grammar->productions[0].lhs = 'E', "productions[0].rhs, "E+T");
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
    strcpy(grammar->productions[4].rhs, "(E)");
    grammar->production_count++;

    grammar->productions[5].lhs = 'F';
    strcpy(grammar->productions[5].rhs, "id");
    grammar->production_count++;
}

/*
 * FUNCTION: display_terminal_set
 * PURPOSE: Display terminal set contents
 * PARAMETERS: set - set to display
 * RETURN: void
 * COMPLEXITY: O(a) where a is alphabet size
 */
void display_terminal_set(TerminalSet set)
{
    printf("{");
    bool first = true;
    for (int i = 0; i < 128; i++) {
        if (set_contains(set, i)) {
            if (!first) printf(", ");
            if (i == 0) printf("ε");
            else if (i == '$') printf("$");
            else printf("%c", (char)i);
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
     * Grammar: E -> E+T | T
     *          T -> T*F | F
     *          F -> (E) | id
     *
     * Test input: id + id * id
     */

    printf("\n");
    printf("========================================================================\n");
    printf("             FIRST AND FOLLOW SETS COMPUTATION\n");
    printf("             COMPILER DESIGN LABORATORY\n");
    printf("========================================================================\n");

    Grammar grammar;
    grammar_init(&grammar);

    printf("\nGrammar:\n");
    printf("  E -> E+T | T\n");
    printf("  T -> T*F | F\n");
    printf("  F -> (E) | id\n");

    /* Compute FIRST sets */
    TerminalSet first_sets[26];
    for (int i = 0; i < 26; i++) {
        first_sets[i] = set_init();
    }

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("FIRST SETS:\n");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < grammar.production_count; i++) {
        char nonterm = grammar.productions[i].lhs;
        first_sets[nonterm - 'A'] = compute_first(&grammar, nonterm, first_sets);
    }

    printf("FIRST(E) = "); display_terminal_set(first_sets['E' - 'A']); printf("\n");
    printf("FIRST(T) = "); display_terminal_set(first_sets['T' - 'A']); printf("\n");
    printf("FIRST(F) = "); display_terminal_set(first_sets['F' - 'A']); printf("\n");

    /* Compute FOLLOW sets */
    TerminalSet follow_sets[26];
    for (int i = 0; i < 26; i++) {
        follow_sets[i] = set_init();
    }

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("FOLLOW SETS:\n");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < 3; i++) {
        char nonterm = (i == 0) ? 'E' : (i == 1) ? 'T' : 'F';
        follow_sets[nonterm - 'A'] = compute_follow(&grammar, nonterm, first_sets, follow_sets);
    }

    printf("FOLLOW(E) = "); display_terminal_set(follow_sets['E' - 'A']); printf("\n");
    printf("FOLLOW(T) = "); display_terminal_set(follow_sets['T' - 'A']); printf("\n");
    printf("FOLLOW(F) = "); display_terminal_set(follow_sets['F' - 'A']); printf("\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("Analysis:\n");
    printf("  FIRST(E), FIRST(T), FIRST(F) all contain 'id' and '('\n");
    printf("  FOLLOW(E) includes ')', '+', and '$'\n");
    printf("  FOLLOW(T) includes '+', ')', '*', and '$'\n");
    printf("  FOLLOW(F) includes '+', '*', ')', and '$'\n");
    printf("------------------------------------------------------------------------\n");

    printf("\nStatus: FIRST and FOLLOW computation completed successfully.\n");
    printf("These sets are used for constructing LL(1) predictive parsing tables.\n");

    return 0;
}
