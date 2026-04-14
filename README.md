# Compiler Design Laboratory – Complete Reference Implementation

## Overview

This repository contains a comprehensive, publication-quality implementation of a postgraduate (M.Tech / M.S.) Compiler Design Laboratory course suitable for premier research institutions including IIT, NIT, and equivalent establishments. The laboratory consists of 15 interconnected experiments systematically covering the complete compiler pipeline from lexical analysis through code generation and optimization.

## Course Abstract

The Compiler Design Laboratory provides hands-on experience with the theoretical principles and practical implementation techniques fundamental to compiler construction. Through systematic examination of each phase—lexical analysis, syntax analysis, intermediate code generation, and code optimization—students develop production-quality implementations demonstrating how high-level programming constructs are translated to efficient machine code. This laboratory bridges formal language theory and real-world compiler engineering, equipping students with deep understanding of language implementation and practical software engineering skills applicable to domain-specific language design, program analysis, and runtime systems.

## Laboratory Structure

| Lab # | Title | Key Concepts | C Source |
|-------|-------|--------------|----------|
| 1 | Lexical Analyzer | Tokenization, keyword recognition, finite state machines | [lexical_analyzer.c](Lab01_Lexical_Analyzer/lexical_analyzer.c) |
| 2 | RE to NFA | Thompson's construction, regular expressions, epsilon transitions | [re_to_nfa.c](Lab02_RE_to_NFA/re_to_nfa.c) |
| 3 | NFA to DFA | Subset construction, determinization, state explosion | [nfa_to_dfa.c](Lab03_NFA_to_DFA/nfa_to_dfa.c) |
| 4 | Grammar Transformations | Ambiguity removal, left recursion elimination, left factoring | [grammar_transform.c](Lab04_Ambiguity_LeftRecursion_LeftFactoring/grammar_transform.c) |
| 5 | FIRST/FOLLOW Sets | Set computation, fixed-point algorithms, LL(1) conditions | [first_follow.c](Lab05_FIRST_FOLLOW/first_follow.c) |
| 6 | Predictive Parsing Table | LL(1) table construction, top-down parsing | [predictive_parsing_table.c](Lab06_Predictive_Parsing_Table/predictive_parsing_table.c) |
| 7 | Shift-Reduce Parsing | Handle recognition, stack-based parsing | [shift_reduce_parser.c](Lab07_Shift_Reduce_Parsing/shift_reduce_parser.c) |
| 8 | LEADING/TRAILING Sets | Operator-precedence parsing, set theory | [leading_trailing.c](Lab08_LEADING_TRAILING/leading_trailing.c) |
| 9 | LR(0) Items | Canonical collection, LR parser states | [lr0_items.c](Lab09_LR0_Items/lr0_items.c) |
| 10 | Postfix/Prefix Code | Expression trees, traversals, intermediate notation | [postfix_prefix.c](Lab10_Intermediate_Code_Postfix_Prefix/postfix_prefix.c) |
| 11 | Quadruple/Triple Code | Intermediate representations, code forms | [quad_triple_indirect.c](Lab11_Intermediate_Code_Quadruple_Triple/quad_triple_indirect.c) |
| 12 | Code Generator | Register allocation, address descriptors | [code_generator.c](Lab12_Code_Generator/code_generator.c) |
| 13 | DAG Optimization | Common subexpression elimination, basic block | [dag.c](Lab13_DAG/dag.c) |
| 14 | Data Flow Analysis | Reaching definitions, iterative algorithms | [data_flow_analysis.c](Lab14_Global_Data_Flow_Analysis/data_flow_analysis.c) |
| 15 | Storage Allocation | Heap management, first-fit strategy, coalescing | [storage_allocation.c](Lab15_Storage_Allocation/storage_allocation.c) |

## Prerequisites

### Academic Foundation
- **Formal Language Theory**: Context-free grammars, regular languages, automata (Hopcroft et al., 2006)
- **Discrete Mathematics**: Set theory, graph algorithms, mathematical induction
- **Programming**: Proficiency in C (data structures, pointers, dynamic memory)

### Recommended Textbooks
- *Compilers: Principles, Techniques, and Tools* (Dragon Book) – Aho, Lam, Sethi, Ullman (2006)
- *Introduction to Automata Theory, Languages, and Computation* – Hopcroft, Motwani, Ullman (2006)
- *Modern Compiler Implementation in C* – Appel (2002)

## Repository Structure

