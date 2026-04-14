# Lab 6 – LL(1) Predictive Parsing Table

## Objective

To construct an LL(1) predictive parsing table from FIRST and FOLLOW sets and simulate the LL(1) parsing algorithm. Students will implement deterministic top-down parsing by building a parse table that maps (non-terminal, lookahead-terminal) pairs to productions.

## Theoretical Background

The LL(1) parsing method is a top-down deterministic parsing technique suitable for LL(1) grammars (Aho et al., 2006). An LL(1) grammar is one where the parser can determine the correct production to apply for any non-terminal by examining only the next input symbol (lookahead of 1).

For a grammar to be LL(1), no two productions of the same non-terminal can have overlapping FIRST sets: ∀A → α | β: FIRST(α) ∩ FIRST(β) = ∅. Additionally, at most one production per non-terminal can derive epsilon.

The predictive parsing table M[A, a] is constructed as:
1. For each production A → α:
   - For each terminal a ∈ FIRST(α), add A → α to M[A, a]
   - If ε ∈ FIRST(α), for each terminal b ∈ FOLLOW(A), add A → α to M[A, b]

Parsing uses a stack initialized with {$, S} and an input stream with appended $. The algorithm:
1. If stack top = input symbol: pop stack, advance input
2. If stack top = non-terminal: look up M[top, lookahead]; if entry exists, replace with production RHS

## Algorithm

**LL(1) Parsing Table Construction:**

1. **For each production** A → α:
   - **For each terminal** a ∈ FIRST(α):
     - M[A, a] ← A → α
   - **If** ε ∈ FIRST(α):
     - **For each terminal** b ∈ FOLLOW(A):
       - M[A, b] ← A → α

**LL(1) Parsing Algorithm:**

1. Stack ← {$, S}, input ← {w, $}
2. **Repeat**:
   - If stack top = $ and input = $: Accept
   - Else if stack top = input: Pop stack, advance input
   - Else if stack top = non-terminal A:
     - **If** M[A, lookahead] defined: Replace A with RHS
     - **Else**: Error (table entry undefined)

## Example Used

Grammar (left-recursion removed):
```
E  → TE'
E' → +TE' | ε
T  → FT'
T' → *FT' | ε
F  → id | (E)
```

Input: `id+id*id$`

## Program Structure

- **`initialize_parsing_table(table)`**: Create hardcoded parse table. Time: O(1).
- **`ll1_parse(table, input)`**: Execute LL(1) parsing algorithm. Time: O(n + p).
- **`display_parsing_table(table)`**: Output table in matrix form. Time: O(n²).

## How to Compile and Run

```bash
gcc -std=c99 -Wall -Wextra -O2 -o predictive_parsing_table predictive_parsing_table.c
./predictive_parsing_table
```

## Sample Output

```
Grammar (without left recursion):
  E  -> TE'
  E' -> +TE' | ε
  T  -> FT'
  T' -> *FT' | ε
  F  -> id | (E)

============================================================
            LL(1) PREDICTIVE PARSING TABLE
============================================================
          |    id   |    +    |    *    |    (    |    )    |    $    |
----------+---------+---------+---------+---------+---------+---------+
    E    |  Prod0  |   --   |   --   |  Prod0  |   --   |   --   |
----------+---------+---------+---------+---------+---------+---------+
    E'   |   --   |  Prod1  |   --   |   --   |  Prod2  |  Prod2  |
----------+---------+---------+---------+---------+---------+---------+
...

Input string: id+id*id$

------------------------------------------------------------------------
LL(1) PARSING TRACE
------------------------------------------------------------------------
Stack                Input                Action               
------------------------------------------------------------------------
E                    id                   Expand              
T                    id                   Expand              
F                    id                   Expand              
id                   id                   Match               

Status: LL(1) parsing table construction and simulation completed.
```

## Complexity Analysis

| Metric | Complexity |
|--------|-----------|
| Time (Table Construction) | O(p · n) – for each production, process symbols |
| Time (Parsing) | O(n + p) – stack operations and table lookups |
| Space Complexity | O(|N| × |T|) – table size |

Table construction is linear in grammar size. Parsing time is proportional to input length plus production count.

## References

- Aho, A.V., Lam, M.S., Sethi, R., & Ullman, J.D. (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.). Pearson Education.
- Hopcroft, J.E., Motwani, R., & Ullman, J.D. (2006). *Introduction to Automata Theory, Languages, and Computation* (3rd ed.). Pearson Education.

---
