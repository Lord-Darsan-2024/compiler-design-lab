/*
 * ============================================================================
 * Lab 1: Lexical Analyzer (Lexical Scanner)
 * ============================================================================
 *
 * DESCRIPTION:
 * This laboratory implements a complete lexical analyzer (scanner/tokenizer)
 * for a C-like programming language. The lexical analyzer recognizes and
 * classifies lexical tokens: keywords, identifiers, numeric literals, operators,
 * delimiters, and comments. This is the first phase of a compiler that converts
 * a stream of characters into a stream of meaningful tokens for downstream
 * parsing phases. The implementation uses finite state machine principles and
 * character classification tables for efficient token recognition.
 *
 * INPUT:
 * A hardcoded C-like source code string containing at least 15 tokens including
 * keywords, identifiers, literals, operators, delimiters, and comments.
 *
 * OUTPUT:
 * A formatted table displaying: Token Type | Lexeme | Line Number
 *
 * COMPILATION AND EXECUTION:
 * gcc -std=c99 -Wall -Wextra -O2 -o lexical_analyzer lexical_analyzer.c
 * ./lexical_analyzer
 *
 * AUTHOR: PG Scholar, Department of Computer Science and Engineering
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* ======================== Type Definitions ======================== */

#define MAX_TOKEN_LENGTH 256
#define MAX_TOKENS       500
#define MAX_KEYWORDS     6

/* Token type enumeration */
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER_INT,
    TOKEN_NUMBER_FLOAT,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

/* Token structure */
typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
    int line_number;
} Token;

/* Lexical analyzer state */
typedef struct {
    const char *source;
    int position;
    int line_number;
    Token tokens[MAX_TOKENS];
    int token_count;
} LexicalAnalyzer;

/* ======================== Keyword Table ======================== */

const char *keywords[] = {"if", "else", "while", "for", "int", "float", "return"};
const int keyword_count = 7;

/*
 * FUNCTION: is_keyword
 * PURPOSE: Determine if a given lexeme is a C keyword
 * PARAMETERS: lexeme - the string to check
 * RETURN: true if lexeme is a keyword, false otherwise
 * COMPLEXITY: O(k*m) where k is number of keywords, m is average lexeme length
 */
