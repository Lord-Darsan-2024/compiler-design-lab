# Lab 7 – Shift-Reduce Parsing

## Objective

To implement a shift-reduce parser for bottom-up parsing. Students will build a parser that uses stack-based state transitions to recognize grammar productions (handles) and reduce them, demonstrating how bottom-up parsers recognize patterns bottom-to-top on the stack.

## Theoretical Background

Shift-reduce parsing is a bottom-up deterministic parsing method where the parser maintains a stack of symbols and processes input left-to-right (Aho et al., 2006). At each step, the parser decides to either shift (push next input symbol) or reduce (recognize a production on the stack and replace with LHS).

The key insight is handle recognition: a handle is an occurrence of production RHS on the stack such that its replacement with LHS leads toward the start symbol. Formal parsing tables (LR tables) determine shifts/reduces precisely, but simpler operator-precedence parsing uses precedence rules between operators.

Advantages of shift-reduce vs. top-down:
- Can parse left-recursive grammars directly
- Natural for operator precedence
- Matches most programming language grammars exactly

## Algorithm

**Shift-Reduce Parsing:**

1. Stack ← {$}, Input ← {w, $}
2. **Repeat**:
   - **If** top of stack matches production RHS (handle found):
     - Reduce: pop RHS, push LHS
   - **Else if** more input available:
     - Shift: push next input symbol
   - **Else if** stack = {$, S} and input = {$}:
     - Accept
   - **Else**:
     - Error

## Example Used

```
Grammar:
E → E+T | T
T → T*F | F
F → (E) | id

Input: id*id+id
```

Parsing steps show handle recognition and reduction sequences.

## Program Structure

- **`initialize_grammar(grammar)`**: Load production rules. Time: O(1).
- **`find_handle(grammar, stack, stack_top)`**: Detect handles. Time: O(r·n).
- **`shift_reduce_parse(grammar, input)`**: Execute parsing. Time: O(n·p).
- **`display_grammar(grammar)`**: Output rules. Time: O(p·n).

## How to Compile and Run

```bash
gcc -std=c99 -Wall -Wextra -O2 -o shift_reduce_parser shift_reduce_parser.c
./shift_reduce_parser
```

## Sample Output

```
------------------------------------------------------------------------
GRAMMAR PRODUCTIONS
------------------------------------------------------------------------
  0. E -> E+T
  1. E -> T
  2. T -> T*F
  3. T -> F
  4. F -> (E)
  5. F -> id

Input string: id*id+id$

------------------------------------------------------------------------
SHIFT-REDUCE PARSING TRACE
------------------------------------------------------------------------
Stack                    Input                    Action              
------------------------------------------------------------------------
$                        id*id+id                 Shift i             
$ i                      d*id+id                  Shift d             
$ i d                    *id+id                   Reduce F->id        
$ F                      *id+id                   Reduce T->F         
$ T                      *id+id                   Shift *             
$ T *                    id+id                    Shift i             
$ T * i                  d+id                     Shift d             
$ T * i d                +id                      Reduce F->id        
$ T * F                  +id                      Reduce T->T*F       
$ T                      +id                      Reduce E->T         
$ E                      +id                      Shift +             
```

## Complexity Analysis

| Metric | Complexity |
|--------|-----------|
| Time Complexity | O(n · p) – for each input, search for handles |
| Space Complexity | O(n + p) – stack and grammar storage |

Shift-reduce is practical for real parsers when combined with LR table generation, achieving O(n) parsing time with clever state encoding.

## References

- Aho, A.V., Lam, M.S., Sethi, R., & Ullman, J.D. (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.). Pearson Education.

---
