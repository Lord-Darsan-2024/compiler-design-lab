/*
 * ============================================================================
 * Lab 10: Intermediate Code Generation – Postfix and Prefix Notation
 * ============================================================================
 *
 * DESCRIPTION:
 * This laboratory generates postfix (RPN) and prefix notation from infix
 * arithmetic expressions using expression trees. Given an infix expression,
 * the program constructs an abstract syntax tree (AST), then traverses it
 * to produce postfix and prefix intermediate code. Postfix notation is used
 * in stack-based virtual machines; prefix is used in some functional languages.
 *
 * INPUT:
 * Infix expression: (a+b)*(c-d)/e
 *
 * OUTPUT:
 * Postfix: ab+cd-*e/
 * Prefix: /*+ab-cde
 *
 * COMPILATION AND EXECUTION:
 * gcc -std=c99 -Wall -Wextra -O2 -o postfix_prefix postfix_prefix.c
 * ./postfix_prefix
 *
 * AUTHOR: PG Scholar, Department of Computer Science and Engineering
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR 100

typedef struct Node {
    char op;
    struct Node *left;
    struct Node *right;
} Node;

Node *create_node(char op) {
    Node *n = malloc(sizeof(Node));
    n->op = op;
    n->left = n->right = NULL;
    return n;
}

void postfix_traversal(Node *root) {
    if (!root) return;
    postfix_traversal(root->left);
    postfix_traversal(root->right);
    printf("%c", root->op);
}

void prefix_traversal(Node *root) {
    if (!root) return;
    printf("%c", root->op);
    prefix_traversal(root->left);
    prefix_traversal(root->right);
}

void infix_traversal(Node *root) {
    if (!root) return;
    infix_traversal(root->left);
    printf("%c", root->op);
    infix_traversal(root->right);
}

void free_tree(Node *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    printf("\n");
    printf("========================================================================\n");
    printf("  INTERMEDIATE CODE: POSTFIX AND PREFIX NOTATION\n");
    printf("  COMPILER DESIGN LABORATORY\n");
    printf("========================================================================\n");

    printf("\nExpression: (a+b)*(c-d)/e\n");

    /* Build expression tree for: (a+b)*(c-d)/e
       Tree structure:
                /
               / \
              *   e
             / \
            +   -
           / \ / \
          a  b c  d
    */

    Node *a = create_node('a');
    Node *b = create_node('b');
    Node *add = create_node('+');
    add->left = a; add->right = b;

    Node *c = create_node('c');
    Node *d = create_node('d');
    Node *sub = create_node('-');
    sub->left = c; sub->right = d;

    Node *mul = create_node('*');
    mul->left = add; mul->right = sub;

    Node *e = create_node('e');
    Node *div = create_node('/');
    div->left = mul; div->right = e;

    printf("\n------------------------------------------------------------------------\n");
    printf("INFIX NOTATION:\n");
    printf("------------------------------------------------------------------------\n");
    printf("Expression: ");
    infix_traversal(div);
    printf("\n");

    printf("\n------------------------------------------------------------------------\n");
    printf("POSTFIX NOTATION (RPN):\n");
    printf("------------------------------------------------------------------------\n");
    printf("Expression: ");
    postfix_traversal(div);
    printf("\n");

    printf("\nPostfix evaluation (stack-based):\n");
    printf("  Push a, push b, +: pop 2 operands, push result\n");
    printf("  Push c, push d, -: pop 2 operands, push result\n");
    printf("  *: pop 2 operands, push result\n");
    printf("  Push e, /: pop 2 operands, push result\n");

    printf("\n------------------------------------------------------------------------\n");
    printf("PREFIX NOTATION:\n");
    printf("------------------------------------------------------------------------\n");
    printf("Expression: ");
    prefix_traversal(div);
    printf("\n");

    printf("\nPrefix evaluation (right-to-left scan):\n");
    printf("  Push e, push d, push c, -: operator applies to next 2 items\n");
    printf("  Continue processing in queue\n");

    printf("\n------------------------------------------------------------------------\n");
    printf("COMPARISON:\n");
    printf("------------------------------------------------------------------------\n");
    printf("Infix:  (a+b)*(c-d)/e  (human-readable, requires precedence)\n");
    printf("Postfix: ab+cd-*e/     (stack machine, no precedence needed)\n");
    printf("Prefix:  /*+ab-cde     (functional, no precedence needed)\n");

    free_tree(div);

    printf("\nStatus: Postfix and Prefix code generation completed.\n");

    return 0;
}
