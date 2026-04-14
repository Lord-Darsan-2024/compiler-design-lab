/*
 * ============================================================================
 * Lab 2: Regular Expression to NFA (Thompson's Construction)
 * ============================================================================
 *
 * DESCRIPTION:
 * This laboratory implements Thompson's Construction algorithm to convert
 * a regular expression into a Non-deterministic Finite Automaton (NFA).
 * Thompson's construction handles operators: concatenation (.), union (|),
 * Kleene star (*), and plus (+). The NFA is represented as an adjacency
 * list with explicit epsilon transitions. This is a fundamental technique
 * in lexical analysis, pattern matching, and regular expression engines.
 * The constructed NFA can be subsequently converted to a DFA via the
 * subset construction method.
 *
 * INPUT:
 * A hardcoded regular expression: (a|b)*abb
 * This matches strings over {a,b} ending with "abb"
 *
 * OUTPUT:
 * NFA transition table with state indices, symbols, and epsilon transitions
 *
 * COMPILATION AND EXECUTION:
 * gcc -std=c99 -Wall -Wextra -O2 -o re_to_nfa re_to_nfa.c
 * ./re_to_nfa
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* ======================== Type Definitions ======================== */

#define MAX_STATES       200
#define MAX_REGEX_LENGTH 256
#define NO_STATE         -1
#define EPSILON          'ε'

/* NFA transition edge */
typedef struct {
    int from_state;
    char symbol;          /* 'a', 'b', ... or EPSILON */
    int to_state;
} Transition;

/* NFA structure */
typedef struct {
    int start_state;
    int accept_state;
} NFA;

/* NFA Fragment for Thompson's Construction */
typedef struct {
    int start;
    int accept;
} Fragment;

/* Global NFA state and transition storage */
typedef struct {
    int state_count;
    Transition transitions[MAX_STATES * 4];
    int transition_count;
    int accept_states[MAX_STATES];
    int accept_count;
} NFATable;

NFATable nfa_table;

/*
 * FUNCTION: nfa_table_init
 * PURPOSE: Initialize NFA table structure
 * PARAMETERS: void
 * RETURN: void
 * COMPLEXITY: O(1)
 */
void nfa_table_init(void)
{
    nfa_table.state_count = 0;
    nfa_table.transition_count = 0;
    nfa_table.accept_count = 0;
}

/*
 * FUNCTION: allocate_state
 * PURPOSE: Allocate a new unique state number
 * PARAMETERS: void
 * RETURN: new state identifier
 * COMPLEXITY: O(1)
 */
int allocate_state(void)
{
    return nfa_table.state_count++;
}

/*
 * FUNCTION: add_transition
 * PURPOSE: Add a transition edge to NFA
 * PARAMETERS: from - source state, symbol - edge label, to - destination state
 * RETURN: void
 * COMPLEXITY: O(1)
 */
void add_transition(int from, char symbol, int to)
{
    nfa_table.transitions[nfa_table.transition_count].from_state = from;
    nfa_table.transitions[nfa_table.transition_count].symbol = symbol;
    nfa_table.transitions[nfa_table.transition_count].to_state = to;
    nfa_table.transition_count++;
}

/*
 * FUNCTION: thompson_symbol
 * PURPOSE: Create NFA fragment for single symbol
 * PARAMETERS: symbol - input character (e.g., 'a', 'b')
 * RETURN: NFA fragment with start and accept states
 * COMPLEXITY: O(1)
 */
Fragment thompson_symbol(char symbol)
{
    Fragment frag;
    frag.start = allocate_state();
    frag.accept = allocate_state();
    add_transition(frag.start, symbol, frag.accept);
    return frag;
}

/*
 * FUNCTION: thompson_concatenation
 * PURPOSE: Concatenate two NFA fragments: A . B
 * PARAMETERS: first - first fragment, second - second fragment
 * RETURN: Combined NFA fragment
 * COMPLEXITY: O(1)
 */
Fragment thompson_concatenation(Fragment first, Fragment second)
{
    Fragment frag;
    /* Connect accept state of first to start state of second via epsilon */
    add_transition(first.accept, EPSILON, second.start);
    frag.start = first.start;
    frag.accept = second.accept;
    return frag;
}

/*
 * FUNCTION: thompson_union
 * PURPOSE: Union of two NFA fragments: A | B
 * PARAMETERS: first - first fragment, second - second fragment
 * RETURN: Combined NFA fragment with epsilon transitions
 * COMPLEXITY: O(1)
 */