bool is_keyword(const char *lexeme)
{
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(lexeme, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

/*
 * FUNCTION: is_operator
 * PURPOSE: Determine if a character sequence is a valid operator
 * PARAMETERS: ch - current character, next_ch - lookahead character
 * RETURN: true if forms a valid operator, false otherwise
 * COMPLEXITY: O(1)
 */
bool is_operator(char ch, char next_ch)
{
    /* Single character operators */
    if (strchr("+-*/%=!<>", ch)) {
        return true;
    }
    /* Two-character operators */
    if ((ch == '&' && next_ch == '&') ||
        (ch == '|' && next_ch == '|') ||
        (ch == '=' && next_ch == '=') ||
        (ch == '!' && next_ch == '=') ||
        (ch == '<' && next_ch == '=') ||
        (ch == '>' && next_ch == '=')) {
        return true;
    }
    return false;
}

/*
 * FUNCTION: is_delimiter
 * PURPOSE: Check if character is a delimiter
 * PARAMETERS: ch - character to check
 * RETURN: true if character is a delimiter, false otherwise
 * COMPLEXITY: O(1)
 */
bool is_delimiter(char ch)
{
    return strchr(";,{}()", ch) != NULL;
}

/*
 * FUNCTION: lexical_analyzer_init
 * PURPOSE: Initialize the lexical analyzer with source code
 * PARAMETERS: analyzer - pointer to analyzer structure, source - source code string
 * RETURN: void
 * COMPLEXITY: O(1)
 */
void lexical_analyzer_init(LexicalAnalyzer *analyzer, const char *source)
{
    analyzer->source = source;
    analyzer->position = 0;
    analyzer->line_number = 1;
    analyzer->token_count = 0;
}

/*
 * FUNCTION: skip_whitespace
 * PURPOSE: Skip whitespace characters and update line number on newlines
 * PARAMETERS: analyzer - pointer to analyzer
 * RETURN: void
 * COMPLEXITY: O(n) where n is length of whitespace sequence
 */
void skip_whitespace(LexicalAnalyzer *analyzer)
{
    while (analyzer->position < strlen(analyzer->source)) {
        char ch = analyzer->source[analyzer->position];
        if (ch == '\n') {
            analyzer->line_number++;
            analyzer->position++;
        } else if (isspace(ch)) {
            analyzer->position++;
        } else {
            break;
        }
    }
}

/*
 * FUNCTION: skip_comment
 * PURPOSE: Skip single-line C++ style comments (//)
 * PARAMETERS: analyzer - pointer to analyzer
 * RETURN: void
 * COMPLEXITY: O(n) where n is length of line after comment start
 */
void skip_comment(LexicalAnalyzer *analyzer)
{
    if (analyzer->source[analyzer->position] == '/' &&
        analyzer->source[analyzer->position + 1] == '/') {
        analyzer->position += 2;
        while (analyzer->position < strlen(analyzer->source) &&
               analyzer->source[analyzer->position] != '\n') {
            analyzer->position++;
        }
    }
}

/*
 * FUNCTION: read_number
 * PURPOSE: Read and classify numeric literal (integer or float)
 * PARAMETERS: analyzer - pointer to analyzer, buffer - token storage
 * RETURN: TokenType (TOKEN_NUMBER_INT or TOKEN_NUMBER_FLOAT)
 * COMPLEXITY: O(n) where n is digit count in number
 */
TokenType read_number(LexicalAnalyzer *analyzer, char *buffer)
{
    int idx = 0;
    bool has_dot = false;

    /* Read all digits and at most one decimal point */
    while (analyzer->position < strlen(analyzer->source)) {
        char ch = analyzer->source[analyzer->position];
        if (isdigit(ch)) {
            buffer[idx++] = ch;
            analyzer->position++;
        } else if (ch == '.' && !has_dot && idx > 0) {
            has_dot = true;
            buffer[idx++] = ch;
            analyzer->position++;
        } else {
            break;
        }
    }
    buffer[idx] = '\0';
    return has_dot ? TOKEN_NUMBER_FLOAT : TOKEN_NUMBER_INT;
}

/*
 * FUNCTION: read_identifier_or_keyword
 * PURPOSE: Read identifier or keyword and determine its type
 * PARAMETERS: analyzer - pointer to analyzer, buffer - token storage
 * RETURN: TokenType (TOKEN_IDENTIFIER or TOKEN_KEYWORD)
 * COMPLEXITY: O(m) where m is lexeme length
 */
TokenType read_identifier_or_keyword(LexicalAnalyzer *analyzer, char *buffer)
{
    int idx = 0;

    /* Read alphanumeric and underscore characters */
    while (analyzer->position < strlen(analyzer->source)) {
        char ch = analyzer->source[analyzer->position];
        if (isalnum(ch) || ch == '_') {
            buffer[idx++] = ch;
            analyzer->position++;
        } else {
            break;
        }
    }
    buffer[idx] = '\0';

    return is_keyword(buffer) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
}

/*
 * FUNCTION: read_operator
 * PURPOSE: Read single or double character operator
 * PARAMETERS: analyzer - pointer to analyzer, buffer - token storage
 * RETURN: void
 * COMPLEXITY: O(1)
 */
void read_operator(LexicalAnalyzer *analyzer, char *buffer)
{
    char ch = analyzer->source[analyzer->position];
    char next_ch = (analyzer->position + 1 < strlen(analyzer->source)) ?
                   analyzer->source[analyzer->position + 1] : '\0';

    buffer[0] = ch;
    analyzer->position++;

    /* Check for two-character operators */
    if ((ch == '&' && next_ch == '&') ||
        (ch == '|' && next_ch == '|') ||
        (ch == '=' && next_ch == '=') ||
        (ch == '!' && next_ch == '=') ||
        (ch == '<' && next_ch == '=') ||
        (ch == '>' && next_ch == '=')) {
        buffer[1] = next_ch;
        buffer[2] = '\0';
        analyzer->position++;
    } else {
        buffer[1] = '\0';
    }
}

/*
 * FUNCTION: lexical_analyzer_next_token
 * PURPOSE: Extract the next token from source code
 * PARAMETERS: analyzer - pointer to analyzer
 * RETURN: Token structure containing type, lexeme, and line number
 * COMPLEXITY: O(m) where m is token length
 */
Token lexical_analyzer_next_token(LexicalAnalyzer *analyzer)
{
    Token token;
    token.line_number = analyzer->line_number;
    token.type = TOKEN_EOF;
    strcpy(token.lexeme, "");

    /* Skip whitespace and comments */
    while (analyzer->position < strlen(analyzer->source)) {
        skip_whitespace(analyzer);
        if (analyzer->position < strlen(analyzer->source) &&
            analyzer->source[analyzer->position] == '/') {
            skip_comment(analyzer);
        } else {
            break;
        }
    }

    /* Check for end of input */
    if (analyzer->position >= strlen(analyzer->source)) {
        token.type = TOKEN_EOF;
        return token;
    }

    char ch = analyzer->source[analyzer->position];
    char buffer[MAX_TOKEN_LENGTH];

    /* Number literal */
    if (isdigit(ch)) {
        token.type = read_number(analyzer, buffer);
        strcpy(token.lexeme, buffer);
    }
    /* Identifier or keyword */
    else if (isalpha(ch) || ch == '_') {
        token.type = read_identifier_or_keyword(analyzer, buffer);
        strcpy(token.lexeme, buffer);
    }
    /* Operator */
    else if (is_operator(ch, analyzer->source[analyzer->position + 1])) {
        token.type = TOKEN_OPERATOR;
        read_operator(analyzer, buffer);
        strcpy(token.lexeme, buffer);
    }
    /* Delimiter */
    else if (is_delimiter(ch)) {
        token.type = TOKEN_DELIMITER;
        buffer[0] = ch;
        buffer[1] = '\0';
        strcpy(token.lexeme, buffer);
        analyzer->position++;
    }
    /* Error case */
    else {
        token.type = TOKEN_ERROR;
        buffer[0] = ch;
        buffer[1] = '\0';
        strcpy(token.lexeme, buffer);
        analyzer->position++;
    }

    return token;
}

/*
 * FUNCTION: tokenize
 * PURPOSE: Perform complete lexical analysis on source code
 * PARAMETERS: analyzer - pointer to analyzer
 * RETURN: number of tokens generated
 * COMPLEXITY: O(n) where n is source code length
 */
int tokenize(LexicalAnalyzer *analyzer)
{
    Token token;
    while (1) {
        token = lexical_analyzer_next_token(analyzer);
        if (token.type == TOKEN_EOF) {
            break;
        }
        if (token.type != TOKEN_ERROR) {
            analyzer->tokens[analyzer->token_count++] = token;
        }
    }
    return analyzer->token_count;
}

/*
 * FUNCTION: get_token_type_name
 * PURPOSE: Convert TokenType enumeration to printable string
 * PARAMETERS: type - token type enumeration value
 * RETURN: string representation of token type
 * COMPLEXITY: O(1)
 */
const char *get_token_type_name(TokenType type)
{
    switch (type) {
        case TOKEN_KEYWORD:      return "KEYWORD";
        case TOKEN_IDENTIFIER:   return "IDENTIFIER";
        case TOKEN_NUMBER_INT:   return "NUMBER_INT";
        case TOKEN_NUMBER_FLOAT: return "NUMBER_FLOAT";
        case TOKEN_OPERATOR:     return "OPERATOR";
        case TOKEN_DELIMITER:    return "DELIMITER";
        case TOKEN_EOF:          return "EOF";
        case TOKEN_ERROR:        return "ERROR";
        default:                 return "UNKNOWN";
    }
}

/*
 * FUNCTION: display_token_table
 * PURPOSE: Display all recognized tokens in a formatted table
 * PARAMETERS: analyzer - pointer to analyzer with token array
 * RETURN: void
 * COMPLEXITY: O(n) where n is number of tokens
 */
void display_token_table(LexicalAnalyzer *analyzer)
{
    printf("\n");
    printf("==============================================================\n");
    printf("                    LEXICAL ANALYSIS RESULTS\n");
    printf("==============================================================\n");
    printf("%-20s %-20s %-15s\n", "Token Type", "Lexeme", "Line Number");
    printf("--------------================================================\n");

    for (int i = 0; i < analyzer->token_count; i++) {
        Token *token = &analyzer->tokens[i];
        printf("%-20s %-20s %-15d\n",
               get_token_type_name(token->type),
               token->lexeme,
               token->line_number);
    }

    printf("--------------================================================\n");
    printf("Total Tokens: %d\n", analyzer->token_count);
    printf("==============================================================\n\n");
}

/* ======================== Main Program ======================== */

int main(void)
{
    /*
     * EXAMPLE USED:
     * This program analyzes a sample C-like source code containing:
     * - Keywords: if, else, while, for, int, float, return
     * - Identifiers: count, sum, value, result
     * - Integer and float literals: 10, 20, 0, 3.14, 2.71
     * - Operators: +, =, ==, !=, >, <, &&, ||, *
     * - Delimiters: ;, {, }, (, ), ,
     * - Comments: // single line comments
     */

    const char *source_code =
        "// Example C-like code\n"
        "int main() {\n"
        "    int count = 10;\n"
        "    float value = 3.14;\n"
        "    if (count > 0) {\n"
        "        sum = count + value;\n"
        "    }\n"
        "    while (count != 0) {\n"
        "        count = count - 1;\n"
        "    }\n"
        "    return 0;\n"
        "}\n";

    LexicalAnalyzer analyzer;
    lexical_analyzer_init(&analyzer, source_code);

    printf("\n");
    printf("======================================================================\n");
    printf("        LEXICAL ANALYZER - COMPILER DESIGN LABORATORY\n");
    printf("======================================================================\n");
    printf("\nSource Code:\n");
    printf("----------------------------------------------------------------------\n");
    printf("%s", source_code);
    printf("----------------------------------------------------------------------\n");

    /* Perform lexical analysis */
    int token_count = tokenize(&analyzer);

    /* Display results */
    display_token_table(&analyzer);

    printf("Status: Lexical analysis completed successfully.\n");
    printf("Total tokens recognized: %d\n", token_count);

    return 0;
}
