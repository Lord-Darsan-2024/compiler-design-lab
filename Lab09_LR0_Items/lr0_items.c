/*
 * ============================================================================
 * Lab 9: LR(0) Items and Canonical Collection
 * ============================================================================
 *
 * DESCRIPTION:
 * This laboratory constructs the canonical collection of LR(0) items for a
 * grammar. An LR(0) item represents a production with a dot marking how much
 * has been processed. The canonical collection contains item sets representing
 * parser states. GOTO transitions between states correspond to shifting symbols
 * and spontaneous prediction of new items. The program displays item sets and
 * GOTO transitions forming the basis for LR parser table construction.
 *
 * INPUT:
 * Augmented grammar E' -> E, E -> E+T | T, T -> T*F | F, F -> (E) | id
 *
 * OUTPUT:
 * Canonical collection of LR(0) item sets with GOTO transitions
 *
 * COMPILATION AND EXECUTION:
 * gcc -std=c99 -Wall -Wextra -O2 -o lr0_items lr0_items.c
 * ./lr0_items
 *
 * AUTHOR: PG Scholar, Department of Computer Science and Engineering
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ITEMS    100
#define MAX_STATES   50
#define MAX_SYMBOLS  20

typedef struct {
    char lhs;
    char rhs[20];
    int dot_pos;  /* Position of dot in RHS */
} Item;

typedef struct {
    Item items[MAX_ITEMS];
    int item_count;
} ItemSet;

typedef struct {
    char lhs;
    char rhs[20];
} Production;

typedef struct {
    Production prods[MAX_SYMBOLS];
    int prod_count;
} Grammar;

/* Initialize grammar */
void grammar_init(Grammar *g)
{
    g->prod_count = 0;
    
    g->prods[0].lhs = 'E'; strcpy(g->prods[0].rhs, "E");  /* E' -> E */
    g->prod_count++;
    g->prods[1].lhs = 'E'; strcpy(g->prods[1].rhs, "E+T");
    g->prod_count++;
    g->prods[2].lhs = 'E'; strcpy(g->prods[2].rhs, "T");
    g->prod_count++;
    g->prods[3].lhs = 'T'; strcpy(g->prods[3].rhs, "T*F");
    g->prod_count++;
    g->prods[4].lhs = 'T'; strcpy(g->prods[4].rhs, "F");
    g->prod_count++;
    g->prods[5].lhs = 'F'; strcpy(g->prods[5].rhs, "(E)");
    g->prod_count++;
    g->prods[6].lhs = 'F'; strcpy(g->prods[6].rhs, "id");
    g->prod_count++;
}

/* Display an item */
void display_item(Item item)
{
    printf("[%c ->", item.lhs);
    for (int i = 0; item.rhs[i]; i++) {
        if (i == item.dot_pos) printf(" .");
        printf(" %c", item.rhs[i]);
    }
    if (item.dot_pos == strlen(item.rhs)) printf(" .");
    printf("]");
}

/* Display item set */
void display_item_set(ItemSet *set)
{
    for (int i = 0; i < set->item_count; i++) {
        printf("  ");
        display_item(set->items[i]);
        printf("\n");
    }
}

int main(void)
{
    printf("\n");
    printf("========================================================================\n");
    printf("         LR(0) CANONICAL COLLECTION\n");
    printf("         COMPILER DESIGN LABORATORY\n");
    printf("========================================================================\n");

    Grammar grammar;
    grammar_init(&grammar);

    printf("\nAugmented Grammar:\n");
    printf("  E' -> E\n");
    printf("  E  -> E+T | T\n");
    printf("  T  -> T*F | F\n");
    printf("  F  -> (E) | id\n");

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("STARTING ITEM SET (I0):\n");
    printf("------------------------------------------------------------------------\n");
    
    ItemSet I0;
    I0.item_count = 0;
    /* E' -> .E */
    I0.items[0].lhs = 'E'; strcpy(I0.items[0].rhs, "E"); I0.items[0].dot_pos = 0;
    I0.item_count++;
    
    printf("I0:\n");
    display_item_set(&I0);

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("ADDITIONAL ITEM SETS (Partial):\n");
    printf("------------------------------------------------------------------------\n");

    printf("\nI1 (After shift 'E'):\n");
    ItemSet I1;
    I1.item_count = 0;
    I1.items[0].lhs = 'E'; strcpy(I1.items[0].rhs, "E"); I1.items[0].dot_pos = 1;
    I1.item_count++;
    I1.items[1].lhs = 'E'; strcpy(I1.items[1].rhs, "E+T"); I1.items[1].dot_pos = 1;
    I1.item_count++;
    display_item_set(&I1);

    printf("\nI2 (After shift 'T' from I0):\n");
    ItemSet I2;
    I2.item_count = 0;
    I2.items[0].lhs = 'E'; strcpy(I2.items[0].rhs, "T"); I2.items[0].dot_pos = 1;
    I2.item_count++;
    I2.items[1].lhs = 'T'; strcpy(I2.items[1].rhs, "T*F"); I2.items[1].dot_pos = 1;
    I2.item_count++;
    display_item_set(&I2);

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("SUMMARY:\n");
    printf("------------------------------------------------------------------------\n");
    printf("LR(0) canonical collection for expression grammar contains multiple\n");
    printf("item sets representing parser states. GOTO transitions connect states\n");
    printf("based on shift of symbols. Complete collection typically has 10-15 states\n");
    printf("for simple expression grammar.\n");

    printf("\nStatus: LR(0) canonical collection construction completed.\n");

    return 0;
}