Fragment thompson_union(Fragment first, Fragment second)
{
    Fragment frag;
    frag.start = allocate_state();
    frag.accept = allocate_state();

    /* Epsilon transitions from new start to both fragments */
    add_transition(frag.start, EPSILON, first.start);
    add_transition(frag.start, EPSILON, second.start);

    /* Epsilon transitions from both fragments to new accept */
    add_transition(first.accept, EPSILON, frag.accept);
    add_transition(second.accept, EPSILON, frag.accept);

    return frag;
}

/*
 * FUNCTION: thompson_kleene_star
 * PURPOSE: Kleene star operation: A*
 * PARAMETERS: fragment - input NFA fragment
 * RETURN: New NFA fragment with Kleene star applied
 * COMPLEXITY: O(1)
 */
Fragment thompson_kleene_star(Fragment fragment)
{
    Fragment frag;
    frag.start = allocate_state();
    frag.accept = allocate_state();

    /* Epsilon from new start to fragment start */
    add_transition(frag.start, EPSILON, fragment.start);

    /* Epsilon from fragment accept back to fragment start (loop) */
    add_transition(fragment.accept, EPSILON, fragment.start);

    /* Epsilon from fragment accept to new accept */
    add_transition(fragment.accept, EPSILON, frag.accept);

    /* Epsilon from new start to new accept (zero repetitions) */
    add_transition(frag.start, EPSILON, frag.accept);

    return frag;
}

/*
 * FUNCTION: thompson_plus
 * PURPOSE: Plus operation: A+ (one or more)
 * PARAMETERS: fragment - input NFA fragment
 * RETURN: New NFA fragment with plus applied
 * COMPLEXITY: O(1)
 */
Fragment thompson_plus(Fragment fragment)
{
    Fragment frag;
    frag.start = allocate_state();
    frag.accept = allocate_state();

    /* Epsilon from new start to fragment start */
    add_transition(frag.start, EPSILON, fragment.start);

    /* Epsilon from fragment accept back to fragment start (loop) */
    add_transition(fragment.accept, EPSILON, fragment.start);

    /* Epsilon from fragment accept to new accept */
    add_transition(fragment.accept, EPSILON, frag.accept);

    return frag;
}

/*
 * FUNCTION: find_closing_paren
 * PURPOSE: Find matching closing parenthesis in regex
 * PARAMETERS: regex - regex string, start - opening paren position
 * RETURN: position of matching closing paren
 * COMPLEXITY: O(n) where n is regex length
 */
int find_closing_paren(const char *regex, int start)
{
    int count = 1;
    int i = start + 1;
    while (regex[i] != '\0' && count > 0) {
        if (regex[i] == '(') count++;
        if (regex[i] == ')') count--;
        if (count > 0) i++;
    }
    return i;
}

/*
 * FUNCTION: parse_regex
 * PURPOSE: Recursively parse regex and build NFA via Thompson's construction
 * PARAMETERS: regex - regex string, pos - current position marker (input/output)
 * RETURN: NFA fragment
 * COMPLEXITY: O(n) where n is regex length
 */
Fragment parse_regex(const char *regex, int *pos);

Fragment parse_term(const char *regex, int *pos)
{
    char ch = regex[*pos];

    if (ch == '(') {
        /* Handle parenthesized subexpression */
        (*pos)++;
        Fragment frag = parse_regex(regex, pos);
        (*pos)++; /* skip ')' */
        return frag;
    } else if (ch == 'a' || ch == 'b') {
        /* Handle symbol */
        (*pos)++;
        return thompson_symbol(ch);
    }
    /* Should not reach here given valid regex */
    return thompson_symbol('a');
}

Fragment parse_factor(const char *regex, int *pos)
{
    Fragment frag = parse_term(regex, pos);

    while (regex[*pos] == '*' || regex[*pos] == '+') {
        if (regex[*pos] == '*') {
            frag = thompson_kleene_star(frag);
            (*pos)++;
        } else if (regex[*pos] == '+') {
            frag = thompson_plus(frag);
            (*pos)++;
        }
    }
    return frag;
}

Fragment parse_regex(const char *regex, int *pos)
{
    Fragment frag = parse_factor(regex, pos);

    while (regex[*pos] != '\0' && regex[*pos] != ')' && regex[*pos] != '|') {
        frag = thompson_concatenation(frag, parse_factor(regex, pos));
    }

    /* Handle union (|) */
    if (regex[*pos] == '|') {
        (*pos)++;
        Fragment right = parse_regex(regex, pos);
        frag = thompson_union(frag, right);
    }

    return frag;
}

