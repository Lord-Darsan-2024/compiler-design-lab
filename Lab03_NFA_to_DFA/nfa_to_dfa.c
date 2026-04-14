/*
 * ============================================================================
 * Lab 3: NFA to DFA Conversion (Subset Construction)
 * ============================================================================
 *
 * DESCRIPTION:
 * This laboratory implements the Subset Construction algorithm to convert
 * a Non-deterministic Finite Automaton (NFA) into a Deterministic Finite
 * Automaton (DFA). The DFA produced accepts the same language as the input
 * NFA. Each DFA state corresponds to a subset of NFA states reachable via
 * epsilon closures. The algorithm processes input alphabets systematically,
 * computing DFA transitions from NFA epsilon closures and direct transitions.
 * This conversion is essential for implementing efficient lexical analyzers
 * and pattern matching engines with deterministic behavior.
 *
 * INPUT:
 * An NFA hardcoded for the language (a|b)*abb (from Lab 2)
 *
 * OUTPUT:
 * DFA transition table with states (as NFA state subsets), start state, accept states
 *
 * COMPILATION AND EXECUTION:
 * gcc -std=c99 -Wall -Wextra -O2 -o nfa_to_dfa nfa_to_dfa.c
 * ./nfa_to_dfa
 *
 * AUTHOR: PG Scholar, Department of Computer Science and Engineering
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ======================== Type Definitions ======================== */

#define MAX_NFA_STATES       50
#define MAX_DFA_STATES       128
#define MAX_ALPHABET         2
#define EPSILON              -1

/* NFA transition edge */
typedef struct {
    int from_state;
    int symbol;           /* 0='a', 1='b', -1=epsilon */
    int to_state;
} NFATransition;

/* Set of states (bit-vector representation) */
typedef struct {
    unsigned long states;  /* Bit-vector for set membership */
    int hash_id;           /* DFA state identifier */
} StateSet;

/* DFA state */
typedef struct {
    StateSet nfa_states;
    int dfa_id;
    bool is_accept;
    int transitions[MAX_ALPHABET];  /* DFA transitions on 'a' (0), 'b' (1) */
} DFAState;

/* NFA specification */
typedef struct {
    NFATransition transitions[MAX_NFA_STATES * 4];
    int transition_count;
    int start_state;
    int accept_state;
    int state_count;
} NFASpec;

/* DFA specification */
typedef struct {
    DFAState states[MAX_DFA_STATES];
    int state_count;
    int start_state_id;
} DFASpec;

/*
 * FUNCTION: set_create
 * PURPOSE: Create initial empty set
 * PARAMETERS: void
 * RETURN: Empty StateSet
 * COMPLEXITY: O(1)
 */
StateSet set_create(void)
{
    StateSet s;
    s.states = 0;
    s.hash_id = -1;
    return s;
}

/*
 * FUNCTION: set_add
 * PURPOSE: Add state to set
 * PARAMETERS: s - set, state - state to add
 * RETURN: Modified set
 * COMPLEXITY: O(1)
 */
StateSet set_add(StateSet s, int state)
{
    if (state >= 0 && state < MAX_NFA_STATES) {
        s.states |= (1UL << state);
    }
    return s;
}

/*
 * FUNCTION: set_contains
 * PURPOSE: Check if state is in set
 * PARAMETERS: s - set, state - state to check
 * RETURN: true if state in set
 * COMPLEXITY: O(1)
 */
bool set_contains(StateSet s, int state)
{
    return (state >= 0 && state < MAX_NFA_STATES) && 
           ((s.states & (1UL << state)) != 0);
}

/*
 * FUNCTION: set_equal
 * PURPOSE: Check if two sets are equal
 * PARAMETERS: s1, s2 - sets to compare
 * RETURN: true if sets identical
 * COMPLEXITY: O(1)
 */
bool set_equal(StateSet s1, StateSet s2)
{
    return s1.states == s2.states;
}

