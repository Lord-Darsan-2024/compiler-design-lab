# Lab 8 – LEADING and TRAILING Sets

## Objective

To compute LEADING and TRAILING sets for operator grammars, which are essential for operator-precedence parsing table construction and conflict resolution in shift-reduce parsers.

## Theoretical Background

For operator grammars (grammars without adjacent non-terminals), LEADING and TRAILING sets guide parsing decisions. LEADING(A) contains first terminals derivable from A; TRAILING(A) contains terminals that can follow A in any production.

## Algorithm

1. **LEADING(A)**: For each A → α, if α starts with terminal t, add t; if α starts with non-terminal B, add LEADING(B)
2. **TRAILING(A)**: For each production containing A, examine what follows

## Example Used

```
E → E+T | T
T → T*F | F
F → id
```

## Program Structure

Simple fixed-point computation functions for set construction and display.

## How to Compile and Run

```bash
gcc -std=c99 -Wall -Wextra -O2 -o leading_trailing leading_trailing.c
./leading_trailing
```

## Complexity Analysis

| Metric | Complexity |
|--------|-----------|
| Time | O(p²) amortized |
| Space | O(a · p) |

---
