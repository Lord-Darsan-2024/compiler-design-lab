# Lab 9 – LR(0) Items and Canonical Collection

## Objective

To construct the canonical collection of LR(0) items, which represents parser states in LR parser construction.

## Theoretical Background

An LR(0) item is a production with a dot indicating how much of the RHS has been processed. The canonical collection is generated via closure and GOTO operations, forming the basis for LR parser automaton construction.

## Algorithm

1. Start with augmented start production with dot at beginning
2. Compute closure: add items for all productions of non-terminals after dot
3. Compute GOTO for each symbol: shift dot over symbol, recompute closure
4. Repeat until no new states generated

## Example Used

Augmented grammar: E' → E | E → E+T | T | T → T*F | F | F → (E) | id

## Complexity Analysis

| Metric | Complexity |
|--------|-----------|
| Time | O(n² · m) |
| Space | O(n · m) |

---
