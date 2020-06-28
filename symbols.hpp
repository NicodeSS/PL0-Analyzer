#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "utils.hpp"
extern Info *info;

/* symbol list */
typedef enum
{
    nul,       // NULL
    eof,       // EOF
    ident,     // identifier
    number,    // number
    plus,      // +
    minus,     // -
    times,     // *
    slash,     // /
    eql,       // =
    neq,       // #
    lss,       // <
    leq,       // <=
    gtr,       // >
    geq,       // >=
    lparen,    // (
    rparen,    // )
    comma,     // ,
    semicolon, // ;
    period,    // .
    becomes,   // :=
    beginsym,  // "begin"
    endsym,    // "end"
    ifsym,     // "if"
    thensym,   // "then"
    elsesym,   // "else"
    whilesym,  // "while"
    dosym,     // "do"
    repeatsym, // "repeat"
    untilsym,  // "until"
    callsym,   // "call"
    readsym,   // "read"
    writesym,  // "write"
    constsym,  // "const"
    varsym,    // "var"
    procsym,   // "procedure"
    oddsym     // is odd or not
} SYMBOL;

/* symbol labels */
const std::string labels[] = {
    "null",
    "eof",
    "identifier",
    "number",
    "plus",
    "minus",
    "times",
    "slash",
    "eql",
    "neq",
    "lss",
    "leq",
    "gtr",
    "geq",
    "lparen",
    "rparen",
    "comma",
    "semicolon",
    "period",
    "becomes",
    "beginsym",
    "endsym",
    "ifsym",
    "thensym",
    "elsesym",
    "whilesym",
    "dosym",
    "repeatsym",
    "untilsym",
    "callsym",
    "readsym",
    "writesym",
    "constsym",
    "varsym",
    "procsym",
    "oddsym"};

/* symbol keywords */
const std::string keywords[] = {
    "begin", "end", "if", "then", "else",
    "while", "do", "repeat", "until", "call",
    "read", "write", "const", "var", "procedure",
    "odd"};

/* cursor position */
typedef struct _POS_T
{
    int row;
    int col;
} pos_t;

/* token information */
typedef struct _TOKEN_T
{
    unsigned int seq;
    std::string text;
    SYMBOL type;
    struct _TOKEN_T *next;
} token_t;

/* the exception throw when getSymbol() got a invalid symbol */
class InvalidSymbolException : public std::exception
{
    std::string s;
    pos_t pos;

public:
    InvalidSymbolException(std::string s, pos_t pos) : s(s), pos(pos) {}
    ~InvalidSymbolException() throw(){};
    const char *what() const throw();
    std::string where();
};

const int KEYWORD_NUM = 16;
const int SYM_OFFSET = 20;

/* program only permit number that length < MAX_NUMBER_LENGTH */
const int MAX_NUMBER_LENGTH = 14;
/* program only permit identifier that length < MAX_IDENTIFIER_LENGTH */
const int MAX_IDENTIFIER_LENGTH = 10;

/* how many token will program prealloc */
const int PREALLOC_TOKEN_NUM = 64;

/* get a char(as int) from ifstream and set cursor */
int _get();
/* putback a char(as int) to ifstream and reset cursor */
int _unget(int);

/* initialize */
void init();
/* set err cursor to current cursor */
void set_errcur();

/* get a symbol from ifstream */
SYMBOL getSymbol();
/* get a symbol from keyword */
SYMBOL getSymbolFromId(std::string);

/* create a token_t object */
token_t *token_create(int, std::string, SYMBOL);
/* pack current symbol and add to link list */
const token_t *token_add();

/* print result */
void result_print();

#endif