/*
 * FUNCTION: epsilon_closure
 * PURPOSE: Compute epsilon closure of a set of NFA states
 * PARAMETERS: nfa - NFA specification, states - input state set
 * RETURN: StateSet including all epsilon-reachable states
 * COMPLEXITY: O(n*m) where n is state count, m is transition count
 */
StateSet epsilon_closure(NFASpec *nfa, StateSet states)
{
    StateSet closure = states;
    bool changed = true;

    while (changed) {
        changed = false;
        for (int i = 0; i < nfa->transition_count; i++) {
            if (nfa->transitions[i].symbol == EPSILON &&
                set_contains(closure, nfa->transitions[i].from_state) &&
                !set_contains(closure, nfa->transitions[i].to_state)) {
                closure = set_add(closure, nfa->transitions[i].to_state);
                changed = true;
            }
        }
    }
    return closure;
}

/*
 * FUNCTION: move
 * PURPOSE: Compute NFA states reachable via symbol from a state set
 * PARAMETERS: nfa - NFA, states - current state set, symbol - input symbol (0=a, 1=b)
 * RETURN: Set of reachable NFA states via given symbol
 * COMPLEXITY: O(t) where t is transition count
 */
StateSet move(NFASpec *nfa, StateSet states, int symbol)
{
    StateSet result = set_create();
    for (int i = 0; i < nfa->transition_count; i++) {
        if (nfa->transitions[i].symbol == symbol &&
            set_contains(states, nfa->transitions[i].from_state)) {
            result = set_add(result, nfa->transitions[i].to_state);
        }
    }
    return result;
}

/*
 * FUNCTION: nfa_init_hardcoded
 * PURPOSE: Initialize hardcoded NFA for (a|b)*abb (simplified version)
 * PARAMETERS: nfa - NFA to initialize
 * RETURN: void
 * COMPLEXITY: O(1)
 */
void nfa_init_hardcoded(NFASpec *nfa)
{
    nfa->state_count = 8;
    nfa->start_state = 0;
    nfa->accept_state = 7;
    nfa->transition_count = 0;

    /* Simplified NFA for (a|b)*abb: 
       0 --eps--> 1, 4
       1 --eps--> 2, 3
       2 --a--> 0
       3 --b--> 0
       4 --a--> 5
       5 --b--> 6
       6 --b--> 7 (accept)
    */

    int trans_idx = 0;

    /* Start state epsilon transitions */
    nfa->transitions[trans_idx].from_state = 0;
    nfa->transitions[trans_idx].symbol = EPSILON;
    nfa->transitions[trans_idx].to_state = 1;
    trans_idx++;

    nfa->transitions[trans_idx].from_state = 0;
    nfa->transitions[trans_idx].symbol = EPSILON;
    nfa->transitions[trans_idx].to_state = 4;
    trans_idx++;

    /* (a|b) part */
    nfa->transitions[trans_idx].from_state = 1;
    nfa->transitions[trans_idx].symbol = EPSILON;
    nfa->transitions[trans_idx].to_state = 2;
    trans_idx++;

    nfa->transitions[trans_idx].from_state = 1;
    nfa->transitions[trans_idx].symbol = EPSILON;
    nfa->transitions[trans_idx].to_state = 3;
    trans_idx++;

    nfa->transitions[trans_idx].from_state = 2;
    nfa->transitions[trans_idx].symbol = 0;  /* 'a' */
    nfa->transitions[trans_idx].to_state = 0;
    trans_idx++;

    nfa->transitions[trans_idx].from_state = 3;
    nfa->transitions[trans_idx].symbol = 1;  /* 'b' */
    nfa->transitions[trans_idx].to_state = 0;
    trans_idx++;

    /* abb part */
    nfa->transitions[trans_idx].from_state = 4;
    nfa->transitions[trans_idx].symbol = 0;  /* 'a' */
    nfa->transitions[trans_idx].to_state = 5;
    trans_idx++;

    nfa->transitions[trans_idx].from_state = 5;
    nfa->transitions[trans_idx].symbol = 1;  /* 'b' */
    nfa->transitions[trans_idx].to_state = 6;
    trans_idx++;

    nfa->transitions[trans_idx].from_state = 6;
    nfa->transitions[trans_idx].symbol = 1;  /* 'b' */
    nfa->transitions[trans_idx].to_state = 7;
    trans_idx++;

    nfa->transition_count = trans_idx;
}

