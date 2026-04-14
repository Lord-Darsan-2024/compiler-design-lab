# Lab 1 – Lexical Analyzer

## Objective

To design and implement a complete lexical analyzer (scanner) that recognizes and classifies lexical tokens from a C-like source code stream. The lexical analyzer acts as the first phase of a compiler, converting character sequences into meaningful tokens (keywords, identifiers, operators, delimiters, literals) for downstream parsing phases.

## Theoretical Background

The lexical analysis phase is the foundational component of a compiler that performs the first pass over source code. According to Aho, Lam, Sethi, and Ullman (2006), the lexical analyzer's primary responsibilities include: recognizing tokens (the minimal syntactic units of a language), managing source code position tracking (line and column numbers), filtering out whitespace and comments, and generating a token stream suitable for syntax analysis.

Formally, a lexical analyzer can be modeled as a deterministic finite automaton (DFA) that reads a sequence of input characters and produces a sequence of tokens. Each token is associated with a token type (category) and a lexeme (the actual character sequence). The analyzer typically operates in a greedy, single-pass manner, adopting the longest match principle: when multiple token patterns could match at the current position, the lexer selects the pattern that matches the longest possible character sequence.

Token classification typically includes: (1) **Keywords**, reserved words with predefined linguistic meaning (e.g., if, while, return); (2) **Identifiers**, user-defined names following language conventions (typically starting with a letter or underscore, followed by alphanumerics); (3) **Literals**, constant values including integers, floating-point numbers, and string constants; (4) **Operators**, symbols denoting computational operations (arithmetic, relational, logical); (5) **Delimiters**, syntactic separators such as semicolons, parentheses, and braces; (6) **Comments**, non-executable text skipped by the lexer.

The implementation strategy employs character classification and state-machine principles. For efficiency, the analyzer maintains a current position pointer, line/column counters, and buffers for character lookahead. Keyword recognition typically uses hash tables or simple sequential searches, while operator recognition must distinguish between single-character (+, -, *) and multi-character operators (==, !=, <=, >=, &&, ||).

## Algorithm

**Lexical Analysis Algorithm (Character-by-Character Scanning):**

1. **Initialize** analyzer state: position ← 0, line_number ← 1
2. **Loop** until end of source:
   - Skip whitespace and comments; update line counters
   - Identify current character type
   - **Case: Digit**
      - Call read_number() for integer/float classification
      - Return numeric token
   - **Case: Letter or Underscore**
      - Call read_identifier_or_keyword()
      - Check keyword table; classify as KEYWORD or IDENTIFIER
      - Return token
   - **Case: Operator Character**
      - Check for two-character operators (==, !=, etc.)
      - Return operator token
   - **Case: Delimiter**
      - Recognize punctuation ({, }, (, ), ;, ,)
      - Return delimiter token
   - **Case: Other**
      - Return ERROR token
3. **Return** TOKEN_EOF when source exhausted
4. **Collect** all tokens in array
5. **Display** results in formatted table

## Example Used

```
// Example C-like code
int main() {
    int count = 10;
    float value = 3.14;
    if (count > 0) {
        sum = count + value;
    }
    while (count != 0) {
        count = count - 1;
    }
    return 0;
}
```

This example contains 28 tokens including:
- Keywords: int (×2), if, while, return, main (identifier, not keyword)
- Identifiers: main, count, value, sum
- Numeric literals: 10 (integer), 3.14 (float), 0 (integer), 1 (integer)
- Operators: =, >, !=, -, +
- Delimiters: (, ), {, }, ;, ,
- Comments: // single-line comment

## Program Structure

- **`is_keyword(lexeme)`**: Checks if lexeme matches reserved keywords. Time: O(k·m), where k = keyword count, m = lexeme length.
- **`is_operator(ch, next_ch)`**: Determines if character sequence forms valid operator. Time: O(1).
- **`is_delimiter(ch)`**: Tests if character is a delimiter. Time: O(1).
- **`lexical_analyzer_init(analyzer, source)`**: Initializes analyzer state. Time: O(1).
- **`skip_whitespace(analyzer)`**: Advances past whitespace; updates line counter. Time: O(w), where w = whitespace sequence length.
- **`skip_comment(analyzer)`**: Skips C++ style comments (//). Time: O(c), where c = comment length.
- **`read_number(analyzer, buffer)`**: Reads integer or float literal. Time: O(n), where n = number length.
- **`read_identifier_or_keyword(analyzer, buffer)`**: Reads identifier; checks keyword table. Time: O(m), where m = lexeme length.
- **`read_operator(analyzer, buffer)`**: Reads single or two-character operator. Time: O(1).
- **`lexical_analyzer_next_token(analyzer)`**: Extracts next token. Time: O(m), where m = token length.
- **`tokenize(analyzer)`**: Performs complete lexical analysis. Time: O(n), where n = source length.
- **`display_token_table(analyzer)`**: Outputs formatted token table. Time: O(t), where t = token count.

## How to Compile and Run

```bash
gcc -std=c99 -Wall -Wextra -O2 -o lexical_analyzer lexical_analyzer.c
./lexical_analyzer
```

## Sample Output

```
Source Code:
----------------------------------------------------------------------
// Example C-like code
int main() {
    int count = 10;
    float value = 3.14;
    ...
----------------------------------------------------------------------

==============================================================
                    LEXICAL ANALYSIS RESULTS
==============================================================
Token Type           Lexeme               Line Number    
--------------================================================
KEYWORD              int                  2              
IDENTIFIER           main                 2              
DELIMITER            (                    2              
DELIMITER            )                    2              
DELIMITER            {                    3              
KEYWORD              int                  3              
IDENTIFIER           count                3              
OPERATOR             =                    3              
NUMBER_INT           10                   3              
DELIMITER            ;                    3              
KEYWORD              float                4              
IDENTIFIER           value                4              
OPERATOR             =                    4              
NUMBER_FLOAT         3.14                 4              
...
--------------------------------------------------------------
Total Tokens: 28
==============================================================
```

## Complexity Analysis

| Metric | Complexity |
|--------|-----------|
| Time Complexity | O(n) – Single pass over source code of length n |
| Space Complexity | O(m) – Storage for token array of m tokens; O(k) for keyword table |

The lexical analyzer operates in linear time proportional to source code length because each character is examined exactly once. Space complexity is O(m) for storing tokens plus O(k) for the keyword table (k is typically small and constant).

## References

- Aho, A.V., Lam, M.S., Sethi, R., & Ullman, J.D. (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.). Pearson Education.
- Hopcroft, J.E., Motwani, R., & Ullman, J.D. (2006). *Introduction to Automata Theory, Languages, and Computation* (3rd ed.). Pearson Education.
- Appel, A.W. (2002). *Modern Compiler Implementation in C*. Cambridge University Press.

---
