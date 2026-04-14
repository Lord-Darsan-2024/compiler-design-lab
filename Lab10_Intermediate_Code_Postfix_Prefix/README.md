# Lab 10 – Intermediate Code Generation – Postfix and Prefix Notation

## Objective

To generate postfix (Reverse Polish Notation) and prefix notation from infix expressions using expression trees, demonstrating intermediate code forms used in compiler backends.

## Theoretical Background

Intermediate code bridges high-level source and low-level target code. Postfix and prefix notations eliminate precedence ambiguity. Postfix (RPN) is used in stack machines; prefix in functional languages. Tree traversals generate these forms: inorder (infix), postorder (postfix), preorder (prefix).

## Algorithm

1. Build AST from infix expression respecting operator precedence
2. Postfix: postorder traversal (left, right, operator)
3. Prefix: preorder traversal (operator, left, right)

## Example Used

(a+b)*(c-d)/e → Postfix: ab+cd-*e/ | Prefix: /*+ab-cde

## Time/Space: O(n) where n is expression length

---