/*
 * FUNCTION: dfa_init
 * PURPOSE: Initialize empty DFA
 * PARAMETERS: dfa - DFA to initialize
 * RETURN: void
 * COMPLEXITY: O(1)
 */
void dfa_init(DFASpec *dfa)
{
    dfa->state_count = 0;
    dfa->start_state_id = -1;
}

/*
 * FUNCTION: add_dfa_state
 * PURPOSE: Add new DFA state to table
 * PARAMETERS: dfa - DFA, nfa_states - NFA state subset, is_accept - acceptance flag
 * RETURN: DFA state identifier
 * COMPLEXITY: O(1)
 */
int add_dfa_state(DFASpec *dfa, StateSet nfa_states, bool is_accept)
{
    DFAState *dfa_state = &dfa->states[dfa->state_count];
    dfa_state->nfa_states = nfa_states;
    dfa_state->dfa_id = dfa->state_count;
    dfa_state->is_accept = is_accept;
    for (int i = 0; i < MAX_ALPHABET; i++) {
        dfa_state->transitions[i] = -1;
    }
    return dfa->state_count++;
}

/*
 * FUNCTION: find_dfa_state
 * PURPOSE: Find DFA state corresponding to NFA state subset
 * PARAMETERS: dfa - DFA, nfa_states - state subset
 * RETURN: DFA state ID if found, -1 otherwise
 * COMPLEXITY: O(d) where d is DFA state count
 */
int find_dfa_state(DFASpec *dfa, StateSet nfa_states)
{
    for (int i = 0; i < dfa->state_count; i++) {
        if (set_equal(dfa->states[i].nfa_states, nfa_states)) {
            return i;
        }
    }
    return -1;
}

/*
 * FUNCTION: subset_construction
 * PURPOSE: Convert NFA to DFA using subset construction
 * PARAMETERS: nfa - input NFA, dfa - output DFA
 * RETURN: void
 * COMPLEXITY: O(2^n * m) in worst case, O(n*m) in practice
 */
void subset_construction(NFASpec *nfa, DFASpec *dfa)
{
    dfa_init(dfa);

    /* Queue for BFS processing */
    StateSet queue[MAX_DFA_STATES];
    int queue_head = 0, queue_tail = 0;

    /* Compute epsilon closure of NFA start state */
    StateSet start_set = set_create();
    start_set = set_add(start_set, nfa->start_state);
    start_set = epsilon_closure(nfa, start_set);

    /* Add DFA start state */
    bool is_accept = set_contains(start_set, nfa->accept_state);
    int dfa_start_id = add_dfa_state(dfa, start_set, is_accept);
    dfa->start_state_id = dfa_start_id;

    queue[queue_tail++] = start_set;

    /* BFS to construct DFA states and transitions */
    while (queue_head < queue_tail) {
        StateSet current = queue[queue_head];
        int current_dfa_id = find_dfa_state(dfa, current);
        queue_head++;

        /* For each symbol in alphabet */
        for (int symbol = 0; symbol < MAX_ALPHABET; symbol++) {
            /* Compute move(current, symbol) and epsilon_closure */
            StateSet moved = move(nfa, current, symbol);
            if (moved.states == 0) continue;  /* No transition */

            StateSet next = epsilon_closure(nfa, moved);

            /* Check if this DFA state already exists */
            int next_dfa_id = find_dfa_state(dfa, next);
            if (next_dfa_id == -1) {
                /* New DFA state */
                bool next_is_accept = set_contains(next, nfa->accept_state);
                next_dfa_id = add_dfa_state(dfa, next, next_is_accept);
                queue[queue_tail++] = next;
            }

            /* Add DFA transition */
            dfa->states[current_dfa_id].transitions[symbol] = next_dfa_id;
        }
    }
}