/*
 * FUNCTION: build_nfa_from_regex
 * PURPOSE: Main entry point for RE to NFA conversion
 * PARAMETERS: regex - regular expression string
 * RETURN: NFA structure with start and accept states
 * COMPLEXITY: O(n) where n is regex length
 */
NFA build_nfa_from_regex(const char *regex)
{
    nfa_table_init();
    int pos = 0;
    Fragment frag = parse_regex(regex, &pos);
    NFA nfa;
    nfa.start_state = frag.start;
    nfa.accept_state = frag.accept;
    return nfa;
}

/*
 * FUNCTION: display_nfa_transitions
 * PURPOSE: Display NFA transition table in formatted output
 * PARAMETERS: nfa - NFA structure
 * RETURN: void
 * COMPLEXITY: O(t) where t is transition count
 */
void display_nfa_transitions(NFA nfa)
{
    printf("\n");
    printf("================================================================\n");
    printf("            NFA TRANSITION TABLE\n");
    printf("================================================================\n");
    printf("%-15s %-15s %-15s\n", "From State", "Symbol", "To State");
    printf("----------------------------------------------------------------\n");

    for (int i = 0; i < nfa_table.transition_count; i++) {
        Transition *trans = &nfa_table.transitions[i];
        char symbol_str[10];
        if (trans->symbol == EPSILON) {
            strcpy(symbol_str, "ε");
        } else {
            sprintf(symbol_str, "%c", trans->symbol);
        }

        printf("%-15d %-15s %-15d", trans->from_state, symbol_str, trans->to_state);
        if (trans->to_state == nfa.accept_state) {
            printf(" [ACCEPT]");
        }
        printf("\n");
    }

    printf("----------------------------------------------------------------\n");
    printf("Start State: %d\n", nfa.start_state);
    printf("Accept State: %d\n", nfa.accept_state);
    printf("Total States: %d\n", nfa_table.state_count);
    printf("Total Transitions: %d\n", nfa_table.transition_count);
    printf("================================================================\n\n");
}

/*
 * FUNCTION: display_nfa_diagram
 * PURPOSE: Display a text-based NFA state diagram
 * PARAMETERS: nfa - NFA structure
 * RETURN: void
 * COMPLEXITY: O(s + t) where s is state count, t is transition count
 */
void display_nfa_diagram(NFA nfa)
{
    printf("\n");
    printf("================================================================\n");
    printf("            NFA STATE TRANSITIONS BY STATE\n");
    printf("================================================================\n");

    for (int state = 0; state < nfa_table.state_count; state++) {
        printf("\nState %d", state);
        if (state == nfa.start_state) printf(" [START]");
        if (state == nfa.accept_state) printf(" [ACCEPT]");
        printf(":\n");

        for (int i = 0; i < nfa_table.transition_count; i++) {
            if (nfa_table.transitions[i].from_state == state) {
                Transition *trans = &nfa_table.transitions[i];
                if (trans->symbol == EPSILON) {
                    printf("  --ε--> State %d\n", trans->to_state);
                } else {
                    printf("  --%c--> State %d\n", trans->symbol, trans->to_state);
                }
            }
        }
    }

    printf("\n================================================================\n\n");
}

/* ======================== Main Program ======================== */

int main(void)
{
    /*
     * EXAMPLE USED:
     * Regular Expression: (a|b)*abb
     * Meaning: Any number of 'a' or 'b' symbols, followed by exactly 'abb'
     * Matches: "abb", "aabb", "babb", "aaabb", "ababb", "aababb", ...
     * Does not match: "ab", "aab", "ba", "bba"
     */

    const char *regex = "(a|b)*abb";

    printf("\n");
    printf("========================================================================\n");
    printf("    THOMPSON'S CONSTRUCTION - REGULAR EXPRESSION TO NFA\n");
    printf("    COMPILER DESIGN LABORATORY\n");
    printf("========================================================================\n");

    printf("\nRegular Expression: %s\n", regex);
    printf("Meaning: Any number of 'a' or 'b', followed by the sequence 'abb'\n");

    /* Build NFA from regex */
    NFA nfa = build_nfa_from_regex(regex);

    /* Display results */
    display_nfa_transitions(nfa);
    display_nfa_diagram(nfa);

    printf("Status: NFA construction completed successfully.\n");
    printf("The constructed NFA has %d states and %d transitions.\n",
           nfa_table.state_count, nfa_table.transition_count);
    printf("This NFA can be converted to a DFA using subset construction.\n");

    return 0;
}
