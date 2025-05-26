/*
    Lexer for Oxygen Language.
*/

#include "error.h"
#include "utarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum OxxTokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,

  // Keywords.
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,
  END,

  EOF_
} OxxTokenType;

typedef struct OxxToken {
  enum OxxTokenType type;
  int line;
  char *literal;
  char *lexeme;
} OxxToken;

UT_icd oxxToken_icd = {sizeof(OxxToken), NULL, NULL, NULL};
UT_array *tokens;
OxxToken token, *p;
char *source;
char e = '=';
char s = '/';
int start = 0;
int current = 0;
int line = 1;

char* OxxStringify (OxxToken token) {
    /* get lenght of string required to hold struct values */
    size_t len = 0;
    len = snprintf (NULL, len, "%u,%s,%s,%d", token.type, token.lexeme, token.literal,
                    token.line);

    /* allocate/validate string to hold all values (+1 to null-terminate) */
    char *tokenstr = calloc (1, sizeof *tokenstr * len + 1);
    if (!tokenstr) {
        fprintf (stderr, "%s() error: virtual memory allocation failed.\n", __func__);
    }

    /* write/validate struct values to tokenstr */
    if (snprintf (tokenstr, len + 1, "%u,%s,%s,%d", token.type, token.lexeme, token.literal,
                    token.line) > len + 1)
    {
        fprintf (stderr, "%s() error: snprintf returned truncated result.\n", __func__);
        return NULL;
    }

    return tokenstr;
};

int _IsEnd() { return current >= strlen(source); };

char _Advance() { return source[current++]; };

int _Match(char expected) {
  if (_IsEnd())
    return 0;
  if (source[current] != expected)
    return 0;

  current++;
  return 1;
};

char _Peek() {
  if (_IsEnd())
    return '\0';

  return source[current];
};

void _AddToken(OxxTokenType type, char *literal) {
  token.type = type;
  token.literal = literal; // todo: implement literals
  token.line = line;
  strncpy(token.lexeme, source + start, start + current);
  utarray_push_back(tokens, &token);
};

UT_array *OxxScanTokens(char src[]) {
  utarray_new(tokens, &oxxToken_icd);
  source = src;

  while (!_IsEnd()) {
    start = current;
    switch (_Advance()) {
    case '(':
      _AddToken(LEFT_PAREN, NULL);
      break;
    case ')':
      _AddToken(RIGHT_PAREN, NULL);
      break;
    case '{':
      _AddToken(LEFT_BRACE, NULL);
      break;
    case '}':
      _AddToken(RIGHT_BRACE, NULL);
      break;
    case ',':
      _AddToken(COMMA, NULL);
      break;
    case '.':
      _AddToken(DOT, NULL);
      break;
    case '-':
      _AddToken(MINUS, NULL);
      break;
    case '+':
      _AddToken(PLUS, NULL);
      break;
    case ';':
      _AddToken(SEMICOLON, NULL);
      break;
    case '*':
      _AddToken(STAR, NULL);
      break;
    case '!':
      _AddToken(_Match(e) ? BANG_EQUAL : BANG, NULL);
      break;
    case '=':
      _AddToken(_Match(e) ? EQUAL_EQUAL : EQUAL, NULL);
      break;
    case '<':
      _AddToken(_Match(e) ? LESS_EQUAL : LESS, NULL);
      break;
    case '>':
      _AddToken(_Match(e) ? GREATER_EQUAL : GREATER, NULL);
      break;
    case '/': {
      if (_Match(s)) {
        while (_Peek() && !_IsEnd())
          _Advance();
      } else {
        _AddToken(SLASH, NULL);
      }
    }; break;
    case ' ':
    case '\r':
    case '\t':
      break;

    case '\n':
      line++;
      break;

    default:
      ErrorP(line, source[current], "Unexpected character at:");
      break;
    }
  }



  return tokens;
};