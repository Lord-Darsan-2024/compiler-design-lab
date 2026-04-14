# Lab 5 – FIRST and FOLLOW Sets

## Objective

To implement algorithms for computing FIRST and FOLLOW sets for context-free grammars. These sets are essential components of parsing table construction, particularly for LL(1) deterministic top-down parsing. Students will employ iterative fixed-point computation techniques to compute sets over the grammar's production structure.

## Theoretical Background

FIRST and FOLLOW sets are fundamental concepts in compiler construction (Aho et al., 2006). For a grammar symbol X, FIRST(X) is the set of terminals that can appear as the first symbol in any string derivable from X. Formally:

FIRST(X) = {a | X ⇒* aα for some α ∈ (N ∪ T)*, a ∈ T} ∪ {ε | X ⇒* ε}

Similarly, for a non-terminal A, FOLLOW(A) is the set of terminals that can appear immediately after A in any sentential form derivable from the start symbol:

FOLLOW(A) = {a | S ⇒* αAaβ for some α, β ∈ (N ∪ T)*, a ∈ T} ∪ {$ | S ⇒* αA}

**FIRST Computation Rules:**
1. If X is terminal: FIRST(X) = {X}
2. If X → ε: add ε to FIRST(X)
3. If X → Y₁Y₂...Yₙ: add FIRST(Y₁) - {ε} to FIRST(X); if Y₁ ⇒* ε, add FIRST(Y₂) - {ε}, etc.
4. If all Yᵢ ⇒* ε: add ε to FIRST(X)

**FOLLOW Computation Rules:**
1. Add $ to FOLLOW(S) (start symbol)
2. For production A → αBβ: add FIRST(β) - {ε} to FOLLOW(B)
3. If β ⇒* ε or β is empty: add FOLLOW(A) to FOLLOW(B)

These sets are computed via iterative fixed-point computation (closure operation) until no changes occur.

## Algorithm

**Algorithm for Computing FIRST Sets:**

1. **Initialize**: FIRST(X) ← ∅ for all symbols X
2. **Repeat until no changes**:
   - **For each production** A → X₁X₂...Xₙ:
     - **For i = 1 to n**:
       - FIRST(A) ← FIRST(A) ∪ (FIRST(Xᵢ) - {ε})
       - **If** ε ∉ FIRST(Xᵢ): break
     - **If** ∀j: ε ∈ FIRST(Xⱼ): FIRST(A) ← FIRST(A) ∪ {ε}

**Algorithm for Computing FOLLOW Sets:**

1. **Initialize**: FOLLOW(A) ← ∅ for all non-terminals; FOLLOW(S) ← {$}
2. **Repeat until no changes**:
   - **For each production** A → αBβ where B is non-terminal:
     - FOLLOW(B) ← FOLLOW(B) ∪ (FIRST(β) - {ε})
     - **If** β ⇒* ε: FOLLOW(B) ← FOLLOW(B) ∪ FOLLOW(A)

## Example Used

Grammar:
```
E → E+T | T
T → T*F | F
F → (E) | id
```

- FIRST(E) = {id, (}
- FIRST(T) = {id, (}
- FIRST(F) = {id, (}
- FOLLOW(E) = {), +, $}
- FOLLOW(T) = {*, +, ), $}
- FOLLOW(F) = {*, +, ), $}

## Program Structure

- **`set_init()`**: Create empty terminal set. Time: O(n).
- **`set_add(s, term)`**: Add terminal to set. Time: O(1).
- **`set_contains(s, term)`**: Test membership. Time: O(1).
- **`set_union(s1, s2)`**: Union two sets. Time: O(n).
- **`is_nonterminal(sym)`**: Check if non-terminal. Time: O(1).
- **`can_derive_epsilon(grammar, symbol)`**: Test epsilon derivability. Time: O(p).
- **`compute_first(grammar, symbol, first_sets)`**: Compute FIRST set. Time: O(p·n) per call.
- **`compute_follow(grammar, nonterm, first_sets, follow_sets)`**: Compute FOLLOW set. Time: O(p·n²).
- **`display_terminal_set(set)`**: Output set contents. Time: O(a) where a is alphabet size.

## How to Compile and Run

```bash
gcc -std=c99 -Wall -Wextra -O2 -o first_follow first_follow.c
./first_follow
```

## Sample Output

```
========================================================================
             FIRST AND FOLLOW SETS COMPUTATION
             COMPILER DESIGN LABORATORY
========================================================================

Grammar:
  E -> E+T | T
  T -> T*F | F
  F -> (E) | id

------------------------------------------------------------------------
FIRST SETS:
------------------------------------------------------------------------
FIRST(E) = {id, (}
FIRST(T) = {id, (}
FIRST(F) = {id, (}

------------------------------------------------------------------------
FOLLOW SETS:
------------------------------------------------------------------------
FOLLOW(E) = {), +, $}
FOLLOW(T) = {*, +, ), $}
FOLLOW(F) = {*, +, ), $}

------------------------------------------------------------------------
Analysis:
  FIRST(E), FIRST(T), FIRST(F) all contain 'id' and '('
  FOLLOW(E) includes ')', '+', and '$'
  FOLLOW(T) includes '+', ')', '*', and '$'
  FOLLOW(F) includes '+', '*', ')', and '$'
------------------------------------------------------------------------

Status: FIRST and FOLLOW computation completed successfully.
These sets are used for constructing LL(1) predictive parsing tables.
```

## Complexity Analysis

| Metric | Complexity |
|--------|-----------|
| Time Complexity (FIRST) | O(p² · n) – iterative fixed-point over p productions, n alphabet |
| Time Complexity (FOLLOW) | O(p² · n²) – depends on FIRST sets |
| Space Complexity | O(n + p) – for sets and grammar |

The algorithms employ iterative fixed-point computation, repeating until convergence. The number of iterations is typically small (constant factor) in practice.

## References

- Aho, A.V., Lam, M.S., Sethi, R., & Ullman, J.D. (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.). Pearson Education.
- Hopcroft, J.E., Motwani, R., & Ullman, J.D. (2006). *Introduction to Automata Theory, Languages, and Computation* (3rd ed.). Pearson Education.

---
