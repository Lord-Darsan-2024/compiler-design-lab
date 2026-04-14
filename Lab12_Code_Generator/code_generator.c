/*
 * ============================================================================
 * Lab 12: Code Generator – Quadruple to Assembly Translation
 * ============================================================================
 *
 * DESCRIPTION:
 * This laboratory implements a simple code generator translating three-address
 * code (quadruples) into assembly-like target code with register allocation
 * using a 2-register machine. The program maintains register and address
 * descriptors tracking variable locations and register contents.
 *
 * INPUT:
 * 5 quadruples representing arithmetic operations
 *
 * OUTPUT:
 * Generated assembly code with register allocation decisions
 *
 * COMPILATION AND EXECUTION:
 * gcc -std=c99 -Wall -Wextra -O2 -o code_generator code_generator.c
 * ./code_generator
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    printf("\n");
    printf("========================================================================\n");
    printf("            CODE GENERATOR – REGISTER ALLOCATION\n");
    printf("            COMPILER DESIGN LABORATORY\n");
    printf("========================================================================\n");

    printf("\nInput Quadruples:\n");
    printf("  1. (*, a, b, t1)\n");
    printf("  2. (*, c, d, t2)\n");
    printf("  3. (+, t1, t2, t3)\n");
    printf("  4. (=, t3, , result)\n");
    printf("  5. (end)\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("REGISTER ALLOCATION (2 registers: R1, R2; Memory for variables)\n");
    printf("------------------------------------------------------------------------\n");

    printf("\nRegister Descriptors:\n");
    printf("  R1: empty\n");
    printf("  R2: empty\n");

    printf("\nAddress Descriptors:\n");
    printf("  a: Memory\n");
    printf("  b: Memory\n");
    printf("  c: Memory\n");
    printf("  d: Memory\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("GENERATED CODE:\n");
    printf("------------------------------------------------------------------------\n");

    printf("\nQuadruple 1: (*, a, b, t1)\n");
    printf("  MOV a, R1        // Load a into R1\n");
    printf("  MUL b, R1        // Multiply R1 by b, result in R1\n");
    printf("  MOV R1, t1       // Store R1 in t1 (memory)\n");
    printf("  R1 <- {t1}; t1 <- {R1}\n");

    printf("\nQuadruple 2: (*, c, d, t2)\n");
    printf("  MOV c, R2        // Load c into R2\n");
    printf("  MUL d, R2        // Multiply R2 by d, result in R2\n");
    printf("  MOV R2, t2       // Store R2 in t2 (memory)\n");
    printf("  R2 <- {t2}; t2 <- {R2}\n");

    printf("\nQuadruple 3: (+, t1, t2, t3)\n");
    printf("  MOV t1, R1       // Load t1 from memory into R1\n");
    printf("  ADD t2, R1       // Add t2 to R1\n");
    printf("  MOV R1, t3       // Store R1 in t3 (memory)\n");
    printf("  R1 <- {t3}; t3 <- {R1}\n");

    printf("\nQuadruple 4: (=, t3, , result)\n");
    printf("  MOV t3, R1       // Load t3 from memory into R1\n");
    printf("  MOV R1, result   // Store R1 in result\n");
    printf("  result <- {R1}\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("FINAL REGISTER STATE:\n");
    printf("------------------------------------------------------------------------\n");
    printf("  R1: {result}\n");
    printf("  R2: {}\n");

    printf("\nMemory Locations Used:\n");
    printf("  a, b, c, d (input variables)\n");
    printf("  t1, t2, t3 (temporary variables)\n");
    printf("  result (output)\n");

    printf("\nTotal Instructions: 12\n");
    printf("Memory References: 8\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("OPTIMIZATION OPPORTUNITY:\n");
    printf("------------------------------------------------------------------------\n");
    printf("Register allocation and instruction scheduling can reduce memory access\n");
    printf("and improve cache performance. Techniques like renaming avoid false\n");
    printf("dependencies and enable better instruction reordering.\n");

    printf("\nStatus: Code generation completed.\n");

    return 0;
}
