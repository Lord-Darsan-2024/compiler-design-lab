# Lab 11 – Intermediate Code: Quadruple, Triple, Indirect Triple

## Objective

To generate three intermediate code representations for the same expression, demonstrating different data structure approaches to intermediate representation.

## Theoretical Background

Intermediate code bridges source and target. Three forms:
- **Quadruple**: (op, arg1, arg2, result) – explicit result
- **Triple**: (op, arg1, arg2) – implicit result at index
- **Indirect Triple**: separate statement table pointing to triples – enables optimization

## Algorithm

Generate same computation in three forms; demonstrate equivalence and optimization opportunities.

## Example: a = b*c + b*c

---
