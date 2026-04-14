/*
 * ============================================================================
 * Lab 11: Intermediate Code – Quadruple, Triple, Indirect Triple
 * ============================================================================
 *
 * DESCRIPTION:
 * This laboratory generates three intermediate code representations:
 * quadruples (4-addr), triples (3-addr), and indirect triples. All three
 * represent the same computation: a = b*c + b*c. Quadruples use explicit
 * result field; triples use (operator, arg1, arg2) with implicit result;
 * indirect triples add level of indirection for optimization.
 *
 * INPUT:
 * Expression: a = b*c + b*c
 *
 * OUTPUT:
 * Three representation forms as tables
 *
 * COMPILATION AND EXECUTION:
 * gcc -std=c99 -Wall -Wextra -O2 -o quad_triple_indirect quad_triple_indirect.c
 * ./quad_triple_indirect
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char operator[10];
    char arg1[10];
    char arg2[10];
    char result[10];
} Quadruple;

typedef struct {
    char operator[10];
    char arg1[10];
    char arg2[10];
} Triple;

int main(void) {
    printf("\n");
    printf("========================================================================\n");
    printf("  INTERMEDIATE CODE: QUADRUPLE, TRIPLE, INDIRECT TRIPLE\n");
    printf("  COMPILER DESIGN LABORATORY\n");
    printf("========================================================================\n");

    printf("\nExpression: a = b*c + b*c\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("1. QUADRUPLE REPRESENTATION (4-Address Code)\n");
    printf("------------------------------------------------------------------------\n");
    printf("   (Operator, Arg1, Arg2, Result)\n");
    printf("------------------------------------------------------------------------\n");

    Quadruple quads[] = {
        {"*",    "b",  "c",  "t1"},
        {"*",    "b",  "c",  "t2"},
        {"+",    "t1", "t2", "t3"},
        {"=",    "t3", "",   "a"},
    };

    printf("\n%-5s %-10s %-10s %-10s %-10s\n", "Line", "Op", "Arg1", "Arg2", "Result");
    printf("%-5s %-10s %-10s %-10s %-10s\n", "----", "--", "----", "----", "------");
    for (int i = 0; i < 4; i++) {
        printf("%-5d %-10s %-10s %-10s %-10s\n", i+1, quads[i].operator, 
               quads[i].arg1, quads[i].arg2, quads[i].result);
    }

    printf("\nAdvantage: Explicit result field\n");
    printf("Disadvantage: Code has redundancy (b*c computed twice)\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("2. TRIPLE REPRESENTATION (3-Address Code)\n");
    printf("------------------------------------------------------------------------\n");
    printf("   (Operator, Arg1, Arg2)\n");
    printf("   Results implicitly stored at index position\n");
    printf("------------------------------------------------------------------------\n");

    Triple trips[] = {
        {"*",    "b",    "c"},
        {"*",    "b",    "c"},
        {"+",    "(0)",  "(1)"},
        {"=",    "(2)",  ""},
    };

    printf("\n%-5s %-10s %-10s %-10s\n", "Line", "Op", "Arg1", "Arg2");
    printf("%-5s %-10s %-10s %-10s\n", "----", "--", "----", "----");
    for (int i = 0; i < 4; i++) {
        printf("%-5d %-10s %-10s %-10s  -> (%d)\n", i+1, trips[i].operator, 
               trips[i].arg1, trips[i].arg2, i);
    }

    printf("\nAdvantage: Compact, 3 fields\n");
    printf("Disadvantage: Code motion invalidates indices\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("3. INDIRECT TRIPLE REPRESENTATION\n");
    printf("------------------------------------------------------------------------\n");
    printf("   Separate statement table pointing to triples\n");
    printf("------------------------------------------------------------------------\n");

    printf("\nStatement Table:");
    printf("\n%-5s %-15s\n", "Stmt", "Triple Index");
    printf("%-5s %-15s\n", "----", "-------------");
    printf("%-5d %-15d\n", 1, 0);
    printf("%-5d %-15d\n", 2, 1);
    printf("%-5d %-15d\n", 3, 2);
    printf("%-5d %-15d\n", 4, 3);

    printf("\nTriple Table (as before):\n");
    printf("%-5s %-10s %-10s %-10s\n", "Index", "Op", "Arg1", "Arg2");
    printf("%-5s %-10s %-10s %-10s\n", "-----", "--", "----", "----");
    for (int i = 0; i < 4; i++) {
        printf("%-5d %-10s %-10s %-10s\n", i, trips[i].operator, 
               trips[i].arg1, trips[i].arg2);
    }

    printf("\nAdvantage: Enables code motion and optimization\n");
    printf("Disadvantage: Extra indirection level\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("OPTIMIZATION OPPORTUNITY: Common Subexpression Elimination\n");
    printf("------------------------------------------------------------------------\n");
    printf("b*c is computed twice (indices 0 and 1)\n");
    printf("Optimized code:\n");
    printf("  (0) (*, b, c)      -> t1\n");
    printf("  (1) (+, t1, t1)    -> t2    [reuse t1 instead of computing again]\n");
    printf("  (2) (=, t2, a)\n");

    printf("\nStatus: Three intermediate code forms completed.\n");

    return 0;
}