/*
 * FUNCTION: display_dfa_table
 * PURPOSE: Display DFA transition table in formatted output
 * PARAMETERS: dfa - DFA to display
 * RETURN: void
 * COMPLEXITY: O(d * a) where d is DFA states, a is alphabet size
 */
void display_dfa_table(DFASpec *dfa)
{
    printf("\n");
    printf("================================================================\n");
    printf("            DFA TRANSITION TABLE\n");
    printf("================================================================\n");
    printf("%-12s %-15s %-15s %-15s\n", "DFA State", "On 'a'", "On 'b'", "Accept?");
    printf("----------------------------------------------------------------\n");

    for (int i = 0; i < dfa->state_count; i++) {
        DFAState *state = &dfa->states[i];
        printf("%-12d ", i);
        printf("%-15s ", (state->transitions[0] >= 0) ? "→" : "-");
        if (state->transitions[0] >= 0) {
            printf("%d", state->transitions[0]);
        }
        printf("              ");
        printf("%-15s ", (state->transitions[1] >= 0) ? "→" : "-");
        if (state->transitions[1] >= 0) {
            printf("%d", state->transitions[1]);
        }
        printf("              ");
        printf("%-15s\n", state->is_accept ? "YES" : "NO");
    }

    printf("----------------------------------------------------------------\n");
    printf("Start State: %d\n", dfa->start_state_id);
    printf("Total DFA States: %d\n", dfa->state_count);
    printf("================================================================\n\n");
}

/*
 * FUNCTION: display_dfa_transitions_detailed
 * PURPOSE: Display DFA transitions in detailed format
 * PARAMETERS: dfa - DFA to display
 * RETURN: void
 * COMPLEXITY: O(d * a)
 */
void display_dfa_transitions_detailed(DFASpec *dfa)
{
    printf("\n");
    printf("================================================================\n");
    printf("            DFA TRANSITIONS (DETAILED)\n");
    printf("================================================================\n");

    for (int i = 0; i < dfa->state_count; i++) {
        DFAState *state = &dfa->states[i];
        printf("\nState %d", i);
        if (i == dfa->start_state_id) printf(" [START]");
        if (state->is_accept) printf(" [ACCEPT]");
        printf(":\n");

        if (state->transitions[0] >= 0) {
            printf("  --a--> State %d\n", state->transitions[0]);
        }
        if (state->transitions[1] >= 0) {
            printf("  --b--> State %d\n", state->transitions[1]);
        }
    }

    printf("\n================================================================\n\n");
}

/* ======================== Main Program ======================== */

int main(void)
{
    /*
     * EXAMPLE USED:
     * NFA for: (a|b)*abb
     * Convert to equivalent DFA using subset construction
     * The DFA will have multiple states corresponding to different NFA state subsets
     */

    printf("\n");
    printf("========================================================================\n");
    printf("         SUBSET CONSTRUCTION - NFA TO DFA CONVERSION\n");
    printf("         COMPILER DESIGN LABORATORY\n");
    printf("========================================================================\n");

    printf("\nRegular Expression: (a|b)*abb\n");
    printf("Converting NFA to equivalent DFA via Subset Construction...\n");

    /* Initialize hardcoded NFA */
    NFASpec nfa;
    nfa_init_hardcoded(&nfa);

    printf("\nInput NFA:\n");
    printf("  States: 0 - %d\n", nfa.state_count - 1);
    printf("  Start: %d\n", nfa.start_state);
    printf("  Accept: %d\n", nfa.accept_state);
    printf("  Transitions: %d\n", nfa.transition_count);

    /* Convert to DFA */
    DFASpec dfa;
    subset_construction(&nfa, &dfa);

    /* Display results */
    display_dfa_table(&dfa);
    display_dfa_transitions_detailed(&dfa);

    printf("Status: NFA to DFA conversion completed successfully.\n");
    printf("Converted %d NFA states to %d DFA states.\n", nfa.state_count, dfa.state_count);

    return 0;
}
