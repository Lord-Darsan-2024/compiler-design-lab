# Lab 4 – Grammar Transformations

## Objective

To implement three fundamental grammar transformation techniques: elimination of ambiguity, removal of left recursion (direct and indirect), and left factoring. Students will apply these transformations systematically to convert context-free grammars into forms suitable for deterministic parsing algorithms, particularly LL(1) predictive parsing.

## Theoretical Background

Context-free grammars (CFGs) can exhibit properties that prevent the construction of efficient, deterministic parsers. According to Aho, Lam, Sethi, and Ullman (2006), three major transformation techniques address these issues:

**1. Ambiguity Elimination**: A grammar is ambiguous if a single string can have multiple distinct parse trees. The classic "dangling-else" problem exemplifies ambiguity:
```
S → if C then S else S | if C then S | other
```
This grammar is ambiguous because "if c1 then if c2 then s1 else s2" can associate the else with either if. Elimination requires careful grammar rewriting to enforce a specific associativity convention (typically matching else with the nearest unmatched then).

**2. Left Recursion Removal**: A grammar is left-recursive if a non-terminal A can derive a sentential form beginning with A. Such recursion prevents top-down parsers (like recursive descent or LL parsers) from functioning correctly, as the parser would infinitely loop attempting to expand A.

Direct left recursion has form: A → Aα₁ | Aα₂ | ... | Aαₘ | β₁ | β₂ | ... | βₙ

Removal transforms to: 
- A → β₁A' | β₂A' | ... | βₙA'
- A' → α₁A' | α₂A' | ... | αₘA' | ε

Indirect left recursion (cycles in dependency graph) requires elimination of all direct recursion plus iterative elimination of cycles.

**3. Left Factoring**: A grammar lacks the FIRST set property if two productions for the same non-terminal have overlapping FIRST sets. Left factoring resolves this by extracting common prefixes:
```
A → αβ | αγ | δ  →  A → αA' | δ    where   A' → β | γ
```

These transformations enable the construction of LL(1) parsing tables without shift-reduce conflicts.

## Algorithm

**Algorithm 1: Eliminate Direct Left Recursion**

For each non-terminal A with left-recursive productions:
1. Collect productions: A → Aα₁ | ... | Aαₘ | β₁ | ... | βₙ
2. Create new non-terminal A'
3. Replace with:
   - A → β₁A' | β₂A' | ... | βₙA'
   - A' → α₁A' | α₂A' | ... | αₘA' | ε

**Algorithm 2: Left Factor**

1. **For each non-terminal A**:
   - **While** there exist two or more A-productions sharing longest common prefix α:
     - A → αβ | αγ | ...
     - Create new non-terminal A'
     - Replace with: A → αA', A' → β | γ | ...

**Algorithm 3: Eliminate Ambiguity**

- Rewrite ambiguous productions using additional non-terminals or precedence rules
- For if-else: Transform to matched/unmatched distinction
- Enforce operator associativity through grammar structure

## Example Used

Original ambiguous grammar for if-else statements:
```
S → iCtS | iCtSeS | a
C → b
```

Where: i = if, C = condition, t = then, S = statement, e = else, a = other_statement

The ambiguity arises in parsing: `if b then if b then a else a`

## Program Structure

- **`print_production(prod)`**: Display single rule. Time: O(n).
- **`print_grammar(grammar)`**: Display all rules. Time: O(p·n).
- **`create_grammar()`**: Initialize empty grammar. Time: O(1).
- **`add_production(grammar, lhs, rhs)`**: Insert production. Time: O(1).
- **`remove_direct_left_recursion(grammar)`**: Eliminate left recursion. Time: O(p²).
- **`left_factor(grammar)`**: Apply left factoring. Time: O(p²·n).
- **`eliminate_ambiguity(grammar)`**: Remove ambiguity. Time: O(p).

## How to Compile and Run

```bash
gcc -std=c99 -Wall -Wextra -O2 -o grammar_transform grammar_transform.c
./grammar_transform
```

## Sample Output

```
========================================================================
         GRAMMAR TRANSFORMATIONS
         COMPILER DESIGN LABORATORY
========================================================================

1. ORIGINAL GRAMMAR (Ambiguous if-else):
   Input: S -> iCtS | iCtSeS | a, C -> b

Productions:
  S -> iCtS
  S -> iCtSeS
  S -> a
  C -> b

------------------------------------------------------------------------
   Issue: The production 'S -> iCtSeS' introduces ambiguity.
   When parsing nested conditionals, the else can match either the
   inner or outer if (dangling-else problem).
------------------------------------------------------------------------

2. APPLYING LEFT FACTORING:

After Left Factoring:
  S -> iCtS'
  S' -> S
  S' -> eS
  S -> a
  C -> b

Effect: Extracts common prefixes to eliminate non-determinism.

3. ELIMINATING DIRECT LEFT RECURSION:
   Example: E -> E + T | T
   Transformed to:
   E -> T E'
   E' -> + T E' | ε

Before:
  E -> E+T
  E -> T

After:
  E -> TE'
  E' -> +TE'
  E' -> ε

4. SUMMARY OF TRANSFORMATIONS:
   ✓ Ambiguity Elimination: Removes ambiguous productions
   ✓ Left Recursion Removal: Converts A -> Aα | β to A -> βA', A' -> αA' | ε
   ✓ Left Factoring: Converts A -> αβ | αγ to A -> αA', A' -> β | γ
```

## Complexity Analysis

| Metric | Complexity |
|--------|-----------|
| Time (Left Recursion Removal) | O(p²) – where p is production count |
| Time (Left Factoring) | O(p²·n) – where n is average RHS length |
| Time (Ambiguity Elimination) | O(p·n) – linear in grammar size |
| Space Complexity | O(p·n) – storage for transformed grammar |

The algorithms are polynomial in grammar size, making them practical for real-world parsing projects.

## References

- Aho, A.V., Lam, M.S., Sethi, R., & Ullman, J.D. (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.). Pearson Education.
- Hopcroft, J.E., Motwani, R., & Ullman, J.D. (2006). *Introduction to Automata Theory, Languages, and Computation* (3rd ed.). Pearson Education.
- Knuth, D.E. (1965). On the Translation of Languages from Left to Right. *Information and Control*, 8(6), 607-639.

---