```
compiler-design-lab/
├── README.md (this file)
├── Lab01_Lexical_Analyzer/
│   ├── lexical_analyzer.c
│   └── README.md
├── Lab02_RE_to_NFA/
│   ├── re_to_nfa.c
│   └── README.md
├── Lab03_NFA_to_DFA/
│   ├── nfa_to_dfa.c
│   └── README.md
├── Lab04_Ambiguity_LeftRecursion_LeftFactoring/
│   ├── grammar_transform.c
│   └── README.md
├── Lab05_FIRST_FOLLOW/
│   ├── first_follow.c
│   └── README.md
├── Lab06_Predictive_Parsing_Table/
│   ├── predictive_parsing_table.c
│   └── README.md
├── Lab07_Shift_Reduce_Parsing/
│   ├── shift_reduce_parser.c
│   └── README.md
├── Lab08_LEADING_TRAILING/
│   ├── leading_trailing.c
│   └── README.md
├── Lab09_LR0_Items/
│   ├── lr0_items.c
│   └── README.md
├── Lab10_Intermediate_Code_Postfix_Prefix/
│   ├── postfix_prefix.c
│   └── README.md
├── Lab11_Intermediate_Code_Quadruple_Triple/
│   ├── quad_triple_indirect.c
│   └── README.md
├── Lab12_Code_Generator/
│   ├── code_generator.c
│   └── README.md
├── Lab13_DAG/
│   ├── dag.c
│   └── README.md
├── Lab14_Global_Data_Flow_Analysis/
│   ├── data_flow_analysis.c
│   └── README.md
└── Lab15_Storage_Allocation/
    ├── storage_allocation.c
    └── README.md
```

## Building and Running

### Build Instructions

All programs compile cleanly with ANSI C99 standards without warnings or errors:

```bash
# Compile individual lab
cd Lab01_Lexical_Analyzer
gcc -std=c99 -Wall -Wextra -O2 -o lexical_analyzer lexical_analyzer.c

# Run program
./lexical_analyzer
```

### Batch Compilation Script

```bash
#!/bin/bash
# compile_all.sh

for lab in Lab*/; do
    cd "$lab"
    c_file=$(ls *.c | head -1)
    exec_name=$(basename "$c_file" .c)
    gcc -std=c99 -Wall -Wextra -O2 -o "$exec_name" "$c_file"
    echo "Compiled: $lab/$exec_name"
    cd ..
done
```

## Code Quality Standards

Each laboratory adheres to professional compiler engineering practices:

- **Language**: Pure ANSI C (C99 standard)
- **Compilation**: Zero warnings with `-Wall -Wextra -O2`
- **Structure**: Modular functions with single responsibility
- **Documentation**: Header comments, complexity analysis, algorithm descriptions
- **Memory Management**: Dynamic allocation with explicit deallocation
- **Data Structures**: Typedef'd structs, named constants, no global mutable state
- **Examples**: Non-trivial, academically meaningful inputs (not toy examples)
- **Output**: Well-formatted, publication-quality results with explanatory text

## Academic Integrity

This laboratory is provided for educational purposes at recognized research institutes. Usage policies:

1. **Attribution**: When using or adapting code, credit the original laboratory
2. **Modification**: Modifications and enhancements are encouraged for institutional variants
3. **Assessment**: Student solutions must be original; reference implementations cannot be directly submitted
4. **Research**: Code may be referenced in academic papers with proper citation
5. **Commercial**: Institutional or commercial use requires explicit permission

## Extending and Customizing

This reference implementation provides a solid foundation for institutional customization:

- **Grading Rubrics**: Adapt complexity metrics and testing requirements
- **Project Variants**: Create language-specific compilers (e.g., subset of Python, Java)
- **Advanced Topics**: Add labs on SSA form, vectorization, JIT compilation
- **Tool Integration**: Integrate with LLVM, GCC backends
- **Visualization**: Create tools to visualize parse trees, control flow graphs, DAGs

## References

### Primary Texts
- Aho, A.V., Lam, M.S., Sethi, R., & Ullman, J.D. (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.). Pearson Education.
- Hopcroft, J.E., Motwani, R., & Ullman, J.D. (2006). *Introduction to Automata Theory, Languages, and Computation* (3rd ed.). Pearson Education.
- Appel, A.W. (2002). *Modern Compiler Implementation in C*. Cambridge University Press.

### Additional Resources
- Thompson, K. (1968). Programming Techniques: Regular Expression Search Algorithm. *Communications of the ACM*, 11(6), 419-422.
- Knuth, D.E. (1965). On the Translation of Languages from Left to Right. *Information and Control*, 8(6), 607-639.
- Rabin, M.O., & Scott, D. (1959). Finite Automata and Their Decision Problems. *IBM Journal of Research and Development*, 3(2), 115-125.

## Contact and Support

For institutional adoptions, course adaptations, or research applications, contact the course coordinators at your institution.

---

**Laboratory Created**: April 2026  
**Version**: 1.0  
**Compatibility**: GCC 9.0+, Clang 10.0+, MSVC 2019+  
**License**: Educational Use – See Institution Policies

---
